#
#!/bin/bash
#
# Build DecentralChat for macOS
#

# Build Crypto++
function cryptopp {
	echo "Building Crypto++"
	cd vendor/cryptopp
	sudo make CRYPTOPP_CXXFLAGS+="-std=c++11 -stdlib=libc++"

	# Copy the static library to the DecentralChat source tree
	mkdir -p ../../lib
	sudo install libcryptopp.a ../../lib

	cd ../../
}

# Build DecentralChat
function decentralchat {
	echo "Building DecentralChat"
	mkdir -p build
	cd build
	qmake6 -spec macx-clang -config release ../src/qt/decentralchat-qt/DecentralChat.pro
	sudo make
}

function clean() {
	echo "Cleaning build directory"
	cd build
	sudo make clean
	exit
}

if [[ $1 == "clean" ]]; then
	clean
elif [[ $1 == "cryptopp" ]]; then
	cryptopp
elif [[ $1 == "decentralchat" ]]; then
	decentralchat
else
	cryptopp
	echo # Echo an empty new line for formatting purposes
	decentralchat
fi