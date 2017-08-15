for /f %%i in ('dir /S /B *.o') do del "%%i"
for /f %%i in ('dir /S /B object_script.*.*') do del "%%i"
for /f %%i in ('dir /S /B Makefile.*') do del "%%i"
for /f %%i in ('dir /S /B UniversalSimulator*-installer.exe') do del "%%i"
for /f %%i in ('dir /S /B /A:D debug') do rmdir /S /Q "%%i"
for /f %%i in ('dir /S /B /A:D release') do rmdir /S /Q "%%i"
for /f %%i in ('dir /S /B ..\build-*-Desktop*') do rmdir /S /Q "%%i"
rmdir /S /Q output
rmdir /S /Q bin
rmdir /S /Q ship\bin
rmdir /S /Q ship\cmd\ig\bin
