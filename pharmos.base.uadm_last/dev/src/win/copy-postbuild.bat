@echo off

echo ==========================================================================
echo POSTBUILD COPY - START
echo ==========================================================================

echo %1 %2 %3 %4 %5

set ProjectName=%1
set Platform=%2
set Configuration=%3
set SolutionDir=%4
set OutDir=%5
REM #########################################################################
REM copy the external DLLs from servclnt
REM #########################################################################
xcopy /Y /D %SolutionDir%..\ext\servclnt\bin\%Configuration%\*.dll %OutDir%
xcopy /Y /D %SolutionDir%..\ext\servclnt\bin\%Configuration%\*.pdb %OutDir%

REM #########################################################################
REM copy the external DLLs from servclnt
REM #########################################################################
xcopy /Y /D %SolutionDir%win\libappuadm\2019_shared\%Configuration%\libappuadm.dll %OutDir%
xcopy /Y /D %SolutionDir%win\libappuadm\2019_shared\%Configuration%\libappuadm.pdb %OutDir%

REM #########################################################################
REM copy the external DLLs from servclnt
REM #########################################################################
xcopy /Y /D %SolutionDir%win\presentation\uadm.config.ini %OutDir%

