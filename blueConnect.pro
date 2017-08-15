TEMPLATE = lib
TARGET = BlueConnect
QT = core quick dbus qml quick
CONFIG += c++11 qt plugin

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.pelagicore.bluetooth

SOURCES += \
    blueconnect.cpp \
    blueplayer.cpp \
    bluephonebook.cpp \
    contact.cpp \
    bluehandsfree.cpp \
    modem.cpp \
    plugin.cpp \
    blueconnectpropertycallback.cpp


HEADERS += \
    blueconnect.h \
    blueplayer.h \
    bluephonebook.h \
    customtypes.h \
    contact.h \
    bluehandsfree.h \
    modem.h \
    plugin.h \
    blueconnectpropertycallback.h

QMAKE_CXXFLAGS += -std=c++11

DISTFILES += qmldir \
    example/DeviceList.qml \
    example/Dialer.qml \
    example/HandsfreeTab.qml \
    example/main.qml \
    example/PhonebookTab.qml \
    example/PlayerControls.qml \
    example/PlayerMetadata.qml \
    example/PlayerTab.qml

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
