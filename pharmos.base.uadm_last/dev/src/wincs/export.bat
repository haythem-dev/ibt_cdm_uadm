@echo on

set PROJECT=UadmWebLoginService
set PWD=%CD%

set BASEDIR="%PWD%"\..\..\..

set SRCDIR=%BASEDIR%\dev\src\wincs
set EXTDIR=%BASEDIR%\dev\ext
set DESTDIR=%BASEDIR%\export

echo -----------------------------------------------------
echo  copy %PROJECT% files to export directory
echo       from %SRCDIR%
echo       to   %DESTDIR%
echo  compress export files to %DESTDIR%\%PROJECT%.zip
echo -----------------------------------------------------

rmdir /S /Q %DESTDIR% 2>NUL

set PROJSRCDIR=%SRCDIR%\%PROJECT%
set PROJDESTDIR=%DESTDIR%\%PROJECT%

robocopy %PROJSRCDIR%\Content       %PROJDESTDIR%\Content       *.*             /S
robocopy %PROJSRCDIR%\Fonts         %PROJDESTDIR%\Images         *.*            /S
robocopy %PROJSRCDIR%\Scripts       %PROJDESTDIR%\Scripts       *.*             /S
robocopy %PROJSRCDIR%\Views         %PROJDESTDIR%\Views         *.*             /S
robocopy %PROJSRCDIR%               %PROJDESTDIR%               favicon.ico
robocopy %PROJSRCDIR%               %PROJDESTDIR%               Global.asax
robocopy %PROJSRCDIR%               %PROJDESTDIR%               web.config
robocopy %PROJSRCDIR%               %PROJDESTDIR%               web.ACC.config
robocopy %PROJSRCDIR%               %PROJDESTDIR%               web.PRD.config

robocopy %PROJSRCDIR%               %PROJDESTDIR%               log4net.config
robocopy %PROJSRCDIR%               %PROJDESTDIR%               log4net.ACC.config
robocopy %PROJSRCDIR%               %PROJDESTDIR%               log4net.PRD.config

robocopy %PROJSRCDIR%               %PROJDESTDIR%               UadmAppSettings.json
robocopy %PROJSRCDIR%               %PROJDESTDIR%               UadmAppSettings.ACC.json
robocopy %PROJSRCDIR%               %PROJDESTDIR%               UadmAppSettings.PRD.json

robocopy %PROJSRCDIR%\bin           %PROJDESTDIR%\bin           *.dll
robocopy %PROJSRCDIR%\bin\bg        %PROJDESTDIR%\bin\bg        *.dll
robocopy %PROJSRCDIR%\bin\de        %PROJDESTDIR%\bin\de        *.dll
robocopy %PROJSRCDIR%\bin\en        %PROJDESTDIR%\bin\en        *.dll
robocopy %PROJSRCDIR%\bin\fr        %PROJDESTDIR%\bin\fr        *.dll
robocopy %PROJSRCDIR%\bin\hr        %PROJDESTDIR%\bin\hr        *.dll
robocopy %PROJSRCDIR%\bin\Images    %PROJDESTDIR%\bin\Images    *.*             /S
robocopy %PROJSRCDIR%\bin\roslyn    %PROJDESTDIR%\bin\roslyn    *.*             /S

robocopy %PROJSRCDIR%\bin\App_GlobalResources    %PROJDESTDIR%\bin\App_GlobalResources    *.resx             /S


cd %PROJDESTDIR%

echo zip %PROJDESTDIR%\%PROJECT%.zip ...
7z a -sdel .\%PROJECT%.zip 

robocopy %SRCDIR%                   %PROJDESTDIR%               ivy.xml
robocopy %SRCDIR%                   %PROJDESTDIR%               build.xml

cd /D %PWD%
