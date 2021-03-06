#ifndef _Terminal_Terminal_h
#define _Terminal_Terminal_h

#include <CtrlLib/CtrlLib.h>
#include <plugin/jpg/jpg.h>

#include "Parser.h"
#include "Page.h"
#include "Sixel.h"

namespace Upp {

class Terminal : public Ctrl {
public:
    const int ANSI_COLOR_COUNT = 16;    // Actually, ANSI + aixterm colors.

    enum Colors
    {
        COLOR_BLACK = 0,
        COLOR_RED,
        COLOR_GREEN,
        COLOR_YELLOW,
        COLOR_BLUE,
        COLOR_MAGENTA,
        COLOR_CYAN,
        COLOR_WHITE,
        COLOR_LTBLACK,
        COLOR_LTRED,
        COLOR_LTGREEN,
        COLOR_LTYELLOW,
        COLOR_LTBLUE,
        COLOR_LTMAGENTA,
        COLOR_LTCYAN,
        COLOR_LTWHITE,
        COLOR_INK,
        COLOR_INK_SELECTED,
        COLOR_PAPER,
        COLOR_PAPER_SELECTED,
        MAX_COLOR_COUNT
    };

    enum ConformanceLevels
    {
        LEVEL_0 = 0,
        LEVEL_1,
        LEVEL_2,
        LEVEL_3,
        LEVEL_4
    //  LEVEL_5
    };

    enum LEDs
    {
        LED_NUMLOCK  = 0,
        LED_CAPSLOCK,
        LED_SCRLOCK,
        LED_ALL
    };

   enum TimerIds
   {
        TIMEID_REFRESH = Ctrl::TIMEID_COUNT,
        TIMEID_SIZEHINT,
        TIMEID_BLINK,
        TIMEID_COUNT
    };

    // Inline image data structure.
    struct InlineImage : ValueType<InlineImage, 999, Moveable<InlineImage> > {
        Image       image;
        Size        cellsize;
        Size        fontsize;
        Rect        paintrect;
        operator    Value() const                               { return RichValue<Terminal::InlineImage>(*this); }
    };

    typedef Terminal CLASSNAME;

    Terminal();
    virtual ~Terminal()                                         {}

    Event<>              WhenBell;
    Event<>              WhenResize;
    Event<Size>          WhenSetSize;
    Event<Bar&>          WhenBar;
    Event<String>        WhenTitle;
    Event<String>        WhenOutput;
    Event<int, bool>     WhenLED;
    Gate<PasteClip&>     WhenClip;
    Event<const String&> WhenLink;
    Event<const String&> WhenImage;

    // Window Ops support.
    Event<bool>          WhenWindowMinimize;
    Event<bool>          WhenWindowMaximize;
    Event<int>           WhenWindowFullScreen;
    Event<Rect>          WhenWindowGeometryChange;

    // APC support.
    Event<const String&> WhenApplicationCommand;

    void        Write(const void *data, int size, bool utf8 = true);
    void        Write(const String& s, bool utf8 = true)        { Write(~s, s.GetLength(), utf8); }
    void        WriteUtf8(const String& s)                      { Write(s, true);         }
    void        CheckWriteUtf8(const String& s)                 { Write(s, CheckUtf8(s)); }

    Terminal&   SetLevel(int level)                             { SetEmulation(level); return *this; }
    bool        IsLevel0() const                                { return !modes[DECANM]; }
    bool        IsLevel1() const                                { return modes[DECANM] && clevel >= LEVEL_1; }
    bool        IsLevel2() const                                { return modes[DECANM] && clevel >= LEVEL_2; }
    bool        IsLevel3() const                                { return modes[DECANM] && clevel >= LEVEL_3; }
    bool        IsLevel4() const                                { return modes[DECANM] && clevel >= LEVEL_4; }

    Terminal&   Set8BitMode(bool b = true)                      { eightbit = b; return *this; }
    Terminal&   No8BitMode()                                    { return Set8BitMode(false); }
    bool        Is8BitMode() const                              { return IsLevel2() && eightbit; }
    bool        Is7BitMode() const                              { return !Is8BitMode(); }

    void        HardReset()                                     { Reset(true);  }
    void        SoftReset()                                     { Reset(false); }

    Terminal&   History(bool b = true)                          { dpage.History(b); return *this; }
    Terminal&   NoHistory()                                     { return History(false); }
    Terminal&   ClearHistory()                                  { dpage.EraseHistory(); return *this; }
    bool        HasHistory() const                              { return dpage.HasHistory(); }

    Terminal&   SetHistorySize(int sz)                          { dpage.SetHistorySize(sz); return *this; }
    int         GetHistorySize() const                          { return dpage.GetHistorySize(); }

    Terminal&   SetFont(Font f)                                 { font = f; Layout(); return *this; }
    Font        GetFont() const                                 { return font; }

    void        SetCharset(byte cs)                             { charset = ResolveCharset(cs); }
    byte        GetCharset() const                              { return charset;    }

