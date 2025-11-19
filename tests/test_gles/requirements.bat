@echo off
echo Download and unpack ANGLE(EGL+GLES) for Windows:

set "URL=https://github.com/mmozeiko/build-angle/releases/download/2026-05-09/angle-x64-2026-05-09.zip"
set "ZIP=angle-x64-2026-05-09.zip"

REM --- Download ZIP ---
echo Downloading...
curl -L -o "%ZIP%" "%URL%"

REM --- Extract ZIP ---
echo Extracting...
tar -xf "%ZIP%"

REM --- Copy DLLs to current folder ---
echo Copying DLLs...
copy "angle-x64\bin\libEGL.dll" . >nul
copy "angle-x64\bin\libGLESv2.dll" . >nul

REM --- Delete ZIP file ---
echo Cleaning up...
del "%ZIP%"


echo Done!
pause