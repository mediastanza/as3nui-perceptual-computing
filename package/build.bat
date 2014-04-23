set ADT=C:\Program Files (x86)\Adobe\Adobe Flash Builder 4.6\sdks\4.6.0\bin\adt

set SWC=airPc.swc
set XML=extension.xml 

set WINDOWS_PLATFORM= -platform Windows-x86 -C .\Windows-x86\ .

set OUTPUT=./airPc.ane 

"%ADT%" -package -target ane "%OUTPUT%" "%XML%" -swc "%SWC%" %WINDOWS_PLATFORM%