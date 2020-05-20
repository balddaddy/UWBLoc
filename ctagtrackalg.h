#ifndef CTAGTRACKALG_H
#define CTAGTRACKALG_H
#include "public.h"

class cTagTrackAlg
{
public:
    cTagTrackAlg();

    void KalmFilter(const TAG_ANCHOR_DATA data, const int nTagID);
};

#endif // CTAGTRACKALG_H
