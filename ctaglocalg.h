#ifndef CTAGLOCALG_H
#define CTAGLOCALG_H

typedef struct _COORD_XYZ{
    double dx;
    double dy;
    double dz;
} COORD_XYZ;

class cTagLocAlg
{
public:
    cTagLocAlg();
    ~cTagLocAlg();

public:
    void locEstByMatrix(const int nAnchNum, const COORD_XYZ* dAnchPos, const double* dTagRang, COORD_XYZ *dTagPos);
    void locEstByLSE(const int nAnchNum, const COORD_XYZ* dAnchPos, const double* dTagRang, COORD_XYZ *dTagPos);

};

#endif // CTAGLOCALG_H
