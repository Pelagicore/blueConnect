QT = core quick dbus
CONFIG += c++11
SOURCES += \
    blueconnect.cpp \
    blueplayer.cpp \
    main.cpp

TARGET = bluetooth-demo
TEMPLATE = app

RESOURCES += \
    scanner.qrc

OTHER_FILES += \
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
    DeviceList.qml \
    main.qml \
    PlayerTab.qml
