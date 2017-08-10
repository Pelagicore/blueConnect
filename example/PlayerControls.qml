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

import com.pelagicore.bluetooth 1.0

Item {
    Row {
        id: playerRow

        anchors.bottom: disconnectRow.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5
        spacing: 10

        Button {
            text: "Previous"
            enabled: btPlayer != null
            onClicked: {
                btPlayer.prev()
            }
        }

        Button {
            text: "Play"
            enabled: btPlayer? btPlayer.status != "playing" : false
            onClicked: {
                btPlayer.play()
            }
        }

        Button {
            text: "Pause"
            enabled: btPlayer? btPlayer.status != "paused" : false
            onClicked: {
                btPlayer.pause()
            }
        }

        Button {
            text: "Stop"
            enabled: btPlayer? btPlayer.status != "stopped" : false
            onClicked: {
                btPlayer.stop()
            }
        }

        Button {
            text: "Next"
            enabled: btPlayer != null
            onClicked: {
                btPlayer.next()
            }
        }
    }

    Row {
        id: disconnectRow

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 5
        spacing: 10

        Button {
            text: "Disonnect"
            enabled: blueConnect.isConnected
            onClicked: {
                blueConnect.disconnect()
                btPlayer = null
            }
        }
    }
}
