QT = core quick dbus
CONFIG += c++11
SOURCES += qmlscanner.cpp \
    blueconnect.cpp \
    blueplayer.cpp

TARGET = bluetooth-demo
TEMPLATE = app

RESOURCES += \
    scanner.qrc

OTHER_FILES += \
    scanner.qml \
    default.png

#DEFINES += QMLJSDEBUGGER

target.path = /usr/bin/
INSTALLS += target

HEADERS += \
    blueconnect.h \
    blueplayer.h

QMAKE_CXXFLAGS += -std=c++11

DISTFILES += \
    PlayerControls.qml \
    PlayerMetadata.qml \
    DeviceList.qml
