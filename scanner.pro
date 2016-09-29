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

target.path = $$[QT_INSTALL_EXAMPLES]/bluetooth/scanner
INSTALLS += target

HEADERS += \
    blueconnect.h
