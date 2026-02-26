@echo off

set PWD=%CD%

set PROJECT=libtpld

set BASEDIR=%PWD%\..\..

set SRCDIR=%BASEDIR%\dev\src
set DESTDIR=%BASEDIR%\export\%PROJECT%

echo -----------------------------------------------------
echo  copy libtpld files to export directory 
echo       from %SRCDIR%
echo       to   %DESTDIR%
echo  compress export files to %DESTDIR%\%PROJECT%.zip
echo -----------------------------------------------------

rmdir /S /Q %DESTDIR%
mkdir %DESTDIR%
mkdir %DESTDIR%\include
mkdir %DESTDIR%\libstatic\Debug
mkdir %DESTDIR%\libstatic\Release

echo copy header ...
xcopy %SRCDIR%\%PROJECT%\*.h %DESTDIR%\include /d /y /q

echo copy libs ...
xcopy %SRCDIR%\x64\Debug\%PROJECT%.lib   %DESTDIR%\libstatic\Debug   /d /y /s /e /q
xcopy %SRCDIR%\x64\Debug\%PROJECT%.pdb   %DESTDIR%\libstatic\Debug   /d /y /s /e /q
xcopy %SRCDIR%\x64\Release\%PROJECT%.lib %DESTDIR%\libstatic\Release /d /y /s /e /q
xcopy %SRCDIR%\x64\Release\%PROJECT%.pdb %DESTDIR%\libstatic\Release /d /y /s /e /q


cd %DESTDIR%
echo create %DESTDIR%\%PROJECT%.zip
7z.exe a .\%PROJECT%.zip .\

rem cleanup - delete all directories in pwd
FOR /d %%d in (*) do (
rmdir /s /q "%%~d"
)

copy %SRCDIR%\build.xml %DESTDIR%\
copy %SRCDIR%\ivy.xml   %DESTDIR%\

cd /D %PWD%
