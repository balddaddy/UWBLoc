#ifndef CTAGLOCALG_H
#define CTAGLOCALG_H

#include "public.h"

class cTagLocAlg
{
public:
    cTagLocAlg();
    ~cTagLocAlg();

private:
    void locEstByMatrix(const int nAnchNum, const COORD_XYZ* dAnchPos, const double* dTagRang, COORD_XYZ *dTagPos);
    void locEstByLSE(const int nAnchNum, const COORD_XYZ* dAnchPos, const double* dTagRang, COORD_XYZ *dTagPos);
public:
    COORD_XYZ tagLoc(TAG_ANCHOR_DATA data, int tagID, int algID = 0);
};

#endif // CTAGLOCALG_H
