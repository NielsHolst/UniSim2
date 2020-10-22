echo on
cd C:\Users\au152367\Documents\QDev\UniSim2\ship\cmd\ig

set VERSION=2.3.30
REM set QT_PATH=C:\Qt\Qt5.11.2\5.11.2\mingw53_32
set QT_PATH=C:\Qt\5.9.2\mingw53_32
set PATH=C:\Users\au152367\Documents\QDev\UniSim2\ship\cmd\ig\

set IGLIB_BIN=iglib\bin
set IGLIB_BIN_IMAGEFORMATS=iglib\bin\imageformats
set IGLIB_BIN_PLATFORMS=iglib\bin\platforms
set IGLIB_BIN_PLUGINS=iglib\bin\plugins

set IGLIB_BIN=%PATH%%IGLIB_BIN%
set IGLIB_BIN_IMAGEFORMATS=%PATH%%IGLIB_BIN_IMAGEFORMATS%
set IGLIB_BIN_PLATFORMS=%PATH%%IGLIB_BIN_PLATFORMS%
set IGLIB_BIN_PLUGINS=%PATH%%IGLIB_BIN_PLUGINS%

rmdir /S /Q iglib
mkdir iglib
pushd iglib
mkdir bin
pushd bin
mkdir imageformats
mkdir platforms
mkdir plugins
popd
popd

echo = Copy iglib.h =
copy ..\..\..\src\lib\iglib\iglib.h iglib

echo = Copy UniSim plug-ins =
copy ..\..\..\bin\plugins\*.dll %IGLIB_BIN_PLUGINS%

echo = Copy UniSim libraries =
copy ..\..\..\bin\iglib.dll %IGLIB_BIN%
copy ..\..\..\bin\libiglib.a %IGLIB_BIN%
copy ..\..\..\bin\universal_simulator_base.dll %IGLIB_BIN%

echo = Copy Qt library folders =
pushd %QT_PATH%
pause

copy plugins\imageformats\*.* %IGLIB_BIN_IMAGEFORMATS%
copy plugins\platforms\*.* %IGLIB_BIN_PLATFORMS%

echo = Copy Qt libraries =
copy bin\Qt5Core.dll %IGLIB_BIN%
copy bin\Qt5Gui.dll %IGLIB_BIN%
copy bin\Qt5Network.dll %IGLIB_BIN%
copy bin\Qt5PrintSupport.dll %IGLIB_BIN%
copy bin\Qt5Test.dll %IGLIB_BIN%
copy bin\Qt5Widgets.dll %IGLIB_BIN%
copy bin\Qt5XmlPatterns.dll %IGLIB_BIN%

echo = Copy C++ libraries =
copy bin\libgcc_s_dw2-1.dll %IGLIB_BIN%
copy "bin\libstdc++-6.dll" %IGLIB_BIN%
copy bin\libwinpthread-1.dll %IGLIB_BIN%
popd

echo = Copy Windows libraries
pushd C:\Windows\System32
copy atl.dll %IGLIB_BIN%
copy comctl32.dll %IGLIB_BIN%
copy mfc42u.dll %IGLIB_BIN%
copy msvcrt.dll %IGLIB_BIN%
copy oleaut32.dll %IGLIB_BIN%
popd

set A=iglib_
set DIRNAME=%A%%VERSION%
ren iglib %DIRNAME%
pause