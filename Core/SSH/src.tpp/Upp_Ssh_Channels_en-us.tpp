topic "Channels: Scp, Exec, Tunnel, Shell";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Standard Ssh Channels]]}}&]
[s3; &]
[s2; Secure shell protocol (version 2) predefines several channel 
types.&]
[s2;  &]
[s2;i150;O9; -|[^topic`:`/`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:Scp`:`:class^ 1
. Secure data copy channel.]&]
[s2;i150;O9; -|[^topic`:`/`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshExec`:`:class^ 2
. Remote command execution channel.]&]
[s2;i150;O9; -|[^topic`:`/`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshTunnel`:`:class^ 3
. TCP`-IP and port forwarding (network proxy) channel.]&]
[s2;i150;O9; -|[^topic`:`/`/SSH`/src`/Upp`_Ssh`_Channels`_en`-us`#Upp`:`:SshShell`:`:class^ 4
. Real`-time, interactive remote shell (command line interface) 
channel.]&]
[s2;i150;O9; &]
[s2; Below classes wrap the generic SshChannel class to provide easy`-to`-use 
interfaces to these specialized channels.&]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 Scp]]}}&]
[s3; &]
[s1;:Upp`:`:Scp`:`:class:%- [@(0.0.255)3 class][3 _][*3 Scp][3 _:_][@(0.0.255)3 public][3 _][*@3;3 S
shChannel]&]
[s0;#l288; This class encapsulates an SSH2 secure copy channel. It 
provides a means for securely transferring files between local 
host and a remote host. Scp class is derived from SshChannel 
class, and has pick semantics.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:Scp`:`:Get`(Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* Get]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:Scp`:`:operator`(`)`(Upp`:`:Stream`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* operator()]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=
_Null)&]
[s0;l288; Downloads the remote file at [%-*@3 path] to local [%-*@3 out] 
. Returns true on success. [%-*@3 progress ]function can be used 
to track the progress of the download; returning true will abort 
the operation.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:Get`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [_^Upp`:`:String^ S
tring]_[* Get]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=
_Null)&]
[s5;:Upp`:`:Scp`:`:operator`(`)`(const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [_^Upp`:`:String^ S
tring]_[* operator()]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path
], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_
`=_Null)&]
[s0;l288; Downloads the remote file at [%-*@3 path] and returns it 
as a String. Returns an empty string on failure. [%-*@3 progress] 
 function can be used to track the progress of the download; 
returning true will abort the operation.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:Put`(Upp`:`:Stream`&`,const Upp`:`:String`&`,long`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* Put]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 path], [@(0.0.255) long]_[*@3 mode], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:Scp`:`:Put`(const Upp`:`:String`&`,const Upp`:`:String`&`,long`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* Put]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 in], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) long]_[*@3 mode], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:Scp`:`:operator`(`)`(Upp`:`:Stream`&`,const Upp`:`:String`&`,long`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* operator()]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 in], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) long]_[*@3 mode], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s5;:Upp`:`:Scp`:`:operator`(`)`(const Upp`:`:String`&`,const Upp`:`:String`&`,long`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) b
ool]_[* operator()]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 in], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) long]_[*@3 mode], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], 
[_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s0;l288; Uploads the local data at [%-*@3 in] to remote [%-*@3 path] 
, with access [%-*@3 mode]. Returns true on success. [%-*@3 progress] 
function can be used to track the progress of the upload; returning 
true will abort the operation.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Multithreaded Transfer Methods]]}}&]
[s3;%- &]
[s5;:Upp`:`:Scp`:`:AsyncGet`(Upp`:`:SshSession`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[_^Upp`:`:String^ String]>_[* AsyncGet]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 path], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 p
rogress])&]
[s2; Downloads the remote file at [%-*@3 path] as string. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. [%-*@3 progress ]function can be used to 
track the progress of the download; returning true will abort 
the operation. Requires a valid ssh [%-*@3 session].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:AsyncGet`(Upp`:`:SshSession`&`,const char`*`,const char`*`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncGet]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 source
], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 target], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2; Overload. Downloads the remote file at [%-*@3 source] path to 
[%-*@3 target] (local) path. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. [%-*@3 progress ]function can be used to 
track the progress of the download; returning true will abort 
the operation. Requires a valid ssh [%-*@3 session].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:AsyncPut`(Upp`:`:SshSession`&`,Upp`:`:String`&`&`,const Upp`:`:String`&`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncPut]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:String^ String][@(0.0.255) `&`&]_[*@3 da
ta], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 target], 
[_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ int64], [_^Upp`:`:int64^ int64]>_[*@3 progress])&]
[s2; Uploads [%-*@3 data] to [%-*@3 target] (remote) path. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. [%-*@3 progress ]function can be used to 
track the progress of the upload; returning true will abort the 
operation. Requires a valid ssh [%-*@3 session].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Scp`:`:AsyncPut`(Upp`:`:SshSession`&`,const char`*`,const char`*`,Upp`:`:Gate`<Upp`:`:int64`,Upp`:`:int64`>`):%- [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncPut]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 source
], [@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 target], [_^Upp`:`:Gate^ Gate]<[_^Upp`:`:int64^ i
nt64], [_^Upp`:`:int64^ int64]>_[*@3 progress]_`=_Null)&]
[s2; Overload. Upload the local file at [%-*@3 source] path to [%-*@3 target] 
(remote) path. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. [%-*@3 progress ]function can be used to 
track the progress of the upload; returning true will abort the 
operation. Requires a valid ssh [%-*@3 session].&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:Scp`:`:Scp`(Upp`:`:SshSession`&`):%- [* Scp]([_^Upp`:`:SshSession^ SshSession
][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the Scp instance to [%-*@3 session].&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshExec]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshExec`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshExec][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 SshChannel]&]
[s0;#l288; This class encapsulates an SSH2 remote process execution 
(exec) channel. It provides a means for executing a single shell 
command on a remote host. If you need to run a real`-time, interactive 
command line interface, you should consider using SshShell class 
insead. SshExec class is derived from SshChannel class, and has 
pick semantics. &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExec`:`:Execute`(const Upp`:`:String`&`,Upp`:`:Stream`&`,Upp`:`:Stream`&`):%- [@(0.0.255) i
nt]_[* Execute]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&
]_[*@3 err])&]
[s5;:Upp`:`:SshExec`:`:operator`(`)`(const Upp`:`:String`&`,Upp`:`:Stream`&`,Upp`:`:Stream`&`):%- [@(0.0.255) i
nt]_[* operator()]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 cmd], 
[_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 out], [_^Upp`:`:Stream^ Stream][@(0.0.255) `&
]_[*@3 err])&]
[s0;l288; Executes remote process defined by [%-*@3 cmd] command line, 
returns its standard output in [%-*@3 out], its standard error 
output in [%-*@3 err], and its exit code as return value. In non`-blocking 
mode, SshChannel`::GetExitCode() and SshChannel`::GetExitSignal() 
methods can be used to retrieve the exit code and message of 
the executed command. &]
[s3;%- &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Multithreaded Execution Method]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExec`:`::%- [@(0.0.255) static] [_^Upp`:`:AsyncWork^ AsyncWork]<[_^Upp`:`:Tuple^ T
uple]<[@(0.0.255) int], [_^Upp`:`:String^ String], [_^Upp`:`:String^ String]>>_[* Async](
[_^Upp`:`:SshSession^ SshSession][@(0.0.255) `&]_[*@3 session], [@(0.0.255) const]_[_^Upp`:`:String^ S
tring][@(0.0.255) `&]_[*@3 cmd])&]
[s2; Executes a remote process defined by [%-*@3 cmd] command line 
asynchronously, and returns a tuple containing the exit code 
(int), standard output (String), and standard error output (String) 
of the executed process. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. Requires a valid ssh [%-*@3 session]. &]
[s3; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshExec`:`:SshExec`(Upp`:`:SshSession`&`):%- [* SshExec]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s0;l288; Constructor. Binds the SshExec instance to [%-*@3 session].&]
[s3;%- &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshTunnel]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshTunnel`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshTunnel][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 SshChannel]&]
[s2; This class encapsulates the SSH2 TCP/IP and port forwarding 
mechanisms. It provides a means for TCP tunneling from both client 
to server and server to client directions, acting as a secure 
network proxy. SshTunnel class is derived from SshChannel class, 
and has pick semantics.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Connect`(const Upp`:`:String`&`):%- [@(0.0.255) bool]_[* Connect
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 url])&]
[s5;:Upp`:`:SshTunnel`:`:Connect`(const Upp`:`:String`&`,int`):%- [@(0.0.255) bool]_[* Co
nnect]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2; Attempts a tunnelled TCP/IP connection to target [%-*@3 host] 
at [%-*@3 port]. Returns true on success. [%-*@3 url] must contain 
the target host name and port information. (E.g. `"[C localhost:1080`").] 
Please note that while the client to SSH server communication 
remains encrypted, communication from the SSH server to target 
host is not. &]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Listen`(int`,int`):%- [@(0.0.255) bool]_[* Listen]([@(0.0.255) int
]_[*@3 port], [@(0.0.255) int]_[*@3 listen`_count]_`=_[@3 5])&]
[s5;:Upp`:`:SshTunnel`:`:Listen`(const Upp`:`:String`&`,int`,int`*`,int`):%- [@(0.0.255) b
ool]_[* Listen]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port], [@(0.0.255) int`*]_[*@3 bound`_port], [@(0.0.255) int]_[*@3 list
en`_count]_`=_[@3 5])&]
[s2; Instructs the remote SSH server to begin listening for inbound 
TCP/IP connections at [%-*@3 port]. Returns true on success. If 
the listen port is set to 0, the SSH server will select the first 
available dynamic port. If a dynamic port is requested, [%-*@3 bound`_port] 
will be populated with the actual bound port assigned by the 
server. [%-*@3 host]  can be used to specify what interfaces to 
listen on. If host is not specified, or explicilty set to `"0.0.0.0`", 
all available addresses will be used to bind on. New connections 
will be queued until accepted using the Accept() method. [%-*@3 listen`_count] 
can be used to set the maximum number of pending connections 
to queue before rejecting further attempts.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshTunnel`:`:Accept`(Upp`:`:SshTunnel`&`):%- [@(0.0.255) bool]_[* Accept]([_^Upp`:`:SshTunnel^ S
shTunnel][@(0.0.255) `&]_[*@3 listener])&]
[s2; Accepts a connection from the [%-*@3 listener]. Returns true on 
success.&]
[s3; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshTunnel`:`:SshTunnel`(Upp`:`:SshSession`&`):%- [* SshTunnel]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the SshTunnel instance to [%-*@3 session].&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 SshShell]]}}&]
[s3;%- &]
[s1;:Upp`:`:SshShell`:`:class:%- [@(0.0.255)3 class][3 _][*3 SshShell][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 SshChannel]&]
[s2;# This class encapsulates an SSH2 interactive shell channel, 
providing a means for secure interaction with a remote command 
line interface in real`-time. SshShell class is derived from 
SshChannel class, and has pick semantics. If you need to execute 
a single command with no interaction, you should consider using 
SshExec class. &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshShell`:`:Run`(const Upp`:`:String`&`,Upp`:`:Size`):%- [@(0.0.255) bool]_
[* Run]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal], 
[_^Upp`:`:Size^ Size]_[*@3 pagesize])&]
[s5;:Upp`:`:SshShell`:`:Run`(const Upp`:`:String`&`,int`,int`):%- [@(0.0.255) bool]_[* Ru
n]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal], 
[@(0.0.255) int]_[*@3 width], [@(0.0.255) int]_[*@3 height])&]
[s2; Runs a generic remote command line interface. Returns true on 
successful exit. [%-*@3 terminal] should be set to preferred terminal 
emulation (ansi, vt100, xterm, etc.). The dimensions of the terminal 
view (as character cells) can be set using the [%-*@3 width ]and 
[%-*@3 height], or [%-*@3 pagesize] parameters.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:Console`(const Upp`:`:String`&`):%- [@(0.0.255) bool]_[* Console](
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 terminal])&]
[s6;%- Requires console`-based applications.&]
[s2; Runs a console`-based remote command line interface. Returns 
true on successful exit. [%-*@3 terminal] should be set to preferred 
terminal emulation (ansi, vt100, xterm, etc.). Note that in console 
mode SShShell automatically takes care of the local console page 
resizing.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:Send`(int`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) int]_[*@3 c])&]
[s5;:Upp`:`:SshShell`:`:Send`(const char`*`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) cons
t]_[@(0.0.255) char`*]_[*@3 s])&]
[s5;:Upp`:`:SshShell`:`:Send`(const Upp`:`:String`&`):%- [@(0.0.255) void]_[* Send]([@(0.0.255) c
onst]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Puts a single character, or multiple characters, into the input 
queue.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:PageSize`(Upp`:`:Size`):%- [_^Upp`:`:SshShell^ SshShell][@(0.0.255) `&
]_[* PageSize]([_^Upp`:`:Size^ Size]_[*@3 sz])&]
[s2; Sets the terminal view size as character cells to [%-*@3 sz]. 
In console mode it also sets the local console`'s page size.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:GetPageSize`(`)const:%- [_^Upp`:`:Size^ Size]_[* GetPageSize]()_[@(0.0.255) c
onst]&]
[s2; Returns the current SSH terminal view size in character cells.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:GetConsolePageSize`(`):%- [_^Upp`:`:Size^ Size]_[* GetConsolePage
Size]()&]
[s2; This helper method returns the actual page size of the local 
console, and is meant to be used in console mode. Returns Null 
on failure.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:WhenInput:%- [_^Upp`:`:Event^ Event]<>_[* WhenInput]&]
[s2; This event is emitted when writing to shell channel becomes 
possible. Send() method can be used to send data over the channel.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SshShell`:`:WhenOutput:%- [_^Upp`:`:Event^ Event]<[@(0.0.255) const]_[@(0.0.255) v
oid`*], [@(0.0.255) int]>_[* WhenOutput]&]
[s2; This event is emitted whenever data is read from the shell.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Multithreaded Method]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshShell`:`:AsyncRun`(Upp`:`:SshSession`&`,Upp`:`:String`,Upp`:`:Size`,Upp`:`:Event`<Upp`:`:SshShell`&`>`,Upp`:`:Event`<const Upp`:`:String`&`>`):%- [@(0.0.255) s
tatic] [_^Upp`:`:AsyncWork^ AsyncWork]<[@(0.0.255) void]>_[* AsyncRun]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session], [_^Upp`:`:String^ String]_[*@3 terminal], 
[_^Upp`:`:Size^ Size]_[*@3 pagesize], [_^Upp`:`:Event^ Event]<SshShell[@(0.0.255) `&]>_[*@3 i
n], [_^Upp`:`:Event^ Event]<[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]>_
[*@3 out])&]
[s2; Runs a generic remote command line interface asynchronously. 
[%-*@3 terminal] should be set to preferred terminal emulation 
(ansi, vt100, xterm, etc.). The dimensions of the terminal view 
(as character cells) can be set using the [%-*@3 pagesize] parameter. 
[%-*@3 in] event should be used to send data. [%-*@3 out] event should 
be used to receive data. Throws [^topic`:`/`/SSH`/src`/Upp`_Ssh`_Base`$en`-us`#Upp`:`:Ssh`:`:Error`:`:struct^ S
sh`::Error] on failure. Requires a valid ssh [%-*@3 session].&]
[s3;%- &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:SshShell`:`:SshShell`(Upp`:`:SshSession`&`):%- [* SshShell]([_^Upp`:`:SshSession^ S
shSession][@(0.0.255) `&]_[*@3 session])&]
[s2; Constructor. Binds the SshShell instance to [%-*@3 session].&]
[s3; &]
[s3; &]
[s0; ]]