    Terminal&   Ink(Color c)                                    { SetRefreshColor(COLOR_INK, c); return *this; }
    Terminal&   Paper(Color c)                                  { SetRefreshColor(COLOR_PAPER, c); return *this; }
    Terminal&   SelectionInk(Color c)                           { SetRefreshColor(COLOR_INK_SELECTED, c); return *this; }
    Terminal&   SelectionPaper(Color c)                         { SetRefreshColor(COLOR_PAPER_SELECTED, c); return *this; }

    Terminal&   SetColor(int i, Color c)                        { colortable[i] = c; return *this; }
    void        SetRefreshColor(int i, Color c)                 { SetColor(i, c); Refresh(); }
    Color       GetColor(int i) const                           { return colortable[i]; }

    Terminal&   DynamicColors(bool b = true)                    { dynamiccolors = b; return *this; }
    Terminal&   NoDynamicColors()                               { return DynamicColors(false); }
    bool        HasDynamicColors() const                        { return dynamiccolors; }

    Terminal&   LightColors(bool b = true)                      { lightcolors = b; Refresh(); return *this; }
    Terminal&   NoLightColors()                                 { return LightColors(false); }
    bool        HasLightColors() const                          { return lightcolors; }

    Terminal&   AdjustColors(bool b = true)                     { adjustcolors = b; Refresh(); return *this; }
    Terminal&   NoAdjustColors()                                { return AdjustColors(false); }
    bool        HasAdjustedColors() const                       { return adjustcolors; }

    Terminal&   ResetColors();

    Terminal&   IntensifyBoldText(bool b = true)                { intensify = b; Refresh(); return *this; }
    Terminal&   NoIntensifyBoldText()                           { return IntensifyBoldText(false); }
    bool        HasIntensifiedBoldText() const                  { return intensify; }

    Terminal&   BlinkingText(bool b = true)                     { blinkingtext = b; RefreshDisplay(); return *this; }
    Terminal&   NoBlinkingText()                                { return BlinkingText(false); }
    bool        HasBlinkingText() const                         { return blinkingtext; }

    Terminal&   BlinkInterval(int ms)                           { blinkinterval = clamp(ms, 100, 60000); return *this; }

    Terminal&   SetCursorStyle(int style, bool blink = true)    { caret.Set(style, blink); return *this;}
    int         GetCursorStyle() const                          { return caret.GetStyle(); }
    Terminal&   BlockCursor(bool blink = true)                  { caret.Block(blink); return *this; }
    Terminal&   BeamCursor(bool blink = true)                   { caret.Beam(blink);  return *this; }
    Terminal&   UnderlineCursor(bool blink = true)              { caret.Underline(blink); return *this; }
    Terminal&   BlinkingCursor(bool b = true)                   { caret.Blink(b); return *this; }
    Terminal&   NoBlinkingCursor()                              { return BlinkingCursor(false); }
    bool        IsCursorBlinking() const                        { return caret.IsBlinking();    }
    Terminal&   LockCursor(bool b = true)                       { caret.Lock(b);  return *this; }
    Terminal&   UnlockCursor()                                  { caret.Unlock(); return *this; }
    bool        IsCursorLocked() const                          { return caret.IsLocked();      }

    Terminal&   NoBackground(bool b = true)                     { nobackground = b; Transparent(b); Refresh(); return *this; }
    bool        HasBackground() const                           { return !nobackground; }

    Terminal&   ShowSizeHint(bool b = true)                     { sizehint = b; return *this; }
    Terminal&   HideSizeHint()                                  { return ShowSizeHint(false); }

    Terminal&   ShowScrollBar(bool b = true);
    Terminal&   HideScrollBar()                                 { return ShowScrollBar(false);  }
    Terminal&   SetScrollBarStyle(const ScrollBar::Style& s)    { sb.SetStyle(s); return *this; }

    Terminal&   AlternateScroll(bool b = true)                  { alternatescroll = b; return *this; }
    Terminal&   NoAlternateScroll()                             { return AlternateScroll(false); }
    bool        HasAlternateScroll() const                      { return alternatescroll; }

    Terminal&   MouseWheelStep(int lines)                       { wheelstep = max(1, lines); return *this; }

    Terminal&   AutoHideMouseCursor(bool b = true)              { hidemousecursor = b; return *this; }
    Terminal&   NoAutoHideMouseCurosr()                         { return AutoHideMouseCursor(false); }
    bool        IsMouseCursorAutoHidden() const                 { return hidemousecursor; }

    Terminal&   KeyNavigation(bool b = true)                    { keynavigation = b; return *this; }
    Terminal&   NoKeyNavigation()                               { return KeyNavigation(false); }
    bool        HasKeyNavigation() const                        { return keynavigation; }

    Terminal&   InlineImages(bool b = true)                     { sixelimages = jexerimages = iterm2images = b; return *this; }
    Terminal&   NoInlineImages()                                { return InlineImages(false);  }
    bool        HasInlineImages() const                         { return sixelimages || jexerimages || iterm2images; }

    Terminal&   SixelGraphics(bool b = true)                    { sixelimages = b; return *this; }
    Terminal&   NoSixelGraphics()                               { return SixelGraphics(false); }
    bool        HasSixelGraphics() const                        { return sixelimages; }

    Terminal&   JexerGraphics(bool b = true)                    { jexerimages = b; return *this; }
    Terminal&   NoJexerGraphics()                               { return JexerGraphics(false); }
    bool        HasJexerGraphics() const                        { return jexerimages; }

