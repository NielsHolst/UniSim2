echo on
rem Set up version
set MAJOR=2
set MINOR=0
set SUB=11
echo Building version %MAJOR%.%MINOR%.%SUB% release

echo = Set path to Qt build environment =
set QTENV="C:\Qt\5.7\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :qt_root_ok
set QTENV="C:\Qt\5.4\mingw491_32\bin\qtenv2.bat"
if exist %QTENV% goto :qt_root_ok
echo .
echo Could not find %QTENV%
echo .
goto :end_fail

:qt_root_ok
rem Call Qt batch file 
pushd .
call %QTENV%
popd

echo = Clean out old builds =
pushd ..\..\..
dir
call clean
popd

REM echo = Build ship apps =
REM del /S /Q ..\bin
REM pushd ..\src
REM qmake ship-cmd.pro
REM if errorlevel 1 goto :end_fail
REM mingw32-make
REM if errorlevel 1 goto :end_fail
REM popd
pause

echo = Update Qt resources =
call update-qt-resources.bat 
if errorlevel 1 goto :end_fail
..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% release


rem Update Qt project 
..\bin\update-qt-project release
if errorlevel 1 goto :end_fail
pause

echo = Delete shadow folder =
for /f %%i in ('dir /a:d /b ..\..\..\..\build-UniSim2*') do rmdir /s /q "..\..\..\..\%%i"

echo = Delete bin target folder =
del /S /Q ..\..\..\bin

echo = Build UniSim =
pushd ..\..\..
qmake UniSim2.pro
if errorlevel 1 goto :end_fail
mingw32-make -B
if errorlevel 1 goto :end_fail
popd

echo = Copy binary files =
call fill-folders.bat 
if errorlevel 1 goto :end_fail

echo = Set installer version number =
..\bin\update-installer-project %MAJOR% %MINOR% %SUB%
if errorlevel 1 goto :end_fail
echo = Build installer =
pushd "C:\Program Files\BitRock InstallBuilder for Qt 16.7.0\bin
builder build \data\QDev\UniSim2\ship\unisim2_install_builder.xml
popd

echo = Restore to debug version =
call update-qt-resources.bat %MAJOR% %MINOR% %SUB% debug
..\bin\update-qt-resources %MAJOR% %MINOR% %SUB% debug
..\bin\update-qt-project debug

echo *** SUCCESS ***
pause
exit
:end_fail
echo *** BUILD FAILURE ***
echo on
pause
