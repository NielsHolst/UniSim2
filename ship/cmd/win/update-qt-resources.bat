rmdir /S /Q ..\..\..\src\resources\data
mkdir ..\..\..\src\resources\data\input
xcopy /S /Q /I ..\..\..\input\book ..\..\..\src\resources\data\input\book
xcopy /S /Q /I ..\..\..\input\papers ..\..\..\src\resources\data\input\papers
xcopy /S /Q /I ..\..\..\input\scripts ..\..\..\src\resources\data\input\scripts
xcopy /S /Q /I ..\..\..\input\teaching ..\..\..\src\resources\data\input\teaching