    Terminal&   iTerm2Graphics(bool b = true)                   { iterm2images = b; return *this; }
    Terminal&   NoiTerm2Graphics(bool b = true)                 { return iTerm2Graphics(false); }
    bool        HasiTerm2Graphics() const                       { return iterm2images; }

    Terminal&   Hyperlinks(bool b = true)                       { hyperlinks = b; return *this; }
    Terminal&   NoHyperlinks()                                  { return Hyperlinks(false);     }
    bool        HasHyperlinks() const                           { return hyperlinks; }

    Terminal&   ReverseWrap(bool b = true)                      { XTrewrapm((reversewrap = b)); return *this; }
    Terminal&   NoReverseWrap()                                 { return ReverseWrap(false); }
    bool        HasReverseWrap() const                          { return reversewrap; }

    Terminal&   DelayedRefresh(bool b = true)                   { delayedrefresh = b; return *this; }
    Terminal&   NoDelayedRefresh()                              { return DelayedRefresh(false); }
    bool        IsDelayingRefresh() const                       { return delayedrefresh; }

    Terminal&   LazyResize(bool b = true)                       { lazyresize = b; return *this; }
    Terminal&   NoLazyResize()                                  { return LazyResize(false);     }
    bool        IsLazyResizing() const                          { return lazyresize; }

    Terminal&   WindowOps(bool b = true)                        { windowactions = windowreports = b; return *this; }
    Terminal&   NoWindowOps()                                   { return WindowOps(false);      }
    bool        HasWindowOps() const                            { return windowactions || windowreports; }

    Terminal&   WindowReports(bool b = true)                    { windowreports = b; return *this; }
    Terminal&   NoWindowReports()                               { return WindowReports(false);  }
    bool        HasWindowReports() const                        { return windowreports; }

    Terminal&   WindowActions(bool b = true)                    { windowactions = b; return *this; }
    Terminal&   NoWindowActions()                               { return WindowActions(false);  }
    bool        HasWindowActions() const                        { return windowactions; }

    Terminal&   SetImageDisplay(const Display& d)               { imgdisplay = &d; return *this; }
    const Display& GetImageDisplay() const                      { return *imgdisplay; }

    Terminal&   UDK(bool b = true)                              { userdefinedkeys = b; return *this;  }
    Terminal&   NoUDK()                                         { return UDK(false);     }
    bool        HasUDK() const                                  { return userdefinedkeys; }
    Terminal&   LockUDK(bool b = true)                          { userdefinedkeyslocked = b;  return *this; }
    Terminal&   UnlockUDK()                                     { return LockUDK(false); }
    bool        IsUDKLocked() const                             { return userdefinedkeyslocked; }

    Size        GetFontSize() const;
    Size        GetPageSize() const;

    Size        PageSizeToClient(Size sz) const                 { return AddFrameSize(sz * GetFontSize()); }
    Size        PageSizeToClient(int col, int row) const        { return PageSizeToClient(Size(col, row)); }

    Size        GetMinSize() const override                     { return PageSizeToClient(Size(2, 2)); }
    Size        GetStdSize() const override                     { return PageSizeToClient(Size(80, 24)); }
    Size        GetMaxSize() const override                     { return PageSizeToClient(Size(132, 24)); }

    void        Copy()                                          { Copy(GetSelectedText()); }
    void        Copy(const WString& s);
    void        Paste()                                         { DragAndDrop(Null, Clipboard()); }
    void        Paste(const WString& s, bool filter = false);
    void        SelectAll(bool history = false);
    bool        IsSelection() const                             { return !IsNull(anchor) && anchor != selpos; }

    String      GetSelectionData(const String& fmt) const override;
    
    void        StdBar(Bar& menu);
    void        EditBar(Bar& menu);
    void        LinksBar(Bar& menu);
    void        ImagesBar(Bar& menu);
    void        OptionsBar(Bar& menu);

    void        Layout() override                               { SyncSize(true); SyncSb(); }

    void        Paint(Draw& w)  override                        { Paint0(w); }
    void        PaintPage(Draw& w)                              { Paint0(w, true); }

    bool         Key(dword key, int count) override;
    virtual bool VTKey(dword key, int count);
    virtual bool UDKey(dword key, int count);
    virtual bool NavKey(dword key, int count);

    Terminal&   MetaEscapesKeys(bool b = true)                  { metakeyflags = (metakeyflags & ~MKEY_ESCAPE) | (-b & MKEY_ESCAPE); return *this; }
    Terminal&   MetaShiftsKeys(bool b = true)                   { metakeyflags = (metakeyflags & ~MKEY_SHIFT)  | (-b & MKEY_SHIFT);  return *this; }
    Terminal&   MetaKeyDoesNothing()                            { modes.Set(XTALTESCM, false); metakeyflags = MKEY_NONE; return *this; }

