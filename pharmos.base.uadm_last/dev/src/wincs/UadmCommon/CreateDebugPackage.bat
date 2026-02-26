c:\dev\tools\NuGet.exe pack UadmCommon.debug.nuspec -NonInteractive -OutputDirectory package -Properties Configuration="Debug" -IncludeReferencedProjects -Verbosity Detailed

copy package\*.nupkg c:\Packages

pause