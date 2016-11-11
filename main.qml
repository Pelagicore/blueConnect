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
    id: main

    property BluePlayer btPlayer
    property BluePhoneBook btPhoneBook

    BlueConnect {
        id: blueConnect

        onMediaPlayerAdded: {
            main.btPlayer = mediaPlayer
        }

        onPhoneBookAdded: {
            console.log("Phone book added")
            main.btPhoneBook = phoneBook
        }
    }

    TabView {
        id: tabView
        anchors.fill: parent

        Tab {
            title: "Audio player & devices"
            PlayerTab {}
        }

        Tab {
            title: "Phonebook"
            PhonebookTab {}
        }
    }


}
