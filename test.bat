rem for /f %%i in ('dir /S /B /A:D release') do dir "%%i"
rem for /f %%i in ('dir /S /B /A:D release') do rmdir /S /Q "%%i"
for /f %%i in ('dir /S /B .\src\plugins\*') do dir "%%i"

