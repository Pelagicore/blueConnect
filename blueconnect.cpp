#include "blueconnect.h"
#include "iostream"

std::ostream& operator<<(std::ostream& str, const QString& string) {
    return str << string.toStdString();
}

QString getAudioSourceUUID(QDBusInterface *dev)
{
    QStringList uuids = dev->property("UUIDs").toStringList();

    QStringList::iterator k;
    for (k = uuids.begin(); k != uuids.end(); ++k) {
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

    QMap<QDBusObjectPath,QMap<QString,QVariantMap> > objects = reply.value();
    QMap<QDBusObjectPath,QMap<QString,QVariantMap> >::iterator i;

    for (i = objects.begin(); i != objects.end(); ++i) {
        QMap<QString,QVariantMap> ifaces = i.value();
        QMap<QString,QVariantMap>::iterator j;

        for (j = ifaces.begin(); j != ifaces.end(); ++j) {
            if (j.key() != "org.bluez.Device1")
                continue;

            QDBusInterface *dev;
            dev = new QDBusInterface ("org.bluez",
                                      i.key().path(),
                                      "org.bluez.Device1",
                                      QDBusConnection::systemBus());
            QString uuid = getAudioSourceUUID(dev);
            if (uuid != Q_NULLPTR)
                devices << dev;
        }
    }
}

BlueConnect::~BlueConnect()
{
}

void BlueConnect::connect (uint index)
{
    QDBusInterface *dev = devices[index];
    QString address = dev->property("Address").toString();
    std::cout << "will connect to:" << address << std::endl;

    QString uuid = getAudioSourceUUID(dev);
    if (uuid != Q_NULLPTR)
        dev->call("ConnectProfile", uuid);
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
        return devices[0]->property("Name");
    case AddressRole:
        return devices[0]->property("Address");
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> BlueConnect::roleNames() const
{
    return roles;
}
