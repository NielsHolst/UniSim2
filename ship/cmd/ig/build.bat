REM pushd ..\win
REM call build
REM popd

set VERSION=release

echo = Set release version =
..\bin\update-qt-project %VERSION%
if errorlevel 1 goto :end_fail

echo = Set path to Qt build environment =
set QTENV="C:\Qt\5.9.2\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt

set QTENV="C:\Qt\5.7\mingw53_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt

set QTENV="C:\Qt\5.4\mingw491_32\bin\qtenv2.bat"
if exist %QTENV% goto :ok_qt
echo .
echo Could not find %QTENV%
echo .
goto :end_fail
:ok_qt 
rem Call Qt batch file 
pushd .
call %QTENV%
popd

pause

echo = Build ig =
pushd ..\..\..
qmake ig.pro
if errorlevel 1 goto :end_fail
mingw32-make -B
if errorlevel 1 goto :end_fail
popd

echo = Restore to debug version =
..\bin\update-qt-project debug

echo = Copy ig exe and dll =
copy ..\..\..\bin\ig*.* ..\..\bin
copy ..\..\..\bin\libig*.* ..\..\bin

echo = Delete unused binary files =
pushd ..\..\bin
del /q unisim.exe
cd plugins
for %%i in (*.*) do if not "%%i"=="boxes.dll" if not "%%i"=="vg.dll" del /q "%%i"
cd ..
popd

echo = Replace zip files
del /q *.zip
7z a -r -tzip bin ..\..\bin\*.*

rmdir /s /q src
mkdir src
copy ..\..\..\src\apps\igclient\main.cpp src
copy ..\..\..\src\lib\iglib\iglib.h src

echo = Replace zip files in destination folder =
rmdir /s /q ..\..\ig
mkdir ..\..\ig\src
copy *.zip ..\..\ig
copy src ..\..\ig\src

echo *** SUCCESS ***
pause
exit
:end_fail
echo *** BUILD FAILURE ***
echo on
pause
