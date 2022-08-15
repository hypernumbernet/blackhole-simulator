set work=C:\develop\blackhole-simulator\binary\blackhole-simulator-win-x64
set src=C:\develop\_build-blackhole-simulator-Desktop_Qt_6_2_4_MSVC2019_64bit-Release\release
set dst=C:\develop\blackhole-simulator\binary\blackhole-simulator-win-x64.7z

mkdir %work%
cd C:\Qt\6.2.4\msvc2019_64\bin
windeployqt --dir %work% %src%\blackhole-simulator.exe
copy %src%\blackhole-simulator.exe %work%
mkdir %work%\settings
copy C:\develop\blackhole-simulator\settings %work%\settings
del /q %dst%
cd "C:\Program Files\7-Zip"
7z a %dst% %work%
rmdir /s /q %work%
