/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BLUEHANDSFREE_H
#define BLUEHANDSFREE_H

#include <QObject>
#include <QAbstractListModel>
#include <QDBusInterface>
#include "contact.h"
#include "customtypes.h"

class BlueHandsfree : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames {
        DeviceName = Qt::UserRole,
        Selected = Qt::UserRole + 1,
    };

    explicit BlueHandsfree(QObject *parent = 0);

    Q_INVOKABLE void dial(QString number);
    Q_INVOKABLE void hangUp();
    Q_INVOKABLE void select(int id);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    /*
     * Start D-Bus communication with BlueZ5 OBEX service to retrieve phonebook
     */
    void requestPhoneBookAccess(const QString &device);

signals:

private slots:
    void onModemAdded(const QDBusObjectPath &path,
                           const QVariantMap modem);

    void onModemRemoved(const QDBusObjectPath &path);
private:
    QHash<int, QByteArray> roles;
    QList<Modem> m_modems;
    Modem *m_selectedModem;
    void requestPhoneBook(const QString &pbPath);
    void fetchModems();
};

#endif // BLUEHANDSFREE_H
