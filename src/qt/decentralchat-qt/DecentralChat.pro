QT += core gui
QT_MAJOR_VERSION = 6
RESOURCES = ../resources.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$files(*.c, true) + $$files(*.cpp, true)

HEADERS = $$files(*.h, true) + $$files(*.hpp, true)

FORMS += \
    ../ui/decentralchat.ui

TRANSLATIONS += \
    DecentralChat_es_MX.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
