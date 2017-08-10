/*
 * Copyright (C) 2016, Pelagicore AB.
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 * SPDX-License-Identify: MPL-2.0
 */

#include "plugin.h"
#include "blueconnect.h"
#include "bluehandsfree.h"
#include "bluephonebook.h"
#include "blueplayer.h"

#include <qqml.h>

Plugin::Plugin() {

}


void Plugin::registerTypes(const char *uri)
{
    // @uri com.pelagicore.bluetooth
    qmlRegisterType<BluePlayer>(uri, 1, 0, "BluePlayer");
    qmlRegisterType<BlueConnect>(uri, 1, 0, "BlueConnect");
    qmlRegisterType<BluePhoneBook>(uri, 1, 0, "BluePhoneBook");
    qmlRegisterType<BlueHandsfree>(uri, 1, 0, "BlueHandsfree");
}
