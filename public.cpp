#include <QJsonDocument>
#include <QDebug>
#include "public.h"

QMutex _mutex_Print_Info_Status;
bool _b_Print_Info_Status = false;

QByteArray transJson2Bytearray(const QJsonArray &array)
{
    QByteArray byteArray;
    byteArray.clear();
    QJsonDocument document = QJsonDocument(array);
    byteArray = document.toJson();
    return  byteArray;
}

QJsonArray transBytearray2Json(const QByteArray &array)
{
    QJsonDocument document = QJsonDocument::fromJson(array);
    QJsonArray jsonArray;
    jsonArray = document.array();
    return jsonArray;
}

bool getPrintStatus(void)
{
    bool isPrint;
    _mutex_Print_Info_Status.lock();
    isPrint = _b_Print_Info_Status;
    _mutex_Print_Info_Status.unlock();
    return isPrint;
}

void switchPrintOnOff(void)
{
    _mutex_Print_Info_Status.lock();
    _b_Print_Info_Status = !_b_Print_Info_Status;
    if (_b_Print_Info_Status)
        qDebug() << "Printing information function is on" << endl;
    else
        qDebug() << "Printing information function is off" << endl;
    _mutex_Print_Info_Status.unlock();
}
