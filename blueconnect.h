#ifndef BLUECONNECT_H
#define BLUECONNECT_H

#include <QObject>

class BlueConnect : public QObject
{
    Q_OBJECT

public:
    BlueConnect();

    Q_INVOKABLE void connect (QString address);
};

#endif // BLUECONNECT_H
