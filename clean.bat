for /f %%i in ('dir /S /B src\plugins\*.rc') do del "%%i"
for /f %%i in ('dir /S /B *.o') do del "%%i"
for /f %%i in ('dir /S /B object_script.*.*') do del "%%i"
for /f %%i in ('dir /S /B Makefile.*') do del "%%i"
for /f %%i in ('dir /S /B UniversalSimulator*-installer.exe') do del "%%i"
for /f %%i in ('dir /S /B /AD debug') do rmdir /S /Q "%%i"
for /f %%i in ('dir /S /B /AD release') do rmdir /S /Q "%%i"
for /f %%i in ('dir /S /B ..\build-*-Desktop*') do rmdir /S /Q "%%i"
rmdir /S /Q output

rmdir /S /Q bin
for /f %%i in ('dir /S /B /AD ship\cmd\ig\iglib*') do rmdir /S /Q "%%i"
del ship\cmd\ig\*.zip

copy UniSim2.pro CopyUniSim2.pro
del UniSim2.pro.*
del UniSim2.pro.*.*
ren CopyUniSim2.pro UniSim2.pro

copy ig.pro copy_ig.pro
del ig.pro.*
del ig.pro.*.*
ren copy_ig.pro ig.pro

copy test.pro copy_test.pro
del test.pro.*
del test.pro.*.*
ren copy_test.pro test.pro
