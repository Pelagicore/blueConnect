/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Zeeshan Ali <zeeshan.ali@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef BLUECONNECT_H
#define BLUECONNECT_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtDBus/QtDBus>
#include "blueplayer.h"
#include "bluephonebook.h"
#include "contact.h"
#include "customtypes.h"

class BlueConnect : public QAbstractListModel
{
    Q_OBJECT

public:
    enum RoleNames {
        AddressRole = Qt::UserRole,
        NameRole = Qt::UserRole + 1,
        SelectedRole = Qt::UserRole + 2,
    };

    explicit BlueConnect(QObject *parent=0);
    ~BlueConnect();

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)

    Q_INVOKABLE BluePlayer * connect (uint index);
    Q_INVOKABLE void disconnect ();
    bool isConnected() const;

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

protected slots:
    void onInterfacesAdded(const QDBusObjectPath &path, const InterfacesMap interfaces);
    void onInterfacesRemoved(const QDBusObjectPath &path, const QStringList interfaces);

signals:
    void connectionChanged();
    void mediaPlayerAdded(BluePlayer *mediaPlayer);
    void phoneBookAdded(BluePhoneBook *phoneBook);

protected:
    // return the roles mapping to be used by QML
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void fetchObjects(QDBusInterface &manager);
    void setupAdapter(QDBusObjectPath path);
    void addDevice(QDBusObjectPath path);
    bool checkExistingDev(QDBusInterface *dev);
    void requestPhoneBookAccess(const QString &device);
    void requestPhoneBook(const QString &pbPath);
    BluePlayer *getPlayer();

    QList<QDBusInterface *> devices;
    int connected;
    QHash<int, QByteArray> roles;
    BluePhoneBook *m_phoneBook;
};

#endif // BLUECONNECT_H