    void        LeftDown(Point pt, dword keyflags) override;
    void        LeftUp(Point pt, dword keyflags) override;
    void        LeftDrag(Point pt, dword keyflags) override;
    void        LeftDouble(Point pt, dword keyflags) override;
    void        MiddleDown(Point pt, dword keyflags) override;
    void        MiddleUp(Point pt, dword keyflags) override;
    void        RightDown(Point pt, dword keyflags) override;
    void        RightUp(Point pt, dword keyflags) override;
    void        MouseMove(Point pt, dword keyflags) override;
    void        MouseWheel(Point pt, int zdelta, dword keyflags) override;
    Image       MouseEvent(int event, Point pt, int zdelta, dword keyflags) override;
    void        VTMouseEvent(Point pt, dword event, dword keyflags, int zdelta = 0);

    bool        IsMouseOverImage() const                        { Point pt = GetMouseViewPos(); return IsMouseOverImage(ClientToPagePos(pt)); }
    bool        IsMouseOverHyperlink() const                    { Point pt = GetMouseViewPos(); return IsMouseOverHyperlink(ClientToPagePos(pt)); }

    bool        IsTracking() const;

    const VTCell& GetCellAtMousePos() const                     { Point pt = GetMouseViewPos(); return page->FetchCell(ClientToPagePos(pt));; }

    String      GetHyperlinkUri()                               { return GetHyperlinkURI(mousepos, true); }
    Image       GetInlineImage()                                { return GetInlineImage(mousepos, true);  }

    void        DragAndDrop(Point pt, PasteClip& d) override;

    void        GotFocus() override                             { if(modes[XTFOCUSM]) PutCSI('I'); Refresh(); }
    void        LostFocus() override                            { if(modes[XTFOCUSM]) PutCSI('O'); Refresh(); }

    void        RefreshDisplay();

    Image       CursorImage(Point p, dword keyflags) override;

    void        AnswerBackMessage(const String& s)              { answerback = s; }

    void        State(int reason) override;

    void        Serialize(Stream& s) override;
    void        Jsonize(JsonIO& jio) override;
    void        Xmlize(XmlIO& xio) override;

    static void ClearImageCache();
    static void SetImageCacheMaxSize(int maxsize, int maxcount);

    static void ClearHyperlinkCache();
    static void SetHyperlinkCacheMaxSize(int maxcount);

private:
    void        PreParse()                                      { ScheduleDelayedRefresh(); }
    void        PostParse()                                     { if(delayedrefresh) return; SyncSb(); RefreshDisplay(); }

    void        SyncPage(bool notify = true);
    void        SwapPage();

    void        ScheduleDelayedRefresh();

    void        Blink(bool b);

    void        Scroll();
    void        SyncSb();

    void        SyncSize(bool notify = true);

    Tuple<String, Rect> GetSizeHint(Rect r, Size sz);
    void        RefreshSizeHint()                               { Refresh(GetSizeHint(GetView(), GetPageSize()).b.Inflated(8)); }

    Rect        GetCaretRect();
    void        PlaceCaret(bool scroll = false);

    int         GetSbPos() const                                { return IsAlternatePage() ? 0 : sb; }

    Point       GetCursorPos() const                            { return --page->GetPos(); /* VT cursor position is 1-based */ }

    Point       ClientToPagePos(Point pt) const;

    void        SetSelection(Point  l, Point h, bool rsel);
    bool        GetSelection(Point& l, Point& h) const;
    Rect        GetSelectionRect() const;
    void        ClearSelection();
    bool        IsSelected(Point pt) const;
    WString     GetSelectedText() const;

    bool        IsMouseOverImage(Point pt) const                { return !IsSelected(pt) && page->FetchCell(pt).IsImage(); }
    bool        IsMouseOverHyperlink(Point pt) const            { return !IsSelected(pt) && page->FetchCell(pt).IsHyperlink(); }

    void        HighlightHyperlink(Point pt);

    String      GetHyperlinkURI(Point pt, bool modifier);
    Image       GetInlineImage(Point pt, bool modifier);

private:
    using       ImagePart  = Tuple<dword, Point, Rect>;
    using       ImageParts = Vector<ImagePart>;

    struct ImageString : Moveable<ImageString> {
        String  data;
        Size    size;
        bool    encoded:1;
        bool    keepratio:1;
        dword   GetHashValue() const                            { return CombineHash(data, size, encoded, keepratio);   }
        void    SetNull()                                       { data = Null; size = Null; encoded = keepratio = true; }
        bool    IsNullInstance() const                          { return Upp::IsNull(data); }
        ImageString()                                           { SetNull(); }
        ImageString(const Nuller&)                              { SetNull(); }
        ImageString(String&& s)                                 { SetNull(); data = s;  }
    };

    struct InlineImageMaker : LRUCache<InlineImage>::Maker {
        dword   id;
        const   Size& fontsize;
        const   ImageString& imgs;
        String  Key() const override;
        int     Make(InlineImage& imagedata) const override;
        InlineImageMaker(int i, const ImageString& s, const Size& sz)
        : id(i)
        , imgs(s)
        , fontsize(sz)
        {
        }
    };

    struct HyperlinkMaker : LRUCache<String>::Maker {
        dword   id;
        const   String& url;
        String  Key() const override;
        int     Make(String& link) const override;
        HyperlinkMaker(int i, const String& s)
        : id(i)
        , url(s)
        {
        }
    };

