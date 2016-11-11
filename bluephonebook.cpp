/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <QDBusConnection>
#include <QDebug>
#include <QDBusInterface>
#include <QDBusReply>

#include "bluephonebook.h"

BluePhoneBook::BluePhoneBook(QObject *parent) : QAbstractListModel(parent)
{

}

BluePhoneBook::BluePhoneBook(QString device)
{
    roles[ContactNameRole] = "contactName";
    roles[FileNameRole] = "fileName";
    roles[PhoneNumberRole] = "phoneNumber";

    auto sessionBus = QDBusConnection::sessionBus();

    if (!sessionBus.connect("org.bluez.obex",
                            "/",
                            "org.freedesktop.DBus.ObjectManager",
                            "InterfacesAdded",
                            this,
                            SLOT(onInterfacesAdded(const QDBusObjectPath &, const InterfacesMap)))) {
        qWarning() << "Failed to connect InterfacesAdded signal";
    }

    if (!sessionBus.connect("org.bluez.obex",
                            "/",
                            "org.freedesktop.DBus.ObjectManager",
                            "InterfacesRemoved",
                            this,
                            SLOT(onInterfacesRemoved(const QDBusObjectPath &, const QStringList)))) {
        qWarning() << "Failed to connect InterfacesRemoved signal";
    }

    requestPhoneBookAccess(device);
}

int BluePhoneBook::rowCount(const QModelIndex &parent) const {
    return m_contacts.size();
}

QVariant BluePhoneBook::data(const QModelIndex &index, int role) const {
    const Contact &c = m_contacts.at(index.row());

    switch (role) {
        case ContactNameRole:
            return c.personName();
        case FileNameRole:
            return c.fileName();
        case PhoneNumberRole:
            return c.phoneNumber();
        default:
            return "Unknown";
    };
}

QHash<int, QByteArray> BluePhoneBook::roleNames() const
{
    return roles;
}

void BluePhoneBook::requestPhoneBookAccess(const QString &device)
{
    QDBusInterface client("org.bluez.obex",
                          "/org/bluez/obex",
                          "org.bluez.obex.Client1",
                          QDBusConnection::sessionBus());

    QMap<QString, QVariant> args;
    args["Target"] = "PBAP";
    QDBusReply<QDBusObjectPath> reply = client.call("CreateSession", device, args);
    if (!reply.isValid()) {
        qWarning() << "Failed to create OBEX session: " << reply.error();
        return;
    }

    requestPhoneBook(reply.value().path());
}

void BluePhoneBook::requestPhoneBook(const QString &pbPath)
{
    QDBusInterface client("org.bluez.obex",
                          pbPath,
                          "org.bluez.obex.PhonebookAccess1",
                          QDBusConnection::sessionBus());

    QDBusReply<void> reply1 = client.call("Select", "int", "PB");
    if (!reply1.isValid()) {
        qWarning() << "Failed to select internal phonebook: " << reply1.error();
        return;
    }

    QMap<QString, QVariant> map;
    QDBusReply<QList<Contact> > reply2 = client.call("List", map);
    if (!reply2.isValid()) {
        qWarning() << "Failed to list contacts: " << reply2.error();
        return;
    }

    beginResetModel();
    m_contacts = reply.value();
    endResetModel();
}

void BluePhoneBook::onInterfacesAdded(const QDBusObjectPath &path,
                                      const InterfacesMap interfaces)
{
}

void BluePhoneBook::onInterfacesRemoved(const QDBusObjectPath &path,
                                        const QStringList interfaces)
{
}
