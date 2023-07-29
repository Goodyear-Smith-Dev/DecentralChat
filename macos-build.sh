# Build DecentralTalk for macOS

mkdir -p build
cd build
qmake6 -spec macx-clang -config release ../src/qt/decentralchat-qt/DecentralChat.pro
sudo make