    void        Paint0(Draw& w, bool print = false);
    void        AddImagePart(ImageParts& parts, int x, int y, const VTCell& cell, Size sz);
    void        PaintImages(Draw& w, ImageParts& parts, const Size& fsz);

    void        RenderImage(const ImageString& simg, bool scroll);
    const InlineImage& GetCachedImageData(dword id, const ImageString& simg, const Size& fsz);

    void        RenderHyperlink(const Value& uri);
    String      GetCachedHyperlink(dword id, const Value& data = Null);

private:
    enum ModifierKeyFlags : dword {
        MKEY_NONE   = 0,
        MKEY_ESCAPE = 1,
        MKEY_SHIFT  = 2
    };

    const Display *imgdisplay;
    VScrollBar  sb;
    Scroller    scroller;
    Point       mousepos;
    Font        font            = Monospace();
    byte        charset;
    Rect        caretrect;
    Point       anchor          = Null;
    Point       selpos          = Null;
    bool        rectsel         = false;
    bool        ignorescroll    = false;
    bool        mousehidden     = false;
    bool        resizing        = false;
    bool        hinting         = false;
    bool        blinking        = false;
    int         blinkinterval   = 500;
    int         wheelstep       = GUI_WheelScrollLines();
    int         metakeyflags    = MKEY_ESCAPE;
    dword       activelink      = 0;
    dword       prevlink        = 0;

    bool        eightbit;
    bool        reversewrap;
    bool        keynavigation;
    bool        legacycharsets;
    bool        alternatescroll;
    bool        userdefinedkeys;
    bool        userdefinedkeyslocked;
    bool        windowactions;
    bool        windowreports;
    bool        sixelimages;
    bool        jexerimages;
    bool        iterm2images;
    bool        hyperlinks;
    bool        delayedrefresh;
    bool        lazyresize;
    bool        sizehint;
    bool        nobackground;
    bool        intensify;
    bool        blinkingtext;
    bool        dynamiccolors;
    bool        adjustcolors;
    bool        lightcolors;
    bool        hidemousecursor;

// Down beloe is the emulator stuff, formerley knonw as "Console"...

private:
    VTPage*     page;

private:
    Color       GetColorFromIndex(const VTCell& cell, int which) const;
    void        SetInkAndPaperColor(const VTCell& cell, Color& ink, Color& paper);
    void        ReportANSIColor(int opcode, int index, const Color& c);
    void        ReportDynamicColor(int opcode, const Color& c);
    void        ChangeColors(int opcode, const String& oscs, bool reset);
    bool        SetColorTable(int opcode, int index, String colorspec, bool ansicolor, bool reset);
    bool        SetSaveColor(int index, const Color& c);
    bool        ResetLoadColor(int index);
    void        ParseExtendedColors(VTCell& attrs, const Vector<String>& opcodes, int& index);

    VectorMap<int, Color> savedcolors;
    Color       colortable[MAX_COLOR_COUNT];

    struct ColorTableSerializer {
        Color   *table;
        void    Serialize(Stream& s);
        void    Jsonize(JsonIO& jio);
        void    Xmlize(XmlIO& xio);
        ColorTableSerializer(Color *ct) : table(ct) {}
    };

private:
    void        PutChar(int c);
    int         LookupChar(int c);

    void        ParseControlChars(byte c);
    void        ParseEscapeSequences(const VTInStream::Sequence& seq);
    void        ParseCommandSequences(const VTInStream::Sequence& seq);
    void        ParseDeviceControlStrings(const VTInStream::Sequence& seq);
    void        ParseOperatingSystemCommands(const VTInStream::Sequence& seq);
    void        ParseApplicationProgrammingCommands(const VTInStream::Sequence& seq)    { WhenApplicationCommand(seq.payload); }

    bool        Convert7BitC1To8BitC1(const VTInStream::Sequence& seq);

    void        ClearPage(const VTInStream::Sequence& seq);
    void        ClearLine(const VTInStream::Sequence& seq);
    void        ClearTabs(const VTInStream::Sequence& seq);

    void        ReportMode(const VTInStream::Sequence& seq);
    void        ReportDeviceStatus(const VTInStream::Sequence& seq);
    void        ReportDeviceParameters(const VTInStream::Sequence& seq);
    void        ReportDeviceAttributes(const VTInStream::Sequence& seq);
    void        ReportControlFunctionSettings(const VTInStream::Sequence& seq);
    void        ReportRectAreaChecksum(const VTInStream::Sequence &seq);
    void        ReportPresentationState(const VTInStream::Sequence& seq);

    void        RestorePresentationState(const VTInStream::Sequence& seq);

    void        SelectGraphicsRendition(const VTInStream::Sequence& seq);
    void        SetGraphicsRendition(VTCell& attrs, const Vector<String>& opcodes);
    void        InvertGraphicsRendition(VTCell& attrs, const Vector<String>& opcodes);
    String      GetGraphicsRenditionOpcodes(const VTCell& attrs);

    void        ParseSixelGraphics(const VTInStream::Sequence& seq);
    void        ParseJexerGraphics(const VTInStream::Sequence& seq);
    void        ParseiTerm2Graphics(const VTInStream::Sequence& seq);

    void        ParseHyperlinks(const VTInStream::Sequence& seq);

