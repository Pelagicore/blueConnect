/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Zeeshan Ali <zeeshan.ali@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "blueconnect.h"
#include "iostream"
#include <unistd.h>

std::ostream& operator<<(std::ostream& str, const QString& string) {
    return str << string.toStdString();
}

QString getAudioSourceUUID(QDBusInterface *dev)
{
    auto uuids = dev->property("UUIDs").toStringList();

    for (auto k = uuids.begin(); k != uuids.end(); ++k) {
        QString str = (*k);
        QStringList parts = str.split("-");
        if (parts.length() < 1) {
            qDebug() << "Ignoring " << *k;
            continue;
        }

        uint profile = parts.at(0).toUInt(Q_NULLPTR, 16);
        if (profile == 0x110A)
           return str;
    }

    return Q_NULLPTR;
}

BlueConnect::BlueConnect(QObject *parent) : QAbstractListModel(parent)
{
    // Set names to the role name hash container (QHash<int, QByteArray>)
    // model.name, model.hue, model.saturation, model.brightness
    roles[NameRole] = "name";
    roles[AddressRole] = "address";
    roles[SelectedRole] = "selected";

    connected = -1;

    qDBusRegisterMetaType <InterfacesMap> ();
    qDBusRegisterMetaType <ObjectsMap> ();

    auto bus = QDBusConnection::systemBus();
    QDBusInterface manager("org.bluez",
                           "/",
                           "org.freedesktop.DBus.ObjectManager",
                           bus);

    fetchObjects(manager);

    if (!bus.connect("org.bluez",
                     "/",
                     "org.freedesktop.DBus.ObjectManager",
                     "InterfacesAdded",
                     this,
                     SLOT(onInterfacesAdded(const QDBusObjectPath &, const InterfacesMap)))) {
        qWarning() << "Failed to connect InterfacesAdded signal";
    }

    if (!bus.connect("org.bluez",
                     "/",
                     "org.freedesktop.DBus.ObjectManager",
                     "InterfacesRemoved",
                     this,
                     SLOT(onInterfacesRemoved(const QDBusObjectPath &, const QStringList)))) {
        qWarning() << "Failed to connect InterfacesRemoved signal";
    }
}

BlueConnect::~BlueConnect()
{
}

void BlueConnect::onInterfacesAdded(const QDBusObjectPath &path,
                                    const InterfacesMap interfaces)
{
    for (auto i = interfaces.begin(); i != interfaces.end(); ++i) {
        if (i.key() == "org.bluez.Device1") {
            std::cout << "'org.bluez.Device1' interface added at "
                      << path.path()
                      << std::endl;

            addDevice (path);

            break;
        } else if (i.key() == "org.bluez.MediaPlayer1") {
            std::cout << "'org.bluez.MediaPlayer1' interface added at "
                      << path.path()
                      << std::endl;

            BluePlayer *mediaPlayer = new BluePlayer(QDBusObjectPath(path.path()), NULL);
            emit mediaPlayerAdded(mediaPlayer);

            break;
        }
    }
}

void BlueConnect::onInterfacesRemoved(const QDBusObjectPath &path,
                                      const QStringList interfaces)
{
    bool ignore = true;

    for (auto i = interfaces.begin(); i != interfaces.end(); ++i) {
        if (*i == "org.bluez.Device1") {
            ignore = false;
            std::cout << "'org.bluez.Device1' interface removed from "
                      << path.path()
                      << std::endl;

            break;
        }
    }

    if (ignore)
        return;

    for (int i = 0; i < devices.length(); i++) {
        if (path.path() == devices[0]->path()) {
            emit beginRemoveRows(QModelIndex(), i, i);
            devices.removeAt(i);
            emit endRemoveRows();
        }
    }
}

