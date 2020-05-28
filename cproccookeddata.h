#ifndef CPROCCOOKEDDATA_H
#define CPROCCOOKEDDATA_H

#include <QObject>
#include <QByteArray>
#include "public.h"

class cProcCookedData : public QObject
{
    Q_OBJECT

public:
    cProcCookedData(void);
    ~cProcCookedData(void);

    static ERROR_CODE procCookedData(QByteArray data, cProcCookedData *procDevice);

public slots:
    void doWorks(void);

signals:
    void workReady(void);
};

#endif // CPROCCOOKEDDATA_H
