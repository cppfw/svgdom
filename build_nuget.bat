@echo off
cd %~dp0

"%VS140COMNTOOLS%\VsMSBuildCmd.bat" && ^
msbuild msvs_solution/msvs_solution.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x86 /v:minimal /nologo && ^
msbuild msvs_solution/msvs_solution.sln /t:Rebuild /p:Configuration=Release /p:Platform=x86 /v:minimal /nologo && ^
msbuild msvs_solution/msvs_solution.sln /t:Rebuild /p:Configuration=Debug /p:Platform=x64 /v:minimal /nologo && ^
msbuild msvs_solution/msvs_solution.sln /t:Rebuild /p:Configuration=Release /p:Platform=x64 /v:minimal /nologo && ^
powershell Write-NuGetPackage nuget.autopkg
