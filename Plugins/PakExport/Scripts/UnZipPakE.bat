@echo off

setlocal

for %%I in ("%~dp0.") do for %%J in ("%%~dpI.") do set ParentFolderName=%%~dpnxJ

set UtilsDir=%ParentFolderName%\Utils
call %ParentFolderName%\Scripts\Cleanup.bat
cd "%UtilsDir%"
"%UtilsDir%\7-Zip\x64\7za.exe" x "%UtilsDir%\PakExport.7z" -o%UtilsDir%

::sync project settings
echo F|XCOPY /S /Q /Y /F "%ParentFolderName%\..\..\Config\DefaultEngine.ini" "%UtilsDir%\PakExport\Config\DefaultEngine.ini"
