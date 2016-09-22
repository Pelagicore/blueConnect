#ifndef BLUECONNECT_H
#define BLUECONNECT_H

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <QtDBus/QtDBus>

//qRegisterMetaType(QMap<QDBusObjectPath,QMap<QString,QVariantMap >)

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

    Q_INVOKABLE void connect (uint index);

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

protected:
    // return the roles mapping to be used by QML
    virtual QHash<int, QByteArray> roleNames() const override;

private:
    QList<QDBusInterface *> devices;
    QHash<int, QByteArray> roles;
};

#endif // BLUECONNECT_H
