@echo off

setlocal

set SourceName=%1
set SourceName=%SourceName:"=%
set DestDir=%2
set DestDir=%DestDir:"=%

for %%I in ("%~dp0.") do for %%J in ("%%~dpI.") do set ParentFolderName=%%~dpnxJ
set UtilsDir=%ParentFolderName%\Utils

"%UtilsDir%\7-Zip\x64\7za.exe" x -tzip "%SourceName%.zip" -o"%DestDir%"
