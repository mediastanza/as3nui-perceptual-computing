Intructions for building ANE package with adl in Windows:

1. Build native DLL
a. go to \native\win-visualstudio\PcExtension\
b. open PcExtension.sln and build in release

2. Build as3
a. open flash builder(4.6) project in \as3\
b. build it

3. launch cmd
a. run copy.bat
b. extract library.swf from airPc.swc, into \Windows-x86\   // can use extraction tool like 7z
c. run build.bat
d. if everything goes well, you will get airPc.ane


