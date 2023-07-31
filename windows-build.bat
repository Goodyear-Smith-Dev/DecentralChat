@echo off

rem setup build dir, copy over necessary file
mkdir build\.qm
xcopy /S /Q /Y /F "src\qt\decentralchat-qt\DecentralChat_es_MX.ts" "src\qt\"
move "src\qt\DecentralChat_es_MX.ts" "src\qt\decentralchat-qt\DecentralChat_es.qm"
xcopy /S /Q /Y /F "src\qt\decentralchat-qt\DecentralChat_es_MX.qm" "build\.qm\"
cd "build"


rem according to https://stackoverflow.com/a/67324703
set INCLUDE="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\ATLMFC\include;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\VS\include;C:\Program Files (x86)\Windows Kits\10\include\10.0.22000.0\ucrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\um;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\shared;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\winrt;C:\Program Files (x86)\Windows Kits\10\\include\10.0.22000.0\\cppwinrt;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\include\um"
set LIB="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\ATLMFC\lib\x64;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\lib\x64;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.8\lib\um\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.22000.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\10\\lib\10.0.22000.0\\um\x64"
set LIBPATH="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\ATLMFC\lib\x64;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\lib\x64;C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC\14.36.32532\lib\x86\store\references;C:\Program Files (x86)\Windows Kits\10\UnionMetadata\10.0.22000.0;C:\Program Files (x86)\Windows Kits\10\References\10.0.22000.0;C:\Windows\Microsoft.NET\Framework64\v4.0.30319"

rem VCToolsInstallDir + "\bin\Hostx64\x64"
set PRE_PATH = %PATH%
PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.36.32532\bin\Hostx64\x64
rem WindowsSdkVerBinPath + "\x64"
PATH=%PATH%;C:\Program Files (x86)\Windows Kits\10\bin\10.0.22000.0\x64

rem build
"C:\Qt\6.5.2\msvc2019_64\bin\qmake.exe" ../src\qt\decentralchat-qt\DecentralChat.pro -spec win32-msvc "CONFIG+=debug" "CONFIG+=qml_debug"
"C:\Qt\Tools\QtCreator\bin\jom\jom.exe" -f MakeFile.debug

rem revert and return
PATH=%PRE_PATH%
cd ..