    void        ProtectAttributes(bool protect);

    void        SetCaretStyle(const VTInStream::Sequence& seq);

    void        SetProgrammableLEDs(const VTInStream::Sequence& seq);

    void        SetDeviceConformanceLevel(const VTInStream::Sequence& seq);

    void        SetUserDefinedKeys(const VTInStream::Sequence& seq);

    void        CopyRectArea(const VTInStream::Sequence& seq);
    void        FillRectArea(const VTInStream::Sequence& seq);
    void        ClearRectArea(const VTInStream::Sequence& seq, bool selective = false);
    void        SelectRectAreaAttrsChangeExtent(const VTInStream::Sequence& seq);
    void        ChangeRectAreaAttrs(const VTInStream::Sequence& seq, bool invert);

    void        HandleWindowOpsRequests(const VTInStream::Sequence& seq);
    void        WindowMoveRequest(TopWindow *w, int x, int y);
    void        WindowResizeRequest(TopWindow *w, int cx, int cy);
    void        WindowPageResizeRequest(TopWindow *w, int cx, int cy);
    void        WindowMaximizeHorzRequest(TopWindow *w);
    void        WindowMaximizeVertRequest(TopWindow *w);

    void        SetColumns(int cols)                                { WhenSetSize(PageSizeToClient(Size(cols, page->GetSize().cy))); }
    void        SetRows(int rows)                                   { WhenSetSize(PageSizeToClient(Size(page->GetSize().cx, rows))); }

    dword       GetFillerFlags(const VTInStream::Sequence& seq) const;

    void        Backup(bool tpage = true, bool csets = true, bool attrs = true);
    void        Restore(bool tpage = true, bool csets = true, bool attrs = true);

    void        SetEmulation(int level, bool reset = true);

    void        Reset(bool full);

    void        AlternateScreenBuffer(bool b);

private:
    VTInStream  parser;
    VTPage      dpage;
    VTPage      apage;
    VTCell      cellattrs;
    VTCell      cellattrs_backup;
    String      out;
    String      answerback;
    byte        clevel;
    bool        streamfill:1;

private:
    const VTCell&   GetAttrs() const                            { return cellattrs;  }

    void        SetPageSize(Size sz)                            { page->SetSize(sz); }
    VTPage&     GetDefaultPage()                                { return dpage; }
    bool        IsDefaultPage() const                           { return page == &dpage; }
    VTPage&     GetAlternatePage()                              { return apage; }
    bool        IsAlternatePage() const                         { return page == &apage; }

    void        PutC(int c);
    void        PutC(const String& s1);
    void        Put(const String& s, int cnt = 1);
    void        Put(int c, int cnt = 1);
    void        PutUtf8(int c, int cnt = 1);
    void        PutRaw(const String& s, int cnt = 1);
    void        PutESC(const String& s, int cnt = 1);
    void        PutESC(int c, int cnt = 1);
    void        PutCSI(const String& s, int cnt = 1);
    void        PutCSI(int c, int cnt = 1);
    void        PutOSC(const String& s, int cnt = 1);
    void        PutOSC(int c, int cnt = 1);
    void        PutDCS(const String& s, int cnt = 1);
    void        PutDCS(int c, int cnt = 1);
    void        PutSS2(const String& s, int cnt = 1);
    void        PutSS2(int c, int cnt = 1);
    void        PutSS3(const String& s, int cnt = 1);
    void        PutSS3(int c, int cnt = 1);
    void        PutEncoded(const String& s, bool noctl = false);
    void        PutEncoded(const WString& s, bool noctl = false);
    void        PutEol();

    void        Flush();
    void        CancelOut()                                     { out.Clear(); }

    void        DisplayAlignmentTest();

private:
    bool        GetUDKString(byte key, String& val);

private:
    VectorMap<byte, String> udk;

private:
    Bits        modes;

private:
     // ANSI modes.
    void        ANSIkam(bool b);
    void        ANSIcrm(bool b);
    void        ANSIirm(bool b);
    void        ANSIsrm(bool b);
    void        ANSIlnm(bool b);
    void        ANSIerm(bool b);

    // DEC private modes.
    void        DECanm(bool b);
    void        DECarm(bool b);
    void        DECawm(bool b);
    void        DECbkm(bool b);
    void        DECckm(bool b);
    void        DECcolm(bool b);
    void        DECkpam(bool b);
    void        DEClrmm(bool b);
    void        DECom(bool b);
    void        DECsclm(bool b);
    void        DECscnm(bool b);
    void        DECsdm(bool b);
    void        DECtcem(bool b);

    // Private mode extensions.
    void        XTasbm(int mode, bool b);
    void        XTanymm(bool b);
    void        XTascm(bool b);
    void        XTbrpm(bool b);
    void        XTdragm(bool b);
    void        XTfocusm(bool b);
    void        XTaltkeym(bool b);
    void        XTrewrapm(bool b);
    void        XTsgrmm(bool b);
    void        XTsrcm(bool b);
    void        XTutf8mm(bool b);
    void        XTx10mm(bool b);
    void        XTx11mm(bool b);

