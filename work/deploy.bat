set work=C:\develop\blackhole-simulator\binary\blackhole-simulator-win-x64
set src=C:\develop\_build-blackhole-simulator-Desktop_Qt_5_15_2_MSVC2019_64bit\release
set dst=C:\develop\blackhole-simulator\binary\blackhole-simulator-win-x64.7z

mkdir %work%
cd C:\Qt\5.15.2\msvc2019_64\bin
windeployqt --dir %work% %src%\blackhole-simulator.exe
copy %src%\blackhole-simulator.exe %work%
cd "C:\Program Files\7-Zip"
7z a %dst% %work%
rmdir /s /q %work%
