@echo off

set mingw=e:\tools\mingw32-4.9.0
path=%mingw%\bin
set app_name=stardict_to_dsl

set dict_path=e:\latin-english.ifo

if exist %app_name%.exe (
  del %app_name%.exe
)


if "%1"=="c" (
  make clean
  goto:eof
)

echo == Building App ==
make

if not "%1"=="b" (
if exist %app_name%.exe (
  echo.
  echo == Calling App ==
  echo '%app_name%.exe %dict_path%'
  echo.
  
  %app_name%.exe "%dict_path%"
)
)
