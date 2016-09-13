@rem Set up version
@set MAJOR=2
@set MINOR=0
@set SUB=1
@set CONFIG=release
@echo Building version %MAJOR%.%MINOR%.%SUB% %CONFIG%
@
@echo = Set path to Qt build environment =
@set QTENV="C:\Qt\5.4\mingw491_32\bin\qtenv2.bat"
@if exist %QTENV% goto :qt_root_ok
@echo .
@echo Could not find %QTENV%
@echo .
@goto :end_fail
@
@rem Call Qt batch file 
:qt_root_ok
@pushd .
@call %QTENV%
@popd
@
@echo = Build ship apps =
@del /S /Q ..\bin
@pushd ..\src
@qmake ship-cmd.pro
@if errorlevel 1 goto :end_fail
@mingw32-make
@if errorlevel 1 goto :end_fail
@popd
@
@echo = Update Qt resources =
call update-qt-resources.bat %MAJOR% %MINOR% %SUB% %CONFIG%
@if errorlevel 1 goto :end_fail
@
@rem Update Qt project 
@..\bin\update-qt-project %CONFIG%
@if errorlevel 1 goto :end_fail
@
@echo = Delete shadow folder =
for /f %%i in ('dir /a:d /b ..\..\..\..\build-UniSim2*') do rmdir /s /q "..\..\..\..\%%i"
@
@echo = Delete bin target folder =
@del /S /Q ..\..\..\bin
@
@echo = Build UniSim =
@pushd ..\..\..
@qmake UniSim2.pro
@if errorlevel 1 goto :end_fail
@mingw32-make -B
@if errorlevel 1 goto :end_fail
@popd
@
@echo = Copy binary files =
@call fill-folders.bat %CONFIG%
@if errorlevel 1 goto :end_fail
@
@echo = Set installer version number =
@..\bin\update-installer-project %MAJOR% %MINOR% %SUB%
@if errorlevel 1 goto :end_fail
@
@echo = Generate installer =
@set SAVE_PATH=%PATH%
@set PATH=%PATH%;%INSTALL_BUILDER_PATH%
builder-cli build ..\..\unisim2_install_builder.xml"
@set PATH=%SAVE_PATH%

@echo *** SUCCESS ***
pause
exit
:end_fail
@echo *** BUILD FAILURE ***
pause