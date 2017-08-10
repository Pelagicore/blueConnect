/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BLUEPHONEBOOK_H
#define BLUEPHONEBOOK_H

#include <QObject>
#include <QAbstractListModel>
#include "contact.h"
#include "customtypes.h"

class BluePhoneBook : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RoleNames {
        FileNameRole = Qt::UserRole,
        ContactNameRole = Qt::UserRole + 1,
        PhoneNumberRole = Qt::UserRole + 2,
    };

    explicit BluePhoneBook(QObject *parent = 0);
    explicit BluePhoneBook(QString device);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray> roleNames() const;

    /*
     * Start D-Bus communication with BlueZ5 OBEX service to retrieve phonebook
     */
    void requestPhoneBookAccess(const QString &device);

signals:

private slots:
    void onInterfacesAdded(const QDBusObjectPath &path,
                           const InterfacesMap interfaces);

    void onInterfacesRemoved(const QDBusObjectPath &path,
                             const QStringList interfaces);
private:
    QHash<int, QByteArray> roles;
    QList<Contact> m_contacts;
    void requestPhoneBook(const QString &pbPath);
};

#endif // BLUEPHONEBOOK_H
