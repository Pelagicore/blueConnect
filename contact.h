/*
 * Copyright (C) 2016, Pelagicore AB.
 *
 * Author: Jonatan Pålsson <jonatan.palsson@pelagicore.com>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CONTACTSTRUCT_H
#define CONTACTSTRUCT_H

#include <QDBusArgument>


class Contact {
public:
    QString fileName() const;
    QString personName() const;
    QString phoneNumber() const;

private:
    friend QDBusArgument &operator<<(QDBusArgument &argument, const Contact& cs);
    friend const QDBusArgument &operator>>(const QDBusArgument &argument, Contact &cs);

    QString m_fileName;
    QString m_personName;
    QString m_phoneNumber;
};
QDBusArgument &operator<<(QDBusArgument &argument, const Contact& cs);
const QDBusArgument &operator>>(const QDBusArgument &argument, Contact &cs);
#endif // CONTACTSTRUCT_H