    void        SetMode(const VTInStream::Sequence& seq, bool enable);
    static int  FindModeId(word modenum, byte modetype, byte level, bool& refresh);

public:
    // DEC and xterm style caret (cursor) support.

    class Caret {
        int       style;
        bool      blinking;
        bool      locked;
    public:
        enum : int
        {
            BLOCK = 0,
            BEAM,
            UNDERLINE
        };
        Event<> WhenAction;
        void    Set(int style_, bool blink);
        Caret&  Block(bool blink = true)                        { Set(BLOCK, blink); return *this; }
        Caret&  Beam(bool blink = true)                         { Set(BEAM, blink);  return *this; }
        Caret&  Underline(bool blink = true)                    { Set(UNDERLINE, blink); return *this; }
        Caret&  Blink(bool b = true)                            { if(!locked) { blinking = b; WhenAction(); }; return *this; }
        Caret&  Lock(bool b = true)                             { locked = b; return *this; }
        Caret&  Unlock()                                        { return Lock(false); }
        int     GetStyle() const                                { return style;    }
        bool    IsBlinking() const                              { return blinking; }
        bool    IsLocked() const                                { return locked;   }
        void    Serialize(Stream& s);
        void    Jsonize(JsonIO& jio);
        void    Xmlize(XmlIO& xio);
        Caret();
        Caret(int style, bool blink, bool lock);
    };

private:
    Caret       caret;

public:
    // Terminal legacy character sets ("G-set") support.
    class GSets {
        byte  g[4];
        byte  d[4];
        byte  ss;
        int   l, r;
    public:
        GSets&     G0toGL()                                     { l = 0; return *this; }
        GSets&     G1toGL()                                     { l = 1; return *this; }
        GSets&     G2toGL()                                     { l = 2; return *this; }
        GSets&     G3toGL()                                     { l = 3; return *this; }
        GSets&     G0toGR()                                     { r = 0; return *this; }
        GSets&     G1toGR()                                     { r = 1; return *this; }
        GSets&     G2toGR()                                     { r = 2; return *this; }
        GSets&     G3toGR()                                     { r = 3; return *this; }

        GSets&     G0(byte c)                                   { g[0] = c; return *this; }
        GSets&     G1(byte c)                                   { g[1] = c; return *this; }
        GSets&     G2(byte c)                                   { g[2] = c; return *this; }
        GSets&     G3(byte c)                                   { g[3] = c; return *this; }
        GSets&     SS(byte c)                                   { ss   = c; return *this; }

        byte        Get(int c, bool allowgr = true) const       { return allowgr && c > 0x7F ? g[r] : g[l]; }

        int         GetGLNum()                                  { return l; }
        int         GetGRNum()                                  { return r; }

        byte        GetGL() const                               { return g[l]; }
        byte        GetGR() const                               { return g[r]; }
        byte        GetG0() const                               { return g[0]; }
        byte        GetG1() const                               { return g[1]; }
        byte        GetG2() const                               { return g[2]; }
        byte        GetG3() const                               { return g[3]; }
        byte        GetSS() const                               { return ss;   }

        void        ConformtoANSILevel1();
        void        ConformtoANSILevel2();
        void        ConformtoANSILevel3();

        GSets&      ResetG0()                                   { g[0] = d[0]; return *this; }
        GSets&      ResetG1()                                   { g[1] = d[1]; return *this; }
        GSets&      ResetG2()                                   { g[2] = d[2]; return *this; }
        GSets&      ResetG3()                                   { g[3] = d[3]; return *this; }

        void        Reset();
        void        Serialize(Stream& s);
        void        Jsonize(JsonIO& jio);
        void        Xmlize(XmlIO& xio);

        GSets(byte defgset = CHARSET_ISO8859_1);
        GSets(byte g0, byte g1, byte g2, byte g3);
    };

    void            SetLegacyCharsets(GSets newgsets)           { gsets = newgsets;  }
    const GSets&    GetLegacyCharsets() const                   { return gsets;      }
    Terminal&       LegacyCharsets(bool b = true)               { legacycharsets = b; return *this; }
    Terminal&       NoLegacyCharsets()                          { return LegacyCharsets(false); }

private:
    int             ConvertToUnicode(int c, byte gset);
    int             ConvertToCharset(int c, byte gset);

private:
    GSets           gsets;
    GSets           gsets_backup;


    // Currently supported ANSI and private terminal modes.

    enum TerminalModes : byte
    {
        GATM = 0,
        KAM,
        CRM,
        IRM,
        SRTM,
        ERM,
        VEM,
        HEM,
        PUM,
        SRM,
        FEAM,
        FETM,
        MATM,
        TTM,
        SATM,
        TSM,
        EBM,
        LNM,
        DECANM,
        DECARM,
        DECAWM,
        DECBKM,
        DECCKM,
        DECCOLM,
        DECKPAM,
        DECLRMM,
        DECOM,
        DECSCLM,
        DECSCNM,
        DECSDM,
        DECTCEM,
        XTASBM,
        XTASCM,
        XTBRPM,
        XTDRAGM,
        XTANYMM,
        XTFOCUSM,
        XTALTESCM,
        XTREWRAPM,
        XTSPREG,
        XTSRCM,
        XTSGRMM,
        XTUTF8MM,
        XTX10MM,
        XTX11MM,
        XTSHOWSB,
        VTMODECOUNT
    };

