# Build DecentralTalk for Linux

mkdir -p build
cd build
qmake6 -config release ../src/qt/decentralchat-qt/DecentralChat.pro
sudo make
