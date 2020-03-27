#include <QCoreApplication>
#include <QDebug>
#include "cserialports.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug() << "This is a demo to test UWB localization System.\n"
             << "Author: CharleyChang    Data: 2020. 03. 25\n";

    qDebug() << "Program Started\n";
    CSerialPorts *device = new CSerialPorts;
    device->testDevices();
    return a.exec();
}
