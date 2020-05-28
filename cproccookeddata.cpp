#include "cproccookeddata.h"

cProcCookedData::cProcCookedData(void)
{
    connect(this, SIGNAL(workReady()), this, SLOT(doWorks()));
}

cProcCookedData::~cProcCookedData(void)
{

}

ERROR_CODE cProcCookedData::procCookedData(QByteArray data, cProcCookedData* procDevice)
{
    ERROR_CODE err = _ERROR_CODE_SUCC;
    return err;
}

void cProcCookedData::doWorks(void)
{
    return;
}
