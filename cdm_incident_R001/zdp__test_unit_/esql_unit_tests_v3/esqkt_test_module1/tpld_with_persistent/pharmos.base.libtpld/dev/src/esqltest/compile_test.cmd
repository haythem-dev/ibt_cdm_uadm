@echo off
echo Testing ESQL compilation...

echo Step 1: Preprocessing dbio.cxx to dbio.ec
esqlc -e dbio.cxx
if %ERRORLEVEL% NEQ 0 (
    echo ESQL preprocessing failed!
    pause
    exit /b 1
)

echo Step 2: Check generated .ec file
if exist dbio.ec (
    echo dbio.ec generated successfully
) else (
    echo dbio.ec not found!
    pause
    exit /b 1
)

echo Step 3: Compile esqltest.c
cl /c esqltest.c /Fo:esqltest.obj
if %ERRORLEVEL% NEQ 0 (
    echo C compilation failed!
    pause
    exit /b 1
)

echo Step 4: Compile dbio.ec
cl /c dbio.ec /Fo:dbio.obj
if %ERRORLEVEL% NEQ 0 (
    echo ESQL-C compilation failed!
    pause
    exit /b 1
)

echo All compilation steps completed successfully!
pause