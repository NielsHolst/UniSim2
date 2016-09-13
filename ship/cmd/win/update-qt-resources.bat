rmdir /S /Q ..\..\..\src\resources\data
mkdir ..\..\..\src\resources\data\input\book
mkdir ..\..\..\src\resources\data\script
xcopy /S ..\..\..\input\book ..\..\..\src\resources\data\input\book
xcopy /S ..\..\..\script ..\..\..\src\resources\data\script
..\bin\update-qt-resources %1 %2 %3 %4
