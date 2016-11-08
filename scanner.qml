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

Item {
    id: top

    property BluePlayer btPlayer

    BlueConnect {
        id: btModel

        onMediaPlayerAdded: {
            top.btPlayer = mediaPlayer
            playerControls.btPlayer = mediaPlayer
            playerMetadata.btPlayer = mediaPlayer
        }
    }

    DeviceList {
        id: deviceList
        btModel: btModel

        width: top.width
        anchors.top: top.top
        anchors.bottom: playerMetadata.top
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
