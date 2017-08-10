/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQml 2.2

import com.pelagicore.bluetooth 1.0

Item {
    function millisToDate(millis) {
        var date = new Date(millis);
        return Qt.formatDateTime(date, "m:ss")
    }

    Timer {
        id: positionTimer
        interval: 1000;
        running: btPlayer? btPlayer.status == "playing" : false
        repeat: true
        onTriggered: {
            positionSlider.maximumValue = btPlayer.duration
            positionSlider.value = btPlayer.position
        }
    }

    Slider {
        id: positionSlider
        anchors.right: parent.right
        anchors.left: parent.left
        value: 0
        anchors.top: parent.top
        enabled: false
    }

    Label {
        id: title
        text: btPlayer? "Title: " + btPlayer.title: ""
        anchors.top: positionSlider.bottom
        anchors.left: parent.left
        elide: Text.ElideRight
        width: parent.width * 0.75
        anchors.leftMargin: 20
    }

    Label {
        id: album
        text: btPlayer ? "Album: " + btPlayer.album : ""
        anchors.top: title.bottom
        anchors.left: title.left
        elide: Text.ElideRight
        width: parent.width * 0.75
    }

    Label {
        id: artist
        text: btPlayer? "Artist: " + btPlayer.artist : ""
        anchors.top: album.bottom
        anchors.left: title.left
        elide: Text.ElideRight
        width: parent.width * 0.75
    }

    Label {
        id: duration
        text: btPlayer? "Duration: " + millisToDate(btPlayer.duration) : ""
        anchors.top: positionSlider.bottom
        anchors.right: parent.right
        anchors.rightMargin: 20
    }

    Label {
        id: genre
        text: btPlayer? "Genre: " + btPlayer.genre : ""
        anchors.top: duration.bottom
        anchors.left: duration.left
    }

    Label {
        id: status
        text: btPlayer? "Status: " + btPlayer.status : ""
        anchors.top: genre.bottom
        anchors.left: duration.left
    }
}
