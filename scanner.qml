/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Copyright (C) 2013 BlackBerry Limited. All rights reserved.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the QtBluetooth module.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

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

    Rectangle {
        id: busy

        width: top.width * 0.7;
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: top.top;
        height: text.height*1.2;
        radius: 5
        color: "#1c56f3"

        Text {
            id: text
            text: "Scanning"
            font.bold: true
            font.pointSize: 20
            anchors.centerIn: parent
        }

        SequentialAnimation on color {
            id: busyThrobber
            ColorAnimation { easing.type: Easing.InOutSine; from: "#1c56f3"; to: "white"; duration: 1000; }
            ColorAnimation { easing.type: Easing.InOutSine; to: "#1c56f3"; from: "white"; duration: 1000 }
            loops: Animation.Infinite
        }
    }

    ListView {
        enabled: !btModel.isConnected
        id: mainList
        width: top.width
        anchors.top: busy.bottom
        anchors.bottom: buttonGroup.top
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        clip: true

        model: btModel

        //highlight: ListHighlight { }

        delegate: Rectangle {
            id: btDelegate
            width: parent.width
            height: column.height + 10

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
                    statusText.text = "Current selection: " + name;
                    btModel.connect (index);
                }
            }
        }
        focus: true
    }

    Row {
        id: status

        anchors.bottom: buttonGroup.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5
        spacing: 10

        Text {
            id: statusText
        }
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
