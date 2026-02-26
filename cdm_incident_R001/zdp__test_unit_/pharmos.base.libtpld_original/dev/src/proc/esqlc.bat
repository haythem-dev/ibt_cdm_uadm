@ECHO ON
REM $(SolutionDir)proc\esqlc.bat $(SolutionDir) $(IntDir) $(ProjectDir) %(FullPath) %(Filename) $(INFORMIXDIR) [Config]
set SolutionDir=%~1
set IntDir=%~2
set ProjectDir=%~3
set FullPath=%~4
set Filename=%~5
set INFORMIXDIR=%~6
set Config=%~7
REM Use system INFORMIXDIR if %6 is empty or invalid
if not defined INFORMIXDIR set INFORMIXDIR=%INFORMIXDIR%
if not defined INFORMIXDIR set INFORMIXDIR=C:\bin\IBM Informix Client SDK
echo INFORMIXDIR=%INFORMIXDIR%
echo PATH=%PATH%
echo INFORMIXSERVER=%INFORMIXSERVER%
echo Preprocess file %FullPath% to %ProjectDir%%IntDir%%Filename%.ec
where cl
echo Running cl /nologo /P %FullPath% /I"%ProjectDir%." /Fi:"%ProjectDir%%IntDir%%Filename%.ec"
cl /nologo /P %FullPath% /I"%ProjectDir%." /Fi:"%ProjectDir%%IntDir%%Filename%.ec"
if %ERRORLEVEL% neq 0 (
    echo cl preprocessing failed with error %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
echo Creating directory %ProjectDir%%IntDir%
mkdir %ProjectDir%%IntDir% >NUL 2>NUL
pushd %ProjectDir%%IntDir%
if %ERRORLEVEL% neq 0 (
    echo pushd failed with error %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
echo Run %INFORMIXDIR%\bin\esql on %ProjectDir%%IntDir%%Filename%.ec
echo Executing: "%INFORMIXDIR%\bin\esql.exe" -n -p -mserr -dcmdl -g %Filename%.ec %Filename%.c
where esql
if not exist "%INFORMIXDIR%\bin\esql.exe" (
    echo ERROR: %INFORMIXDIR%\bin\esql.exe not found
    exit /b 1
)
"%INFORMIXDIR%\bin\esql.exe" -n -p -mserr -dcmdl -g %Filename%.ec %Filename%.c
if %ERRORLEVEL% neq 0 (
    echo esql failed with error %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
del %Filename%.ec
popd
echo Compile %ProjectDir%%IntDir%%Filename%.c
REM Use /MDd for Debug, /MD for Release
if "%Config%"=="Debug" (
    echo Running cl /nologo /TP /c /MDd /I"%INFORMIXDIR%\incl\esql" "%ProjectDir%%IntDir%%Filename%.c" /Fo:"%ProjectDir%%IntDir%%Filename%.obj"
    cl /nologo /TP /c /MDd /I"%INFORMIXDIR%\incl\esql" "%ProjectDir%%IntDir%%Filename%.c" /Fo:"%ProjectDir%%IntDir%%Filename%.obj"
) else (
    echo Running cl /nologo /TP /c /MD /I"%INFORMIXDIR%\incl\esql" "%ProjectDir%%IntDir%%Filename%.c" /Fo:"%ProjectDir%%IntDir%%Filename%.obj"
    cl /nologo /TP /c /MD /I"%INFORMIXDIR%\incl\esql" "%ProjectDir%%IntDir%%Filename%.c" /Fo:"%ProjectDir%%IntDir%%Filename%.obj"
)
if %ERRORLEVEL% neq 0 (
    echo cl compilation failed with error %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)
del "%ProjectDir%%IntDir%%Filename%.c?"