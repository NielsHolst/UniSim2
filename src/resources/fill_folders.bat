rd /Q /S input
md input
md input\book\weather
md input\book\week01

copy ..\..\input\book\weather input\book\weather
copy ..\..\input\book\week01 input\book\week01

rd /Q /S script
md script
copy ..\..\script script

pause