    private:

    // Currently supported control bytes (C0, C1).

    enum class ControlId : byte
    {
        // C0
        NUL,
        ENQ,
        BEL,
        BS,
        HT,
        LF,
        VT,
        FF,
        CR,
        LS0,
        LS1,
        XON,
        XOFF,
        DEL,
        // C1
        IND,
        NEL,
        HTS,
        RI,
        SS2,
        SS3,
        SPA,
        EPA,
        DECID,
        ST,
        UNHANDLED
    };

    // Currently supported ESC, CSI, and DCS sequences.

    enum class SequenceId : byte
    {
        ANSICL1,
        ANSICL2,
        ANSICL3,
        CBT,
        CHA,
        CHT,
        CNL,
        CPL,
        CUB,
        CUD,
        CUF,
        CUP,
        CUU,
        DA1,
        DA2,
        DA3,
        DCH,
        DECALN,
        DECBI,
        DECCARA,
        DECDC,
        DECCRA,
        DECERA,
        DECFI,
        DECFRA,
        DECIC,
        DECKPAM,
        DECKPNM,
        DECLL,
        DECRARA,
        DECRC,
        DECRQM,
        DECRQSS,
        DECREQTPARM,
        DECRQCRA,
        DECRQPSR,
        DECRSPS,
        DECSACE,
        DECSC,
        DECSCA,
        DECSCL,
        DECSCPP,
        DECSCUSR,
        DECSGR,
        DECSED,
        DECSEL,
        DECSERA,
        DECSIXEL,
        DECSLPP,
        DECSLRM,
        DECSNLS,
        DECST8C,
        DECSTBM,
        DECSTR,
        DECTST,
        DECUDK,
        DECXCPR,
        DL,
        DSR,
        ECH,
        ED,
        EL,
        HOME,
        HPA,
        HPR,
        HVP,
        ICH,
        IL,
        LS1R,
        LS2,
        LS2R,
        LS3,
        LS3R,
        REP,
        RI,
        RIS,
        RM,
        S7C1T,
        S8C1T,
        SCODFK,
        SCORC,
        SCOSC,
        SCS_G0_DEC_DCS,
        SCS_G1_DEC_DCS,
        SCS_G2_DEC_DCS,
        SCS_G3_DEC_DCS,
        SCS_G0_DEC_ACS,
        SCS_G1_DEC_ACS,
        SCS_G0_ASCII,
        SCS_G1_ASCII,
        SCS_G2_ASCII,
        SCS_G3_ASCII,
        SCS_G1_LATIN1,
        SCS_G2_LATIN1,
        SCS_G3_LATIN1,
        SCS_G0_DEC_MCS,
        SCS_G1_DEC_MCS,
        SCS_G2_DEC_MCS,
        SCS_G3_DEC_MCS,
        SCS_G0_DEC_TCS,
        SCS_G1_DEC_TCS,
        SCS_G2_DEC_TCS,
        SCS_G3_DEC_TCS,
        SCS_UTF8,
        SCS_DEFAULT,
        SD,
        SGR,
        SL,
        SM,
        SR,
        SU,
        TBC,
        VPA,
        VPR,
        VT52_CUB,
        VT52_CUD,
        VT52_CUF,
        VT52_CUP,
        VT52_CUU,
        VT52_DA,
        VT52_DCS_ON,
        VT52_DCS_OFF,
        VT52_DECANM,
        VT52_ED,
        VT52_EL,
        VT52_HOME,
        VT52_RI,
        IGNORED,
        UNHANDLED
    };

    static ControlId  FindControlId(byte ctl, byte level, bool& refresh);
    static SequenceId FindSequenceId(byte type, byte level, const VTInStream::Sequence& seq, bool& refresh);
};

// Custom displays.

const Display& NormalImageCellDisplay();
const Display& ScaledImageCellDisplay();

// Color formatters and converters.

class ConvertHashColorSpec : public Convert {
public:
    ConvertHashColorSpec() {}
    int     Filter(int chr) const override;
    Value   Scan(const Value& text) const override;
    Value   Format(const Value& q) const override;
};

class ConvertRgbColorSpec : public Convert {
public:
    ConvertRgbColorSpec() {}
    int     Filter(int chr) const override;
    Value   Scan(const Value& text) const override;
    Value   Format(const Value& q) const override;
};

class ConvertCmykColorSpec : public Convert {
public:
    ConvertCmykColorSpec() {}
    int     Filter(int chr) const override;
    Value   Scan(const Value& text) const override;
    Value   Format(const Value& q) const override;
};

class ConvertColor : public Convert {
public:
    Value   Scan(const Value& text) const override;
    Value   Format(const Value& q) const override;
};

// Legacy charsets.

extern byte CHARSET_DEC_VT52;   // DEC VT52 graphics character set.
extern byte CHARSET_DEC_DCS;    // DEC VT100+ line drawing character set.
extern byte CHARSET_DEC_MCS;    // DEC VT200+ multinational character set.
extern byte CHARSET_DEC_TCS;    // DEC VT300+ technical character set.

INITIALIZE(DECGSets);
}
#endif
