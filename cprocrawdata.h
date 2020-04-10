#ifndef CPROCRAWDATA_H
#define CPROCRAWDATA_H

#include <QObject>
#include <QByteArray>
#include <QMutex>

typedef enum _ERROR_CODE_DATARPOC {
    _ERROR_CODE_DATARPOC_SUCC = 0,
    _ERROR_CODE_DATARPOC_FAIL,
    _ERROR_CODE_DATARPOC_UNDEFINE,
    _ERROR_CODE_DATARPOC_TIMEOUT
} ERROR_CODE_DATARPOC;

typedef enum _DATA_TYPE {
    _DATA_TYPE_TAG_RAW,
    _DATA_TYPE_TAG_LOC,
    _DATA_TYPE_ANCHOR,
    _DATA_TYPE_UNDEFINED
} DATA_TYPE;

#define _MAX_TAG_NUM 1000
#define _MAX_ANCHOR_NUM 100
#define _RAW_DATA_PIECE_LEN  65

extern QMutex g_rawDataMutex;

typedef  struct _TAG_ANCHOR_DATA {
    int nTagNum;
    int nAnchNum;
    double** dRangArray;
    _TAG_ANCHOR_DATA(void){
        nTagNum = 0;
        nAnchNum = 0;
        dRangArray = nullptr;
    };
    void initial(const int _nTag_Num, const int _nAnch_Num){
        nTagNum = _nTag_Num;
        nAnchNum = _nAnch_Num;
        dRangArray = new double*[nAnchNum];
        for (int nid = 0; nid < nAnchNum; nid++)
        {
            dRangArray[nid] = new double[nTagNum];
        }
    }
    void set(const int _tag_id, const int _anch_id, const double _rang)
    {
        if ((_tag_id < 0) || (_tag_id >= nTagNum) || (_anch_id < 0) || (_anch_id >= nAnchNum))
                return;
        dRangArray[_anch_id][_tag_id] = _rang;
    }
    double get(const int _tag_id, const int _anch_id)
    {
        if ((_tag_id < 0) || (_tag_id >= nTagNum) || (_anch_id < 0) || (_anch_id >= nAnchNum))
                return -1.0;
        return dRangArray[_anch_id][_tag_id];
    }
    void destroy(void)
    {
        for (int nid = 0; nid < nAnchNum; nid++)
        {
            delete[] dRangArray[nid];
            dRangArray = nullptr;
        }
        delete [] dRangArray;
        dRangArray = nullptr;
        nTagNum = 0; nAnchNum = 0;
    }
} TAG_ANCHOR_DATA;

class cProcRawData : public QObject
{
    Q_OBJECT
public:
    cProcRawData();
    ~cProcRawData();
    void init(const int nTNum, const int nANum);

private:
    bool m_isProcessing;
    QByteArray m_tagHead_raw, m_tagHead_loc, m_anchHead;
    TAG_ANCHOR_DATA m_processedData;
    QByteArray m_rawData;

public:
    void inputRawData(QByteArray rawData);
private:
    ERROR_CODE_DATARPOC outputRawData(QByteArray& output, DATA_TYPE &dataType);
    DATA_TYPE judgeDataType(QByteArray& data, int &index);

public slots:
    void processRawData(void);
};

#endif // CPROCRAWDATA_H
