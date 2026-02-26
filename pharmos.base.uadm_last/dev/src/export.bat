@echo off

set PROJECT=uadm
set PWD=%CD%

set BASEDIR=%PWD%\..\..

set SRCDIR=%BASEDIR%\dev\src
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export\%PROJECT%
set BINDIR=%DESTDIR%\bin
set LIBDIR=%DESTDIR%\lib
set INCDIR=%DESTDIR%\include

echo -----------------------------------------------------
echo  copy %PROJECT% files to export directory 
echo       from %SRCDIR%
echo       from %EXTDIR%
echo       to   %DESTDIR%
echo  compress export files to %DESTDIR%\%PROJECT%.zip
echo -----------------------------------------------------

rmdir /S /Q %DESTDIR% 2>nul
mkdir %DESTDIR%

if "%1" == "win32" (
  mkdir %LIBDIR%
  mkdir %LIBDIR%\2010_static
  mkdir %LIBDIR%\libcrypto
  mkdir %LIBDIR%\informix
  mkdir %BINDIR%
  mkdir %INCDIR%

  echo copy binaries ...
  xcopy %SRCDIR%\win\libappuadm\libcrypto_static\32bit\libcrypto-1_1.dll	%BINDIR%\                     /d /y /q
  xcopy %SRCDIR%\win\libappuadm\informix\32bit\*.dll						%BINDIR%\                     /d /y /q
  xcopy %SRCDIR%\win\libappuadm\informix\32bit\*.exe						%BINDIR%\                     /d /y /q
  
  echo copy libraries...
  xcopy %SRCDIR%\win\libappuadm\2010\Debug\libappuadm.lib 					%LIBDIR%\2010_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2010\Debug\libappuadm.pdb 					%LIBDIR%\2010_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2010\Debug\libappuadm.res 					%LIBDIR%\2010_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2010\Release\libappuadm.lib 				%LIBDIR%\2010_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2010\Release\libappuadm.res 				%LIBDIR%\2010_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\libcrypto_static\32bit\libcrypto.lib		%LIBDIR%\libcrypto\           /d /y /q
  xcopy %SRCDIR%\win\libappuadm\informix\32bit\lib\cpp\libthc++.lib			%LIBDIR%\informix\            /d /y /q

  echo copy includes...
  xcopy %SRCDIR%\win\libappuadm\UADMLoginManager.h 		%INCDIR%\   /d /y /q
) else (
  mkdir %BINDIR%
  mkdir %LIBDIR%
  mkdir %LIBDIR%\2015_static
  mkdir %LIBDIR%\2019_static
  mkdir %LIBDIR%\2019_shared
  mkdir %LIBDIR%\2015_static\Debug
  mkdir %LIBDIR%\2015_static\Release
  mkdir %LIBDIR%\2019_static\Debug
  mkdir %LIBDIR%\2019_static\Release
  mkdir %LIBDIR%\2019_shared\Debug
  mkdir %LIBDIR%\2019_shared\Release
  mkdir %LIBDIR%\libcrypto_static
  mkdir %INCDIR%

  echo copy binaries ...
  xcopy %EXTDIR%\servclnt\bin\Release\*.dll 						%BINDIR%\   /d /y /q
  xcopy %SRCDIR%\win\presentation\Release\UsrAdmin.exe 				%BINDIR%\   /d /y /q
  xcopy %SRCDIR%\win\presentation\uadm.config.ini 					%BINDIR%\   /d /y /q
  xcopy %SRCDIR%\win\libiamuadm\Release\libiamuadm.dll 				%BINDIR%\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Release\libappuadm.dll	%BINDIR%\   /d /y /q

  echo copy libraries...
  xcopy %SRCDIR%\win\libappuadm\2015\Debug\libappuadm.lib 						%LIBDIR%\2015_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2015\Debug\libappuadm.pdb 						%LIBDIR%\2015_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2015\Debug\libappuadm.res 						%LIBDIR%\2015_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2015\Release\libappuadm.lib 					%LIBDIR%\2015_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2015\Release\libappuadm.res 					%LIBDIR%\2015_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019\Debug\libappuadm.lib 						%LIBDIR%\2019_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019\Debug\libappuadm.pdb 						%LIBDIR%\2019_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019\Debug\libappuadm.res 						%LIBDIR%\2019_static\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019\Release\libappuadm.lib 					%LIBDIR%\2019_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019\Release\libappuadm.res 					%LIBDIR%\2019_static\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Debug\libappuadm.dll 				%LIBDIR%\2019_shared\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Debug\libappuadm.lib 				%LIBDIR%\2019_shared\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Debug\libappuadm.pdb 				%LIBDIR%\2019_shared\Debug\   /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Release\libappuadm.dll 				%LIBDIR%\2019_shared\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\2019_shared\Release\libappuadm.lib 				%LIBDIR%\2019_shared\Release\ /d /y /q
  xcopy %SRCDIR%\win\libappuadm\libcrypto_static\64bit\libcrypto_static.lib		%LIBDIR%\libcrypto_static\    /d /y /q

  echo copy includes...
  xcopy %SRCDIR%\win\libappuadm\UADMLoginManager.h 		%INCDIR%\   /d /y /q
)

cd %DESTDIR%
echo zip %DESTDIR%
7z.exe a -sdel .\%PROJECT%.zip .

xcopy %SRCDIR%\build.xml %DESTDIR%\   /d /y /q
xcopy %SRCDIR%\ivy.xml   %DESTDIR%\   /d /y /q

cd /D %PWD%
