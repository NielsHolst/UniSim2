echo on
cd C:\Users\au152367\Documents\QDev\UniSim2\ship\cmd\win

rem Set up version
set MAJOR=2
set MINOR=3
set SUB=32
set VERSION=release
echo Building version %MAJOR%.%MINOR%.%SUB% %VERSION%

echo = Set path to Qt build environment =

set QTENV="C:\Qt\5.15.1\mingw81_64\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
set QTENV="C:\Qt\Qt5.11.2\5.11.2\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
set QTENV="C:\Qt\5.11.2\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
set QTENV="C:\Qt\5.12.1\mingw73_64\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
set QTENV="C:\Qt\5.9.2\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
C:\Qt\5.9.2\mingw53_32
echo .
echo Could not find %QTENV%
echo .
goto :end_fail

:ok_qt 
echo = Set path to InstallBuilder =
set INSTALL_BUILDER_PATH="C:\Program Files (x86)\BitRock InstallBuilder for Qt 17.7.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder

set INSTALL_BUILDER_PATH="C:\Program Files\BitRock InstallBuilder for Qt 16.7.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder

set INSTALL_BUILDER_PATH="C:\Program Files (x86)\BitRock InstallBuilder for Qt 16.8.0\bin"
if exist %INSTALL_BUILDER_PATH% goto :ok_builder
echo .
echo Could not find %INSTALL_BUILDER_PATH%
echo .
REM goto :end_fail until InstallBuilder is installed!!!!
:ok_builder

rem Call Qt batch file 
pushd .
call %QTENV%
popd

echo = Clean out old builds =
pushd ..\..\..
call clean
popd

echo = Build ship apps =
del /S /Q ..\bin
pushd ..\src
qmake ship-cmd.pro
if errorlevel 1 goto :end_fail
mingw32-make
if errorlevel 1 goto :end_fail
popd

echo = Update Qt resources =
call update-qt-resources.bat 
if errorlevel 1 goto :end_fail
..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% %VERSION%

echo = Update Qt project  =
..\bin\update-qt-project %MAJOR% %MINOR% %SUB% %VERSION%
if errorlevel 1 goto :end_fail

echo = Build Universal Simulator =
pushd ..\..\..
qmake UniSim2.pro
if errorlevel 1 goto :end_fail
mingw32-make -B
if errorlevel 1 goto :end_fail
popd

echo = Copy binary files =
call fill-folders
if errorlevel 1 goto :end_fail

echo = Set installer version number =
..\bin\update-installer-project %MAJOR% %MINOR% %SUB%
if errorlevel 1 goto :end_fail

echo = Build installer =
pushd %INSTALL_BUILDER_PATH%
builder build \data\QDev\UniSim2\ship\unisim2_install_builder.xml
popd

echo = Restore to debug version =
..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% debug
..\bin\update-qt-project %MAJOR% %MINOR% %SUB% debug

echo *** SUCCESS ***
pause
exit
:end_fail
echo *** BUILD FAILURE ***
echo on
pause
