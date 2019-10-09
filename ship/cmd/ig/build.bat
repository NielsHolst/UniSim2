set IGLIB=iglib_2.3.11 
set IGLIB_BIN=iglib_2.3.11\bin 
set IGLIB_BIN_PLUGINS=iglib_2.3.11\bin\plugins 
set IGLIB_BIN_IMAGEFORMATS=iglib_2.3.11\bin\imageformats
set IGLIB_BIN_PLATFORMS=iglib_2.3.11\bin\platforms
mkdir %IGLIB%
pushd %IGLIB%
mkdir bin
pushd bin
mkdir imageformats
mkdir platforms
mkdir plugins
popd
popd

echo = Copy UniSim plug-ins =
copy ..\..\..\bin\plugins\*.dll %IGLIB_BIN_PLUGINS%

echo = Copy Qt library folders =
copy C:\Qt\5.9.2\mingw53_32\plugins\imageformats\*.* %IGLIB_BIN_IMAGEFORMATS%
copy C:\Qt\5.9.2\mingw53_32\plugins\platforms\*.* %IGLIB_BIN_PLATFORMS%

echo = Copy UniSim libraries =
copy ..\..\..\bin\iglib.dll %IGLIB_BIN%
copy ..\..\..\bin\universal_simulator_base.dll %IGLIB_BIN%

echo = Copy Qt libraries =
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5Core.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5Gui.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5Network.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5PrintSupport.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5Test.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5Widgets.dll %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\Qt5XmlPatterns.dll %IGLIB_BIN%

echo = Copy C++ libraries =
copy C:\Qt\5.9.2\mingw53_32\bin\libgcc_s_dw2-1.dll %IGLIB_BIN%
copy "C:\Qt\5.9.2\mingw53_32\bin\libstdc++-6.dll" %IGLIB_BIN%
copy C:\Qt\5.9.2\mingw53_32\bin\libwinpthread-1.dll %IGLIB_BIN%

echo = Copy Windows libraries
copy C:\Windows\System32\atl.dll %IGLIB_BIN%
copy C:\Windows\System32\comctl32.dll %IGLIB_BIN%
copy C:\Windows\System32\mfc42u.dll %IGLIB_BIN%
copy C:\Windows\System32\msvcrt.dll %IGLIB_BIN%
copy C:\Windows\System32\oleaut32.dll %IGLIB_BIN%

pause