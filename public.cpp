#include <QJsonDocument>
#include "public.h"

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
