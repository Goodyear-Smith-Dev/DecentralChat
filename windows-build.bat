:: Build DecentralCHat for Windows

mkdir build\.qm
xcopy /S /Q /Y /F "src\qt\decentralchat-qt\DecentralChat_es_MX.ts" "build\.qm\"
move "build\.qm\DecentralChat_es_MX.ts" "build\.qm\DecentralChat_es_MX.qm"
cd build
"C:\Qt\6.5.2\msvc2019_64\bin\qmake6.exe" ../src/qt/decentralchat-qt/DecentralChat.pro
"C:\Qt\Tools\QtCreator\bin\jom\jom.exe"
