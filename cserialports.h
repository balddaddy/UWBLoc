#ifndef CSERIALPORTS_H
#define CSERIALPORTS_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QList>

class CSerialPorts
{
public:
    CSerialPorts(void);
    ~CSerialPorts(void);

private:
    QList<QSerialPortInfo> m_port_info;
    QStringList m_ports_list;
    QSerialPort* m_deviceports;

    QList<QByteArray> m_cmd_list;

    QString m_connct_version;
    QString m_connct_config;
    bool m_is_proccing_data;

private:
    QStringList  listPorts();
    QSerialPort* allPorts();
    void findSerialDevices(void); // find all tags and BSs
    int  openSerialDevices(QSerialPortInfo device);
    void closePort(void);
    void readData(void);
    void writeData(const QByteArray &data);
    void handleError(QSerialPort::SerialPortError error);
public:
    void testDevices(void);
};

#endif // CSERIALPORTS_H
