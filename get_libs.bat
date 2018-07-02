
@echo off

set srcpath=..\oce\Build\bin
set dstpath1=bin
set dstpath2=Libs


set subdir=Release
set deb=

:next
  

for /F %%i in (get_libs.bls) do (

  echo f | xcopy %srcpath%\%subdir%\%%i%deb%.dll %dstpath1%\%subdir%\%%i%deb%.dll /Y > nul
  echo f | xcopy %srcpath%\%subdir%\%%i%deb%.lib %dstpath2%\%subdir%\%%i.lib /Y > nul
)


if [%subdir%]==[Debug] goto exit

set subdir=Debug
set deb=d

goto next


:exit


