#include "blueconnect.h"
#include "iostream"

std::ostream& operator<<(std::ostream& str, const QString& string) {
    return str << string.toStdString();
}

BlueConnect::BlueConnect()
{
}

void BlueConnect::connect (QString address)
{
    std::cout << "will connect to:" << address << std::endl;
}
