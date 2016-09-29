QT = core quick dbus
SOURCES += qmlscanner.cpp \
    blueconnect.cpp

TARGET = qml_scanner
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
    blueconnect.h