void BlueConnect::fetchObjects(QDBusInterface &manager)
{
    QDBusReply<QMap<QDBusObjectPath,QMap<QString,QVariantMap > > > reply;
    reply = manager.call("GetManagedObjects");
    if (!reply.isValid()) {
        std::cout << "Failed to connect to bluez: " << reply.error().message() << std::endl;

        return;
    }

    auto objects = reply.value();

    for (auto i = objects.begin(); i != objects.end(); ++i) {
        auto ifaces = i.value();

        std::cout << i.key().path() << std::endl;
        for (auto j = ifaces.begin(); j != ifaces.end(); ++j) {
            if (j.key() == "org.bluez.Device1")
                addDevice (i.key());
            else if (j.key() == "org.bluez.Adapter1")
                setupAdapter (i.key());
        }
    }
}

void BlueConnect::addDevice(QDBusObjectPath path)
{
    QDBusInterface *dev;
    dev = new QDBusInterface ("org.bluez",
                              path.path(),
                              "org.bluez.Device1",
                              QDBusConnection::systemBus());
    auto uuid = getAudioSourceUUID(dev);
    if (uuid != Q_NULLPTR && !checkExistingDev(dev)) {
        emit beginInsertRows(QModelIndex(), devices.length(), devices.length());
        devices << dev;
        emit endInsertRows();
    }
}

void BlueConnect::setupAdapter(QDBusObjectPath path)
{
    QDBusInterface adapter("org.bluez",
                           path.path(),
                           "org.bluez.Adapter1",
                           QDBusConnection::systemBus());
    adapter.setProperty("Powered", true);

    QDBusReply<void> reply = adapter.call("StartDiscovery");
    if (!reply.isValid()) {
        std::cout << "Failed to start discovery: " << reply.error().message() << std::endl;

        return;
    }
}

bool BlueConnect::checkExistingDev(QDBusInterface *dev)
{
    for (auto i = devices.begin(); i != devices.end(); ++i) {
        auto existing = *i;

        if (existing->path() == dev->path())
            return true;
    }

    return false;
}

BluePlayer * BlueConnect::connect (uint index)
{
    if (connected >= 0)
        return Q_NULLPTR;

    auto dev = devices[index];
    auto address = dev->property("Address").toString();
    std::cout << "will connect to:" << address << std::endl;

    auto uuid = getAudioSourceUUID(dev);
    if (uuid == Q_NULLPTR)
        return Q_NULLPTR;

    if (dev->property("Paired").toBool()) {
        std::cout << "Device already paired" << std::endl;
    } else {
        QDBusReply<void> reply = dev->call("Pair");
        if (!reply.isValid()) {
            std::cout << "Failed to pair: " << reply.error().message() << std::endl;

            return Q_NULLPTR;
        }
    }

    QDBusReply<void> reply = dev->call("ConnectProfile", uuid);
    if (!reply.isValid()) {
        std::cout << "Failed to connect: " << reply.error().message() << std::endl;

        return Q_NULLPTR;
    }
    connected = index;
    emit connectionChanged();
    emit dataChanged(createIndex(index - 1, 0), createIndex(index + 1, 0));

    return NULL;
}

void BlueConnect::disconnect ()
{
    if(connected < 0 || connected >= devices.length())
        return;

    auto dev = devices[connected];
    auto name = dev->property("Name").toString();
    std::cout << "Disconnecting " << name << std::endl;

    QDBusReply<void> reply = dev->call("Disconnect");
    if (!reply.isValid()) {
        std::cout << "Failed to disconnect: " << reply.error().message() << std::endl;

        return;
    }
    auto index = connected;
    connected = -1;
    emit connectionChanged();
    emit dataChanged(createIndex(index - 1, 0), createIndex(index + 1, 0));
}

// QAbstractItemModel interface
int BlueConnect::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return devices.length();
}

QVariant BlueConnect::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= devices.length())
        return QVariant();

    switch (role) {
    case NameRole:
        return devices[row]->property("Name");
    case AddressRole:
        return devices[row]->property("Address");
    case SelectedRole:
        return (row == connected);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BlueConnect::roleNames() const
{
    return roles;
}

bool BlueConnect::isConnected() const
{
    return (connected >= 0);
}
