QT += core gui network
QT_MAJOR_VERSION = 6

win32:RESOURCES = ../resources-win.qrc
unix:RESOURCES = ../resources-unix.qrc

# All paths are local to the build directory
SRC = ../src
SUBMODULE_PATH = ../vendor
LIBRARY_PATH = ../lib

win32:SRC = ../..
win32:SUBMODULE_PATH = ../../../vendor
win32:LIBRARY_PATH = ../../../lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SRC_ROOT = ../.. # Relative to this directory
SOURCES += $$files("$${SRC_ROOT}/*.c", true) $$files("$${SRC_ROOT}/*.cpp", true)
HEADERS = $$files("$${SRC_ROOT}/*.h", true) $$files("$${SRC_ROOT}/*.hpp", true)

FORMS += \
    ../ui/decentralchat.ui

TRANSLATIONS += \
    DecentralChat_es_MX.ts

CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += $${SRC} $${SUBMODULE_PATH}

LIBS += $${LIBRARY_PATH}/libcryptopp.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
