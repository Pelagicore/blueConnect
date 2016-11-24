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

#include "bluehandsfree.h"
#include "modem.h"

BlueHandsfree::BlueHandsfree(QObject *parent) :
    QAbstractListModel(parent),
    m_selectedModem(NULL)
{
    roles[DeviceName] = "deviceName";
    roles[Selected] = "selected";

    auto systemBus = QDBusConnection::systemBus();

    if (!systemBus.connect("org.ofono",
                           "/",
                           "org.ofono.Manager",
                           "ModemAdded",
                           this,
                           SLOT(onModemAdded(const QDBusObjectPath &, const QVariantMap)))) {
        qWarning() << "Failed to connect ModemAdded signal";
    }

    if (!systemBus.connect("org.ofono",
                           "/",
                           "org.ofono.Manager",
                           "ModemRemoved",
                           this,
                           SLOT(onModemRemoved(const QDBusObjectPath &)))) {
        qWarning() << "Failed to connect ModemAdded signal";
    }

    fetchModems();
}

void BlueHandsfree::fetchModems()
{
    QDBusInterface manager("org.ofono", "/", "org.ofono.Manager", QDBusConnection::systemBus());

    QDBusReply<QList<Modem> > reply;
    reply = manager.call("GetModems");
    if (!reply.isValid()) {
        qWarning() << "Failed to connect to ofono: " << reply.error().message();

        return;
    }

    beginResetModel();
    m_modems  = reply.value();
    endResetModel();
}

int BlueHandsfree::rowCount(const QModelIndex &parent) const {
    return m_modems.size();
}

QVariant BlueHandsfree::data(const QModelIndex &index, int role) const {
    const Modem &c = m_modems.at(index.row());

    switch (role) {
        case DeviceName:
            return c.name();
        case Selected:
            return m_selectedModem ? c.objectPath() == m_selectedModem->objectPath() : false;
        default:
            return "Unknown";
    };
}

QHash<int, QByteArray> BlueHandsfree::roleNames() const
{
    return roles;
}

void BlueHandsfree::dial(QString number)
{
    if (!m_selectedModem) {
        qWarning() << "No modem selected";
        return;
    }

    qDebug() << "Dialing " << number << " with " << m_selectedModem->objectPath().path();

    QDBusInterface iface("org.ofono",
                         m_selectedModem->objectPath().path(),
                         "org.ofono.VoiceCallManager",
                         QDBusConnection::systemBus());
    iface.call("Dial", number, "default");

}

void BlueHandsfree::hangUp()
{
    if (!m_selectedModem) {
        qWarning() << "No modem selected";
        return;
    }

    QDBusInterface iface("org.ofono",
                         m_selectedModem->objectPath().path(),
                         "org.ofono.VoiceCallManager",
                         QDBusConnection::systemBus());
    iface.call("HangupAll");
}

void BlueHandsfree::select(int idx)
{
    if (idx < m_modems.size()) {
        m_selectedModem = &m_modems[idx];
    } else {
        qWarning() << "Modem not found";
    }

    emit dataChanged(createIndex(idx - 1, 0), createIndex(idx + 1, 0));
}

void BlueHandsfree::onModemAdded(const QDBusObjectPath &path,
                                      const QVariantMap modem)
{
    fetchModems();
}

void BlueHandsfree::onModemRemoved(const QDBusObjectPath &path)
{
    fetchModems();
}
