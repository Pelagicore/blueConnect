#include "blueconnect.h"
#include "iostream"

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

    qDBusRegisterMetaType <QMap<QString,QVariantMap > > ();
    qDBusRegisterMetaType <QMap<QDBusObjectPath,QMap<QString,QVariantMap > > > ();

    QDBusInterface manager("org.bluez",
                           "/",
                           "org.freedesktop.DBus.ObjectManager",
                           QDBusConnection::systemBus());
    QDBusReply<QMap<QDBusObjectPath,QMap<QString,QVariantMap > > > reply;
    reply = manager.call("GetManagedObjects");
    if (!reply.isValid()) {
        std::cout << "Failed to connect to bluez: " << reply.error().message() << std::endl;

        return;
    }

    auto objects = reply.value();

    for (auto i = objects.begin(); i != objects.end(); ++i) {
        auto ifaces = i.value();

        for (auto j = ifaces.begin(); j != ifaces.end(); ++j) {
            if (j.key() != "org.bluez.Device1")
                continue;

            QDBusInterface *dev;
            dev = new QDBusInterface ("org.bluez",
                                      i.key().path(),
                                      "org.bluez.Device1",
                                      QDBusConnection::systemBus());
            auto uuid = getAudioSourceUUID(dev);
            if (uuid != Q_NULLPTR)
                devices << dev;
        }
    }
}

void BlueConnect::connect (uint index)
{
    if (connected != Q_NULLPTR)
        return;

    auto dev = devices[index];
    auto address = dev->property("Address").toString();
    std::cout << "will connect to:" << address << std::endl;

    auto uuid = getAudioSourceUUID(dev);
    if (uuid == Q_NULLPTR)
        return;

    if (dev->property("Paired").toBool()) {
        std::cout << "Device already paired" << std::endl;
    } else {
        QDBusReply<void> reply = dev->call("Pair");
        if (!reply.isValid()) {
            std::cout << "Failed to pair: " << reply.error().message() << std::endl;

            return;
        }
    }

    QDBusReply<void> reply = dev->call("ConnectProfile", uuid);
    if (!reply.isValid()) {
        std::cout << "Failed to connect: " << reply.error().message() << std::endl;

        return;
    }
    connected = dev;
}

void BlueConnect::disconnect ()
{
    if (connected == Q_NULLPTR)
        return;

    auto name = connected->property("Name").toString();
    std::cout << "Disconnecting " << name << std::endl;

    QDBusReply<void> reply = connected->call("Disconnect");
    if (!reply.isValid()) {
        std::cout << "Failed to disconnect: " << reply.error().message() << std::endl;

        return;
    }
    connected = Q_NULLPTR;
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
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BlueConnect::roleNames() const
{
    return roles;
}
