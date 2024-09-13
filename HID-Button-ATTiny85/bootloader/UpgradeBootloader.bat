@echo off
:menu
title Micronucleus Upgrade Menu
cd "%~dp0"
color 1F
cls
echo ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo ::                                                      ::
echo :: Upgrade at own risk! No warranty for bricked boards! ::
echo ::                                                      ::
echo :: Choose ATTiny85 bootloader upgrade:                  ::
echo :: (1) Version 1.11                                     ::
echo :: (2) Version 2.6                                      ::
echo :: (i) Board Info                                       ::
echo :: (x) exit                                             ::
echo ::                                                      ::
echo ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
echo.
SET choice=
SET /P choice= Selection: 
if '%choice%'=='' (
goto :menu
)
IF %choice%==1 (goto upgrade_v111)
IF %choice%==2 (goto upgrade_v26)
IF %choice%==i (goto info)
IF %choice%==x (goto exit)
goto :menu

:upgrade_v111
title Micronucleus Upgrade 1.11
micronucleus.exe --run micronucleus-1.11.hex
pause
goto :exit

:upgrade_v26
title Micronucleus Upgrade 2.6
micronucleus.exe --run upgrade-t85_default.hex
pause
goto :exit

:info
title Micronucleus Info
micronucleus.exe --info
pause
goto :exit

:exit
exit