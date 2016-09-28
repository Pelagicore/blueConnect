#ifndef BLUECONNECT_H
#define BLUECONNECT_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtDBus/QtDBus>

typedef QMap<QString,QVariantMap> InterfacesMap;
typedef QList<QString> InterfacesList;
typedef QMap<QDBusObjectPath,InterfacesMap> ObjectsMap;

Q_DECLARE_METATYPE(InterfacesMap)
Q_DECLARE_METATYPE(ObjectsMap)

class BlueConnect : public QAbstractListModel
{
    Q_OBJECT

public:
    enum RoleNames {
        AddressRole = Qt::UserRole,
        NameRole = Qt::UserRole + 1,
    };

    explicit BlueConnect(QObject *parent=0);
    ~BlueConnect();

    Q_PROPERTY(bool isConnected READ isConnected NOTIFY connectionChanged)

    Q_INVOKABLE void connect (uint index);
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

protected:
    // return the roles mapping to be used by QML
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    void fetchDevices(void);
    void addDevice(QDBusObjectPath path);
    bool checkExistingDev(QDBusInterface *dev);

    QList<QDBusInterface *> devices;
    QDBusInterface *manager;
    int connected;
    QHash<int, QByteArray> roles;
};

#endif // BLUECONNECT_H
