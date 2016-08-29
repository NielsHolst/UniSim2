set SUFFIX=d
set UNISIM_SHIP=C:\data\QDev\UniSim2\ship
set QT_PLUGINS="C:\Qt\5.4\mingw491_32\plugins"

rem Clean target bin and set target bin variable
rd /Q /S bin
md bin
md bin\plugins
set UNISIM_BIN=%UNISIM_SHIP%\bin

rem Copy icon file
copy *.ico bin

rem Copy MS files
pushd \Windows\System32
copy atl.dll %UNISIM_BIN%
copy comctl32.dll %UNISIM_BIN%
copy mfc42u.dll %UNISIM_BIN%
copy msvcrt.dll %UNISIM_BIN%
copy oleaut32.dll %UNISIM_BIN%
popd

rem Copy exe and DLL files
copy ..\bin\*%SUFFIX%.exe bin
copy ..\bin\*%SUFFIX%.dll bin
copy ..\bin\plugins\*%SUFFIX%.dll bin\plugins

rem copy MinGW libraries
pushd %QT_ROOT%
copy icu*.dll %UNISIM_BIN%\
copy libwinpthread-1.dll %UNISIM_BIN%
copy libgcc_s_dw2-1.dll %UNISIM_BIN%
copy libstd*.dll %UNISIM_BIN%

rem copy Qt libraries
copy qt5core%SUFFIX%.dll %UNISIM_BIN%
copy qt5gui%SUFFIX%.dll %UNISIM_BIN%
copy Qt5Network%SUFFIX%.dll %UNISIM_BIN%
copy Qt5PrintSupport%SUFFIX%.dll %UNISIM_BIN%
copy qt5test%SUFFIX%.dll %UNISIM_BIN%
copy Qt5Widgets%SUFFIX%.dll %UNISIM_BIN%
copy qt5xmlpatterns%SUFFIX%.dll %UNISIM_BIN%
popd

rem create Qt plugin folders
pushd %UNISIM_BIN%
md imageformats
md platforms
popd

rem copy Qt plugin folders
pushd %QT_PLUGINS%
copy imageformats %UNISIM_BIN%\imageformats
copy platforms %UNISIM_BIN%\platforms
popd

rem Remove test modules
pushd %UNISIM_BIN%
del /Q test*.*
del /Q plugins\test*.*
popd

rem Copy input folder
rd /Q /S input
md input
md input\book
xcopy ..\input\book\*.* input\book /S

rem Copy script folder
rd /Q /S script
md script
xcopy ..\script\*.* script /S

pause