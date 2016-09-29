/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Zeeshan Ali <zeeshan.ali@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import QtBluetooth 5.2
import QtQuick.Controls 1.4

import org.qt 1.0

Item {
    id: top

    property BluetoothService currentService

    BlueConnect {
        id: btModel
    }

    ListView {
        enabled: !btModel.isConnected
        id: mainList
        width: top.width
        anchors.top: top.top
        anchors.bottom: buttonGroup.top
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        clip: true

        model: btModel

        delegate: Rectangle {
            id: btDelegate
            width: parent.width
            height: column.height + 10

            color: model.selected ? "grey" : "white"

            property bool expended: false;
            clip: true
            Image {
                id: bticon
                source: "qrc:/default.png";
                width: bttext.height;
                height: bttext.height;
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
            }

            Column {
                id: column
                anchors.left: bticon.right
                anchors.leftMargin: 5
                Text {
                    id: bttext
                    text: model.name
                    font.family: "FreeSerif"
                    font.pointSize: 16
                }
            }
            Behavior on height { NumberAnimation { duration: 200} }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    btModel.connect (index);
                }
            }
        }
        focus: true
    }

    Row {
        id: buttonGroup

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5
        spacing: 10

        Button {
            text: "Disonnect"
            enabled: btModel.isConnected
            onClicked: btModel.disconnect()
        }
   }

}
