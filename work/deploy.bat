set devfolder=C:\develop
set qtbin=C:\Qt\6.5.1\msvc2019_64\bin
set src=%devfolder%\build-blackhole-simulator-Desktop_Qt_6_5_1_MSVC2019_64bit-Release\release

set work=%devfolder%\blackhole-simulator\binary\blackhole-simulator-win-x64
set dst=%devfolder%\blackhole-simulator\binary\blackhole-simulator-win-x64.7z

mkdir %work%
cd %qtbin%
windeployqt --dir %work% %src%\blackhole-simulator.exe
copy %src%\blackhole-simulator.exe %work%
mkdir %work%\settings
copy %devfolder%\blackhole-simulator\settings %work%\settings
del /q %dst%
cd "C:\Program Files\7-Zip"
7z a %dst% %work%
rmdir /s /q %work%
