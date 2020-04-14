#include "string.h"
#include "ctaglocalg.h"
#include "ccmath.h"

cTagLocAlg::cTagLocAlg()
{

}

cTagLocAlg::~cTagLocAlg()
{

}

void cTagLocAlg::locEstByMatrix(const int nAnchNum, const COORD_XYZ *dAnchPos, const double *dTagRang, COORD_XYZ &dTagPos)
{
    if (nAnchNum < 3)
        return;   // couldn't locate the tag
    double** A = new double*[3];
    double** D = new double*[3];
    for (int id = 0; id < 3; id++)
    {
        A[id] = new double[3];
        D[id] = new double;
        A[id][0] = 2.0*(dAnchPos[id+1].dx - dAnchPos[0].dx);
        A[id][1] = 2.0*(dAnchPos[id+1].dy - dAnchPos[0].dy);
        A[id][2] = 2.0*(dAnchPos[id+1].dz - dAnchPos[0].dz);
        D[id][0] = dTagRang[0]*dTagRang[0] - dTagRang[id+1]*dTagRang[id+1]
                + dAnchPos[id+1].dx*dAnchPos[id+1].dx - dAnchPos[0].dx*dAnchPos[0].dx
                + dAnchPos[id+1].dy*dAnchPos[id+1].dy - dAnchPos[0].dy*dAnchPos[0].dy
                + dAnchPos[id+1].dz*dAnchPos[id+1].dz - dAnchPos[0].dz*dAnchPos[0].dz;
    }

    double** A_Inv = new double*[3];
    for (int id = 0; id < 3; ++id) {
        A_Inv[id] = new double[3];
        memset(A_Inv[id],0,sizeof(double)*3);
    }
    __CCMATH__::MatrixInv(A,A_Inv,nAnchNum-1,3);

    double** Pos_Tag = new double*[3];
    for (int id = 0; id < 3; id++)
    {
        Pos_Tag[id] = new double;
        memset(Pos_Tag[id], 0, sizeof(double));
    }
    __CCMATH__::MatrixMultiply(A_Inv, D, Pos_Tag, 3, 3, 3, 1);

    dTagPos.dx = Pos_Tag[0][0];
    dTagPos.dy = Pos_Tag[1][0];
    dTagPos.dz = Pos_Tag[2][0];

    for (int id = 0; id < 3; id++)
    {
        delete[] A_Inv[id];
        A_Inv[id] = nullptr;
        delete Pos_Tag[id];
        Pos_Tag[id] = nullptr;
    }
    delete [] A_Inv; A_Inv = nullptr;
    delete [] Pos_Tag; Pos_Tag = nullptr;

    for (int id = 1; id < nAnchNum; id++)
    {
        delete [] A[id];     A[id] = nullptr;
        delete [] D[id];     D[id] = nullptr;
    }
    delete [] A; delete [] D;
    A = nullptr; D = nullptr;
}

void cTagLocAlg::locEstByLSE(const int nAnchNum, const COORD_XYZ *dAnchPos, const double *dTagRang, COORD_XYZ &dTagPos)
{
    if (nAnchNum < 3)
        return;   // couldn't locate the tag
    double** A = new double*[nAnchNum-1];
    double** D = new double*[nAnchNum-1];
    for (int id = 0; id < nAnchNum-1; id++)
    {
        A[id] = new double[3];
        D[id] = new double;
        A[id][0] = 2*(dAnchPos[id+1].dx - dAnchPos[0].dx);
        A[id][1] = 2*(dAnchPos[id+1].dy - dAnchPos[0].dy);
        A[id][2] = 2*(dAnchPos[id+1].dz - dAnchPos[0].dz);
        D[id][0] = dTagRang[0]*dTagRang[0] - dTagRang[id+1]*dTagRang[id+1]
                + dAnchPos[id+1].dx*dAnchPos[id+1].dx - dAnchPos[0].dx*dAnchPos[0].dx
                + dAnchPos[id+1].dy*dAnchPos[id+1].dy - dAnchPos[0].dy*dAnchPos[0].dy
                + dAnchPos[id+1].dz*dAnchPos[id+1].dz - dAnchPos[0].dz*dAnchPos[0].dz;
    }

    double** TransA = new double*[3];
    for (int id = 0; id < 3; ++id) {
        TransA[id] = new double[nAnchNum-1];
        memset(TransA[id],0,sizeof(double)*nAnchNum-1);
    }
    __CCMATH__::MatrixTranspose(A,TransA,nAnchNum-1,3);

    double** TransAMultiplyA = new double*[3];
    double** TransA_Multi_A_Inv = new double*[3];
    double** TransA_Multi_D = new double*[3];
    double** Pos_Tag = new double*[3];
    for (int id = 0; id < 3; id++)
    {
        TransAMultiplyA[id] = new double[3];
        memset(TransAMultiplyA[id], 0, sizeof(double)*3);
        TransA_Multi_A_Inv[id] = new double[3];
        memset(TransA_Multi_A_Inv[id], 0, sizeof(double)*3);
        TransA_Multi_D[id] = new double;
        memset(TransA_Multi_D[id], 0, sizeof(double));
        Pos_Tag[id] = new double;
        memset(Pos_Tag[id], 0, sizeof(double));
    }
    __CCMATH__::MatrixMultiply(TransA, A, TransAMultiplyA, nAnchNum - 1, 3, 3, nAnchNum - 1);
    __CCMATH__::MatrixInv(TransAMultiplyA, TransA_Multi_A_Inv, 3, 3);
    __CCMATH__::MatrixMultiply(TransA, D, TransA_Multi_D, 3, nAnchNum - 1, nAnchNum - 1, 1);
    __CCMATH__::MatrixMultiply(TransA_Multi_A_Inv, TransA_Multi_D, Pos_Tag, 3, 3, 3, 1);

    dTagPos.dx = Pos_Tag[0][0];
    dTagPos.dy = Pos_Tag[1][0];
    dTagPos.dz = Pos_Tag[2][0];

    for (int id = 0; id < 3; id++)
    {
        delete[] TransA[id];
        TransA[id] = nullptr;
        delete [] TransAMultiplyA[id];
         TransAMultiplyA[id] = nullptr;
        delete [] TransA_Multi_A_Inv[id];
        TransA_Multi_A_Inv[id] = nullptr;
        delete TransA_Multi_D[id];
        TransA_Multi_D[id] = nullptr;
        delete Pos_Tag[id];
        Pos_Tag[id] = nullptr;
    }
    delete [] TransA; TransA = nullptr;
    delete [] TransAMultiplyA; TransAMultiplyA = nullptr;
    delete [] TransA_Multi_A_Inv; TransA_Multi_A_Inv = nullptr;
    delete [] TransA_Multi_D; TransA_Multi_D = nullptr;
    delete [] Pos_Tag; Pos_Tag = nullptr;

    for (int id = 1; id < nAnchNum; id++)
    {
        delete [] A[id]; A[id] = nullptr;
        delete [] D;     D = nullptr;
    }
    delete [] A; delete [] D;
    A = nullptr; D = nullptr;
}
