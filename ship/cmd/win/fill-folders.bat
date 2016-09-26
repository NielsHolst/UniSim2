set UNISIM_SHIP=C:\data\QDev\UniSim2\ship
set QT_PLUGINS=C:\Qt\5.4\mingw491_32\plugins
set UNISIM_BIN=%UNISIM_SHIP%\bin

echo = Clean target bin and  =
rd /Q /S %UNISIM_BIN%
md %UNISIM_BIN%
md %UNISIM_BIN%\plugins

echo = Copy MS files =
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%
copy comctl32.dll %UNISIM_BIN%
copy mfc42u.dll %UNISIM_BIN%
copy msvcrt.dll %UNISIM_BIN%
copy oleaut32.dll %UNISIM_BIN%
popd

echo = Copy exe and DLL files =
copy %UNISIM_SHIP%\..\bin\unisim.exe %UNISIM_BIN%
copy %UNISIM_SHIP%\..\bin\base.dll %UNISIM_BIN%
copy %UNISIM_SHIP%\..\bin\plugins\*.dll %UNISIM_BIN%\plugins

echo = Copy MinGW libraries =
pushd %QT_ROOT%
copy icu*.dll %UNISIM_BIN%\
copy libwinpthread-1.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy libstd*.dll %UNISIM_BIN%

echo = Copy Qt libraries =
copy qt5core.dll %UNISIM_BIN%
copy qt5gui.dll %UNISIM_BIN%
copy Qt5Network.dll %UNISIM_BIN%
copy Qt5PrintSupport.dll %UNISIM_BIN%
copy qt5test.dll %UNISIM_BIN%
copy Qt5Widgets.dll %UNISIM_BIN%
copy qt5xmlpatterns.dll %UNISIM_BIN%
popd

echo = Create Qt plugin folders =
pushd %UNISIM_BIN%
md imageformats
md platforms
popd

echo = Copy Qt plugin folders =
pushd %QT_PLUGINS%
copy imageformats %UNISIM_BIN%\imageformats
copy platforms %UNISIM_BIN%\platforms
popd

rem Success
exit /B 0

:end_fail
@echo *** BUILD FAILURE ***
exit /B 1

