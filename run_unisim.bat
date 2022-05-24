set cur_dir=%cd%
if exist "%QT_ROOT_32%\qtenv2.bat" (
  call %QT_ROOT_32%\qtenv2.bat
) else (
  call %QT_ROOT%\qtenv2.bat
)
if exist "%cur_dir%\bin\unisimd.exe" (
  "%cur_dir%\bin\unisimd.exe"
) else (
  "%cur_dir%\bin\unisim.exe"
)