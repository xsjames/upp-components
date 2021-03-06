#include "Terminal.h"

// Basic ANSI, dynamic, and extended colors support.
// See: https://invisible-island.net/xterm/ctlseqs/ctlseqs.html#h2-Operating-System-Commands

#define LLOG(x)		// RLOG("Terminal: " << x)
#define LTIMING(x)	// RTIMING(x)

namespace Upp {

Terminal& Terminal::ResetColors()
{
	// The U++ color constants with 'S' prefix are automatically adjusted
	// to the color theme of OS. On the other hand, the 8 ANSI colors and
	// their brighter  counterparts are assumed to be constant. Therefore
	// it would be better if we avoid using the auto-adjusted versions by
	// default, and leave it up to client code to change them  on demand.
	// Note that this rule does not apply to the  default ink, paper, and
	// selection colors.
	
	colortable[COLOR_BLACK] = Black();
	colortable[COLOR_RED] = Red();
	colortable[COLOR_GREEN] = Green();
	colortable[COLOR_YELLOW] = Yellow();
	colortable[COLOR_BLUE] = Blue();
	colortable[COLOR_MAGENTA] = Magenta();
	colortable[COLOR_CYAN] = Cyan();
	colortable[COLOR_WHITE] = White();

	colortable[COLOR_LTBLACK] = Black();
	colortable[COLOR_LTRED] = LtRed();
	colortable[COLOR_LTGREEN] = LtGreen();
	colortable[COLOR_LTYELLOW] = LtYellow();
	colortable[COLOR_LTBLUE] = LtBlue();
	colortable[COLOR_LTMAGENTA] = LtMagenta();
	colortable[COLOR_LTCYAN] = LtCyan();
	colortable[COLOR_LTWHITE] = White();

	colortable[COLOR_INK] = SColorText;
	colortable[COLOR_INK_SELECTED] = SColorHighlightText;
	colortable[COLOR_PAPER] = SColorPaper;
	colortable[COLOR_PAPER_SELECTED] = SColorHighlight;

	return *this;
}

void Terminal::SetInkAndPaperColor(const VTCell& cell, Color& ink, Color& paper)
{
	ink = GetColorFromIndex(cell, COLOR_INK);
	paper = GetColorFromIndex(cell, COLOR_PAPER);

	if(cell.IsInverted())
		Swap(ink, paper);
	if(modes[DECSCNM])
		Swap(ink, paper);
	if(hyperlinks && cell.IsHyperlink() && activelink == cell.data)
		Swap(ink, paper);
}

Color Terminal::GetColorFromIndex(const VTCell& cell, int which) const
{
	Color color = which == COLOR_INK ? cell.ink : cell.paper;
	bool dim = which == COLOR_INK && cell.IsFaint();

	auto AdjustBrightness = [](Color c, double v) -> Color
	{
		double hc, sc, vc;
		RGBtoHSV(c.GetR() / 255.0, c.GetG() / 255.0, c.GetB() / 255.0, hc, sc, vc);
		return HsvColorf(hc, sc, vc * v);
	};

	int index = which;
	
	if(!IsNull(color)) {
		int c = color.GetSpecial();
		if(c >= 0) {
			index = c;
			if((index) > 15) {	// 256-color (6x6x6 cube)
				byte r, g, b;
				if(index < 232) {
					r =	(( index - 16) / 36) * 51,
					g =	(((index - 16) % 36) / 6) * 51,
					b =	(( index - 16) % 6)  * 51;
				}
				else  // Grayscale
					r = g = b = (index - 232) * 10 + 8;
				color = Color(r, g, b);
				goto End;
			}
		}
		else
			goto End;
	}

	if(lightcolors ||
		(intensify && which == COLOR_INK && cell.IsBold()))
			if(index < 8)
				index += 8;

	color = colortable[index];	// Adjust only the first 16 colors.

	if(adjustcolors)
		color = AdjustIfDark(color);

End:
	return dim ? AdjustBrightness(color, 0.70) : color;
}
	
void Terminal::ReportANSIColor(int opcode, int index, const Color& c)
{
	String reply = Format("%d;%d;%", opcode, index, ConvertColor().Format(c));

	LLOG("ReportAnsiColor() -> OSC " << reply);
	
	PutOSC(reply);
}

void Terminal::ReportDynamicColor(int opcode, const Color& c)
{
	String reply = Format("%d;%", opcode, ConvertColor().Format(c));
		
	LLOG("ReportDynamicColor() -> OSC " << reply);
	
	PutOSC(reply);
}

void Terminal::ChangeColors(int opcode, const String& oscs, bool reset)
{
	if(!dynamiccolors)
		return;
	
	Vector<String> params = Split(oscs, ';', false);
	VectorMap<int, String> colormap;

	bool ansicolors = opcode == 4 || opcode == 104;
	int pos = (ansicolors && !reset) ? 1 : 0;
	
	for(int i = pos; i < params.GetCount() - pos; i += 2)
		if(i + 1 < params.GetCount())
			colormap.Add(StrInt(params[i]), params[i + 1]);

	if(colormap.IsEmpty())
		return;
	
	int changed_colors = 0;
	
	for(int i = 0; i < colormap.GetCount(); i++) {
		int index = colormap.GetKey(i);
		String colorspec = colormap[i];
		if(SetColorTable(opcode, index, colorspec, ansicolors, reset))
			changed_colors++;
	}

	if(changed_colors > 0)
		Ctrl::Refresh();
}

bool Terminal::SetColorTable(int opcode, int index, String colorspec, bool ansicolor, bool reset)
{
	if(ansicolor && index >= ANSI_COLOR_COUNT)
		return false;
	else
	if(!ansicolor) {
		switch(opcode) {
		case 10:
		case 110:
			index = COLOR_INK;
			break;
		case 11:
		case 111:
			index = COLOR_PAPER;
			break;
		case 17:
		case 117:
			index = COLOR_INK_SELECTED;
			break;
		case 19:
		case 119:
			index = COLOR_PAPER_SELECTED;
			break;
		default:
			LLOG("Unhandled dynamic color opcode: " << opcode);
			return false;
		}
	}
	Color c;
	if(colorspec.IsEqual("?")) { // Report color value
		c = colortable[index];
		if(ansicolor)
			ReportANSIColor(opcode, index, c);
		else
			ReportDynamicColor(opcode, c);
		return false;
	}

	return reset
			? ResetLoadColor(index)
			: SetSaveColor(index, ConvertColor().Scan(colorspec));
}

bool Terminal::SetSaveColor(int index, const Color& c)
{
	if(IsNull(c))
		return false;
	if(savedcolors.Find(index) < 0)
		savedcolors.Add(index, colortable[index]);
	colortable[index] = c;
	return true;
}

bool Terminal::ResetLoadColor(int index)
{
	int i = savedcolors.Find(index);
	if(i < 0)
		return false;
	colortable[index] = savedcolors[i];
	savedcolors.Remove(i);
	return true;
}

static int sParseExtendedColorFormat(Color& c, int& which, int& palette, const String& s)
{
	// TODO: This function can be more streamlined.
	
	auto SgrDelimiters = [](int c) -> int
	{
		return c == ':' || c == ';';	// Alloe both colon and semicolon.
	};
	
	Vector<String> h = Split(s, SgrDelimiters, false);
	int count = h.GetCount();
	if(3 <= count && count < 8 && (h[0].IsEqual("38") || h[0].IsEqual("48"))) {
		which = StrInt(h[0]);
		palette  = StrInt(h[1]);
		int index = 2;
		if(palette == 2 && (4 < count && count < 8)) {		// True color (RGB)
			index += (int) count > 5;
			int r =	clamp(StrInt(h[index++]), 0, 255);
			int g =	clamp(StrInt(h[index++]), 0, 255);
			int b =	clamp(StrInt(h[index]),   0, 255);
			c = Color(r, g, b);
			return index;
		}
		else
		if(palette == 3 && (4 < count && count < 8)) {		// True color (CMY)
			index += (int) count > 5;
			double c = StrInt(h[index++]) / 100.0;
			double m = StrInt(h[index++]) / 100.0;
			double y = StrInt(h[index])   / 100.0;
			c = CmykColorf(c, m, y, 0.0);
			return index;
		}
		else
		if(palette == 4 && (6 == count || count == 7)) {	// True color (CMYK)
			index += (int) count > 6;
			double c = StrInt(h[index++]) / 100.0;
			double m = StrInt(h[index++]) / 100.0;
			double y = StrInt(h[index++]) / 100.0;
			double k = StrInt(h[index])   / 100.0;
			c = CmykColorf(c, m, y, k);
			return index;
		}
		else
		if(palette == 5 && count >= 3) {					// Indexed (256-color, 6x6x6 cube)
			int ix = clamp(StrInt(h[index]), 0, 255);
			c = Color::Special(ix);
			return index;
		}
	}
	return 1;
}

void Terminal::ParseExtendedColors(VTCell& attrs, const Vector<String>& opcodes, int& index)
{
	// TODO: Optimixization.
	
	LTIMING("Terminal::SetISOColor");

	// Recognized color sequene formats:

	// ISO-8613-6:
	// SGR 38 : 2 : [IGNORED] : R : G : B
	// SGR 48 : 2 : [IGNORED] : R : G : B
	// SGR 38 : 3 : [IGNORED] : C : M : Y
	// SGR 48 : 3 : [IGNORED] : C : M : Y
	// SGR 38 : 4 : [IGNORED] : C : M : Y : K
	// SGR 48 : 4 : [IGNORED] : C : M : Y : K

	// SGR 38 : 2 : R : G : B
	// SGR 48 : 2 : R : G : B
	// SGR 38 : 3 : C : M : Y
	// SGR 48 : 3 : C : M : Y
	// SGR 38 : 4 : C : M : Y : K
	// SGR 48 : 4 : C : M : Y : K
	// SGR 38 : 5 : I
	// SGR 38 : 5 : I


	// ISO-8613-6 (mixed):
	// SGR 38 ; 2 : [IGNORED] : R : G : B
	// SGR 48 ; 2 : [IGNORED] : R : G : B
	// SGR 38 ; 3 : [IGNORED] : C : M : Y
	// SGR 48 ; 3 : [IGNORED] : C : M : Y
	// SGR 38 ; 4 : [IGNORED] : C : M : Y : K
	// SGR 48 ; 4 : [IGNORED] : C : M : Y : K

	// SGR 38 ; 2 : R : G : B
	// SGR 48 ; 2 : R : G : B
	// SGR 38 ; 3 : C : M : Y
	// SGR 48 ; 3 : C : M : Y
	// SGR 38 ; 4 : C : M : Y : K
	// SGR 48 ; 4 : C : M : Y : K
	// SGR 38 ; 5 : I
	// SGR 38 ; 5 : I

	// Legacy (used by xterm):
	// SGR 38 ; 2 ; R ; G ; B
	// SGR 48 ; 2 ; R ; G ; B
	// SGR 38 ; 5 ; I
	// SGR 38 ; 5 ; I

	auto GetSubParameterCount = [](const String& s) -> int
	{
		int rc = 0;
		for(const int& c : s)
			rc += c == ':';
		return rc + (rc != 0);
	};

	int remaining = opcodes.GetCount() - index;
	int which   = 0;
	int format  = 0;
	int palette = 0;
	Color color = Null;
	
	if(GetSubParameterCount(opcodes[index])) {
		index += sParseExtendedColorFormat(color, which, palette, opcodes[index]);
	}
	else
	if(remaining > 1) {
		if(GetSubParameterCount(opcodes[index + 1])) {
			String s = opcodes[index] + ":" + opcodes[index + 1];
			index += sParseExtendedColorFormat(color, which, palette, s);
		}
		else {
			int count = min(remaining, 6);
			auto r = SubRange(opcodes, index, count);
			String s = Join((Vector<String>&) r, ":", false);
			index += sParseExtendedColorFormat(color, which, palette, s);
		}
	}
	else return;

	if(2 == palette || palette == 5) {
		switch(which) {
		case 38:
			attrs.ink = color;
			break;
		case 48:
			attrs.paper = color;
			break;
		}
	}
}

void Terminal::ColorTableSerializer::Serialize(Stream& s)
{
	for(int i = 0; i < Terminal::MAX_COLOR_COUNT; i++)
		s % table[i];
}

void Terminal::ColorTableSerializer::Jsonize(JsonIO& jio)
{
	for(int i = 0; i < Terminal::MAX_COLOR_COUNT; i++)
		switch(i) {
		case Terminal::COLOR_INK:
			jio("Ink", table[i]);
			break;
		case Terminal::COLOR_PAPER:
			jio("Paper", table[i]);
			break;
		case Terminal::COLOR_INK_SELECTED:
			jio("SelectionInk", table[i]);
			break;
		case Terminal::COLOR_PAPER_SELECTED:
			jio("SelectionPaper", table[i]);
			break;
		default:
			jio(Format("Color_%d", i), table[i]);
			break;
		}
}

void Terminal::ColorTableSerializer::Xmlize(XmlIO& xio)
{
	XmlizeByJsonize(xio, *this);
}

static int sCharFilterHashHex(int c)
{
	return IsXDigit(c) || c == '#' ? c : 0;
}

int ConvertHashColorSpec::Filter(int chr) const
{
	return sCharFilterHashHex(chr);
}

Value ConvertHashColorSpec::Scan(const Value& text) const
{
	String s = Upp::Filter((const String&) text, sCharFilterHashHex);
	if(!s.IsEmpty()) {
		int i = 0 + (int) s[0] == '#';
		int64 x = ScanInt64(~s + i, nullptr, 16);
		switch(s.GetCount() - i) {
		case 3:		// Hash3
			return Color(byte(x >> 4) & 0xF0, byte(x) & 0xF0, byte(x << 4));
		case 6:		// Hash6
			return Color(byte(x >> 16), byte(x >> 8), byte(x));
		case 9:		// Hash9
			return Color(byte(x >> 28), byte(x >> 16), byte(x >> 4));
		case 12:	// Hash12
			return Color(byte(x >> 40), byte(x >> 24), byte(x >> 8));
		default:
			break;
		}
	}
	return Upp::ErrorValue(t_("Bad hash color text format"));
}

Value ConvertHashColorSpec::Format(const Value& q) const
{
	if(q.Is<Color>()) {
		const Color& c = (Color&) q;
		return Upp::Format("#%02x%02x%02x", c.GetR(), c.GetG(), c.GetB());
	}
	return Upp::ErrorValue(t_("Bad color value"));
}

int ConvertRgbColorSpec::Filter(int chr) const
{
	return IsXDigit(chr)	// 'B' and 'b' are also hex digits...
		|| chr == 'r' || chr == 'R'
		|| chr == 'g' || chr == 'G'
		|| chr == ':' || chr == '/'	|| chr == ','
			? chr : 0;
}

Value ConvertRgbColorSpec::Scan(const Value& text) const
{
	auto Delimiters = [](int c) -> int
	{
		return c == ':' || c == '/' || c == ',';
	};
	
	Vector<String> h = Split(ToLower((const String&) text), Delimiters);
	int count = h.GetCount();

	if(count == 3
    || (count == 4 && h[0].IsEqual("rgb"))                  // rgb : %04x / %04x / %04x
    || (count == 5 && h[0].IsEqual("rgba"))) {              // rgb : %02z / %02x / %02x
        int index = 0;                                      // rgba : %04x / %04x / %04x / %04x
        int radix = 10;                                     // rgba : %02x / %02x / %02x / %02x
        if(count > 3) { index = 1; radix = 16; }            // %u , %u, %u
		int r = ScanInt(~h[index++], nullptr, radix);
		int g = ScanInt(~h[index++], nullptr, radix);
		int b = ScanInt(~h[index++], nullptr, radix);
		int a = count == 5 ? ScanInt(~h[index], nullptr, radix) : 255;
		if(!IsNull(r)
		&& !IsNull(g)
		&& !IsNull(b)
		&& !IsNull(a)) {
			RGBA rgba;
			rgba.r = byte(r > 255 ? r >> 8 : r);
			rgba.g = byte(g > 255 ? g >> 8 : g);
			rgba.b = byte(b > 255 ? b >> 8 : b);
			rgba.a = byte(a > 255 ? a >> 8 : a);
			return Color(rgba);
		}
	}
	
	return Upp::ErrorValue(t_("Bad rgb/a color text format"));
}

Value ConvertRgbColorSpec::Format(const Value& q) const
{
	if(q.Is<Color>()) {
		const Color& c = (Color&) q;
		return Upp::Format("rgb:%04x/%04x/%04x", c.GetR() * 257, c.GetG() * 257, c.GetB() * 257);
	}
	return Upp::ErrorValue(t_("Bad color value"));
}

int ConvertCmykColorSpec::Filter(int chr) const
{
	return IsXDigit(chr)	// 'C' and 'c' are also hex digits...
		|| chr == 'm' || chr == 'y' || chr == 'k'
		|| chr == 'M' || chr == 'Y' || chr == 'K'
		|| chr == ':' || chr == '/' || chr == ',' || chr == '.'
			?  chr : 0;
}

Value ConvertCmykColorSpec::Scan(const Value& text) const
{
	auto Delimiters = [](int c) -> int
	{
		return c == ':' || c == '/';
	};
	
	Vector<String> h = Split(ToLower((const String&) text), Delimiters);
	int count = h.GetCount();
	if((count == 5 && h[0].IsEqual("cmyk"))		// cmyk : %f / %f / %f / %f
	|| (count == 4 && h[0].IsEqual("cmy"))) {	// cmy  : %f / %f / %f
		double c = ScanDouble(~h[1]);
		double m = ScanDouble(~h[2]);
		double y = ScanDouble(~h[3]);
		double k = count == 5 ? ScanDouble(~h[4]) : 0.0;
		if(!IsNull(c)
		&& !IsNull(m)
		&& !IsNull(y)
		&& !IsNull(k))
			return CmykColorf(c, m, y, k);
	}
	return Upp::ErrorValue(t_("Bad cmy/k color text format"));
}

Value ConvertCmykColorSpec::Format(const Value& q) const
{
	if(q.Is<Color>()) {
		const Color& r = (Color&) q;
		double c, m, y, k;
		RGBtoCMYK(r.GetR() / 255.0, r.GetG() / 255.0, r.GetB() / 255.0, c, m, y, k);
		return Upp::Format("cmyk:%f/%f/%f/%f", c, m, y, k);
	}
	return Upp::ErrorValue(t_("Bad color value"));
}

Value ConvertColor::Scan(const Value& text) const
{
	Value v = ConvertHashColorSpec().Scan(text);
	if(IsError(v)) {
		v = ConvertRgbColorSpec().Scan(text);
		if(IsError(v))
			v = ConvertCmykColorSpec().Scan(text);
	}
	return v;
}

Value ConvertColor::Format(const Value& q) const
{
	return ConvertRgbColorSpec().Format(q);
}

}