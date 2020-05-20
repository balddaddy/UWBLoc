#ifndef PUBLIC_H
#define PUBLIC_H

#include <string.h>

typedef enum _ERROR_CODE {
    _ERROR_CODE_SUCC = 0,
    _ERROR_CODE_FAIL,
    _ERROR_CODE_TIMEOUT
} ERROR_CODE;

typedef enum _THREAD_STATUS{
    _THREAD_STATUS_WORKING,
    _THREAD_STATUS_PAUSE,
    _THREAD_STATUS_STOP,
} THREAD_STATUS;

typedef enum _DATA_TYPE {
    _DATA_TYPE_TAG_RAW,
    _DATA_TYPE_TAG_LOC,
    _DATA_TYPE_ANCHOR,
    _DATA_TYPE_UNDEFINED
} DATA_TYPE;

typedef struct _COORD_XYZ{
    double dx;
    double dy;
    double dz;
} COORD_XYZ;

typedef  struct _TAG_ANCHOR_DATA {
    int nTagNum;
    int nAnchNum;
    COORD_XYZ *anchXYZ;
    double** dRangArray;
    double** dTime;
    int*  nTrackID;
    COORD_XYZ **tagXYZ;
    _TAG_ANCHOR_DATA(void){
        nTagNum = 0;
        nAnchNum = 0;
        anchXYZ = nullptr;
        dRangArray = nullptr;
        dTime = nullptr;
        nTrackID = nullptr;
        tagXYZ = nullptr;
    };
    void initial(const int _nTag_Num, const int _nAnch_Num){
        nTagNum = _nTag_Num;
        nAnchNum = _nAnch_Num;
        anchXYZ = new COORD_XYZ[nAnchNum];
        memset(anchXYZ, 0, sizeof(double)*nAnchNum);
        tagXYZ = new COORD_XYZ*[nTagNum];
        dTime = new double*[nTagNum];
        nTrackID = new int[nTagNum];
        for (int nid = 0; nid < nTagNum; nid++)
        {
            tagXYZ[nid] = new COORD_XYZ[10];
            memset(tagXYZ[nid], 0, sizeof(COORD_XYZ)*10);
            dTime[nid] = new double[11];
            memset(dTime[nid], 0, sizeof(double)*10);
            nTrackID[nid] = 1;
        }
        memset(tagXYZ, 0, sizeof(double)*nTagNum);
        dRangArray = new double*[nAnchNum];
        for (int nid = 0; nid < nAnchNum; nid++)
        {
            dRangArray[nid] = new double[nTagNum];
            memset(dRangArray[nid], 0, sizeof(double)*nTagNum);
        }
    }
    void setAnchXYZ(const int _anch_id, const COORD_XYZ _xyz)
    {
        if ((_anch_id < 0) || (_anch_id >= nAnchNum))
                return;
        anchXYZ[_anch_id] = _xyz;
    }
    COORD_XYZ getAnchXYZ(const int _anch_id)
    {
        COORD_XYZ tmpXYZ;
        tmpXYZ.dx = tmpXYZ.dy = tmpXYZ.dz = 0.0;
        if ((_anch_id < 0) || (_anch_id >= nAnchNum))
                return tmpXYZ;
        tmpXYZ = anchXYZ[_anch_id];
        return tmpXYZ;
    }
    void setTagXYZ(const int _tag_id, const COORD_XYZ _xyz)
    {
        if ((_tag_id < 0) || (_tag_id >= nTagNum))
                return;
        tagXYZ[_tag_id][nTrackID[_tag_id]] = _xyz;
        nTrackID[_tag_id]++;
        if (nTrackID[_tag_id] > 10)
            nTrackID[_tag_id] = 1;
    }
    COORD_XYZ getTagXYZ(const int _tag_id)
    {
        COORD_XYZ tmpXYZ;
        tmpXYZ.dx = tmpXYZ.dy = tmpXYZ.dz = 0.0;
        if ((_tag_id < 0) || (_tag_id >= nTagNum))
                return tmpXYZ;
        tmpXYZ = tagXYZ[_tag_id][0];
        return tmpXYZ;
    }
    void setRange(const int _tag_id, const int _anch_id, const double _rang)
    {
        if ((_tag_id < 0) || (_tag_id >= nTagNum) || (_anch_id < 0) || (_anch_id >= nAnchNum))
                return;
        dRangArray[_anch_id][_tag_id] = _rang;
    }
    double getRange(const int _tag_id, const int _anch_id)
    {
        if ((_tag_id < 0) || (_tag_id >= nTagNum) || (_anch_id < 0) || (_anch_id >= nAnchNum))
                return -1.0;
        return dRangArray[_anch_id][_tag_id];
    }
    void destroy(void)
    {
        delete [] anchXYZ;
        anchXYZ = nullptr;
        for (int nid = 0; nid < nAnchNum; nid++)
        {
            delete[] dRangArray[nid];
            dRangArray = nullptr;
        }
        delete [] dRangArray;
        dRangArray = nullptr;
        delete [] nTrackID;
        for (int nid = 0; nid < nTagNum; nid++)
        {
            delete [] dTime[nid];
            delete [] tagXYZ[nid];
        }
        delete [] dTime;
        delete [] tagXYZ;
        nTagNum = 0; nAnchNum = 0;
    }
} TAG_ANCHOR_DATA;

#define _MAX_TAG_NUM 1000
#define _MAX_ANCHOR_NUM 100
#define _RAW_DATA_PIECE_LEN  65

#define DEVICE_NAME_WIN ("STMicroelectronics Virtual COM Port")
#define DEVICE_NAME_LNX ("Virtual COM Port")
#define DEVICE_NAME_MAC ("Virtual COM Port")

#endif // PUBLIC_H
