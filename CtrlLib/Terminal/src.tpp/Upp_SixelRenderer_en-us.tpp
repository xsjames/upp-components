topic "SixelRaster";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 SixelRenderer]]}}&]
[s0; &]
[s1;:Upp`:`:SixelRenderer`:`:class: [@(0.0.255)3 class][3 _][*3 SixelRenderer][3 _:_][@(0.0.255)3 p
rivate][3 _][*@3;3 NoCopy]&]
[s2;%% This helper class renders [^https`:`/`/en`.wikipedia`.org`/wiki`/Sixel`?oldformat`=true^ s
ixel] images. It can handle both RGB and HSL color spaces, and 
has true color support.&]
[s2;%% Shared palette is not supported.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SixelRenderer`:`:Get`(`): [_^Upp`:`:Image^ Image]_[* Get]()&]
[s5;:Upp`:`:SixelRenderer`:`:operator Image`(`): [* operator_Image]()&]
[s2;%% Processes the sixel data and returns the result as an [^topic`:`/`/Draw`/src`/Image`_en`-us`#Image`:`:class^ I
mage] object.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SixelRenderer`:`:GetSize`(`)const: [_^Upp`:`:Size^ Size]_[* GetSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the size of the image.&]
[s3;%% &]
[s4;@(0.0.255) &]
[s5;:Upp`:`:SixelRenderer`:`:GetRatio`(`)const: [@(0.0.255) int]_[* GetRatio]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the aspect ratio of the image. &]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SixelRenderer`:`:SixelRenderer`(Upp`:`:Stream`&`): [* SixelRenderer]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Constructs a sixel renderer object from [%-*@3 data] .&]
[s3;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 SixelRaster]]}}&]
[s0; &]
[s1;:Upp`:`:SixelRaster`:`:class: [@(0.0.255)3 class][3 _][*3 SixelRaster][3 _:_][@(0.0.255)3 pu
blic][3 _][*@3;3 StreamRaster]&]
[s2;%% [^topic`:`/`/Draw`/src`/Rasters`_en`-us`#StreamRaster`:`:class^ StreamRaster] 
interface for SixelRenderer.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SixelRaster`:`:SixelRaster`(`): [* SixelRaster]()&]
[s2;%% Default constructor.&]
[s3; &]
[s3; ]]