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
import QtQuick.Controls 1.4

import org.qt 1.0

ListView {
    enabled: !blueConnect.isConnected
    id: deviceList
    clip: true

    model: blueConnect

    delegate: Rectangle {
        id: btDelegate
        width: parent.width
        height: column.height + 10
        
        color: model.selected ? "grey" : "transparent"
        
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
            anchors.verticalCenter: bticon.verticalCenter
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
                blueConnect.connect (index);
            }
        }
    }
    focus: true
}
