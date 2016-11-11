import QtQuick 2.0
import QtQuick.Controls 1.4

import org.qt 1.0

Item {
    id: playerTab

    DeviceList {
        id: deviceList
        model: blueConnect

        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: playerMetadata.top
        anchors.margins: 10
    }

    PlayerMetadata {
        id: playerMetadata

        width: parent.width
        anchors.bottom: playerControls.top
        height: 75
    }

    PlayerControls {
        id: playerControls

        anchors.bottom: parent.bottom
        width: parent.width
        height: 75
    }


}
