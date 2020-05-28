#include <QMutexLocker>
#include <iostream>
#include <QDebug>
#include "cprocrawdata.h"

using namespace std;

cProcRawData::cProcRawData():
    m_threadStatus(_THREAD_STATUS_STOP),
    m_tagHead_raw(_RAWDATA_TAG_HEAD_RAW),
    m_tagHead_loc(_RAWDATA_TAG_HEAD_LOC),
    m_anchHead(_RAWDATA_ANCH_HEAD),
    m_handleDataFun(nullptr),
    m_tcpServer(nullptr)
{
    m_rawData.clear();
    m_tagLocer = new cTagLocAlg;
    m_tagTracker = new cTagTrackAlg;
    connect(this, SIGNAL(workReady()), this, SLOT(doWorks()));
}

cProcRawData::~cProcRawData()
{
    m_threadStatus = _THREAD_STATUS_STOP;
    while(this->getThreadStatus()!= _THREAD_STATUS_END)
    {
        // wait untill the thread is ended.
    }
    m_processedData.destroy();
    m_tagHead_raw.clear(); m_tagHead_loc.clear(); m_anchHead.clear();
    m_rawData.clear();
    delete m_tagLocer;
    delete m_tagTracker;
}

DATA_TYPE cProcRawData::judgeDataType(const QByteArray &data, int &index)
{
    int nTagHeadIndex_raw, nTagHeadIndex_loc, nAnchHeadIndex;
    nTagHeadIndex_raw = nTagHeadIndex_loc = nAnchHeadIndex = 0;
    nTagHeadIndex_raw  = data.indexOf(m_tagHead_raw, nTagHeadIndex_raw);
    nTagHeadIndex_loc  = data.indexOf(m_tagHead_loc, nTagHeadIndex_loc);
    nAnchHeadIndex     = data.indexOf(m_anchHead, nAnchHeadIndex);
    if((nTagHeadIndex_raw == -1) && (nTagHeadIndex_loc == -1) && (nAnchHeadIndex == -1))
    {
        index = -1;
        return _DATA_TYPE_UNDEFINED;
    }
    if (nTagHeadIndex_raw == -1) nTagHeadIndex_raw = _MAX_INT_NUM;
    if (nTagHeadIndex_loc == -1) nTagHeadIndex_loc = _MAX_INT_NUM;
    if (nAnchHeadIndex == -1)    nAnchHeadIndex    = _MAX_INT_NUM;
    if ((nTagHeadIndex_raw < nTagHeadIndex_loc) && (nTagHeadIndex_raw < nAnchHeadIndex))
    {
        index = nTagHeadIndex_raw;
        return  _DATA_TYPE_TAG_RAW;
    }
    else if (nTagHeadIndex_loc < nAnchHeadIndex)
    {
        index = nTagHeadIndex_loc;
        return  _DATA_TYPE_TAG_LOC;
    }
    else
    {
        index = nAnchHeadIndex;
        return _DATA_TYPE_ANCHOR;
    }
}

ERROR_CODE cProcRawData::cutRawData(QByteArray &output, DATA_TYPE &dataType)
{
    int index = -1;
    ERROR_CODE err_code;
    output.clear();

    m_mutex_rawData.lock();

    if (m_rawData.isEmpty())
    {
        err_code = _ERROR_CODE_FAIL;
        dataType = _DATA_TYPE_UNDEFINED;
        if (getPrintStatus())
            qDebug() << "No raw data to be processed yet." << endl;
        return  err_code;
    }

    dataType = judgeDataType(m_rawData, index);

    switch (dataType) {
    case _DATA_TYPE_ANCHOR:
        output = m_rawData.mid(index, _RAW_DATA_PIECE_LEN);
        m_rawData.remove(0, index + _RAW_DATA_PIECE_LEN);
        if (output.length() < _RAW_DATA_PIECE_LEN)
            err_code = _ERROR_CODE_FAIL;
        else
            err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_TAG_RAW:
        output = m_rawData.mid(index, _RAW_DATA_PIECE_LEN);
        m_rawData.remove(0, index + _RAW_DATA_PIECE_LEN);
        if (output.length() < _RAW_DATA_PIECE_LEN)
            err_code = _ERROR_CODE_FAIL;
        else
            err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_TAG_LOC:
        output = m_rawData.mid(index, _RAW_DATA_PIECE_LEN);
        m_rawData.remove(0, index + _RAW_DATA_PIECE_LEN);
        if (output.length() < _RAW_DATA_PIECE_LEN)
            err_code = _ERROR_CODE_FAIL;
        else
            err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_UNDEFINED:
        m_rawData.clear();
        output.clear();
        err_code = _ERROR_CODE_FAIL;
        break;
    default:
        break;
    }

    m_mutex_rawData.unlock();
    return err_code;
}

ERROR_CODE cProcRawData::parseRawData(const QByteArray &data, const DATA_TYPE &dataType, int &nTagID)
{
    ERROR_CODE err_code;
    bool ok;
    int nTmpIndex;
    char tmpData;
    QByteArray tmpDataArray;
    int nRang0, nRang1, nRang2, nRang3;
    switch (dataType) {
    case _DATA_TYPE_TAG_RAW:
        nTmpIndex = data.indexOf(":");
        tmpData = data.at(nTmpIndex-1);
        nTagID = tmpData - '0';
        tmpDataArray.clear();
        tmpDataArray = data.mid(6, 8);
        nRang0 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(15, 8);
        nRang1 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(24, 8);
        nRang2 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(33, 8);
        nRang3 = tmpDataArray.toInt(&ok,16);
        m_processedData.setRange(nTagID, 0, nRang0);
        m_processedData.setRange(nTagID, 1, nRang1);
        m_processedData.setRange(nTagID, 2, nRang2);
        m_processedData.setRange(nTagID, 3, nRang3);
        if (getPrintStatus())
            qDebug() << nTagID << "\t" << nRang0 << "\t" << nRang1 << "\t" << nRang2 << "\t" << nRang3 << endl;
        err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_TAG_LOC:
        nTmpIndex = data.indexOf(":");
        tmpData = data.at(nTmpIndex-1);
        nTagID = tmpData - '0';
        tmpDataArray.clear();
        tmpDataArray = data.mid(6, 8);
        nRang0 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(15, 8);
        nRang1 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(24, 8);
        nRang2 = tmpDataArray.toInt(&ok,16);
        tmpDataArray.clear();
        tmpDataArray = data.mid(33, 8);
        nRang3 = tmpDataArray.toInt(&ok,16);
        m_processedData.setRange(nTagID, 0, nRang0);
        m_processedData.setRange(nTagID, 1, nRang1);
        m_processedData.setRange(nTagID, 2, nRang2);
        m_processedData.setRange(nTagID, 3, nRang3);
        if (getPrintStatus())
            qDebug() << nTagID << "\t" << nRang0 << "\t" << nRang1 << "\t" << nRang2 << "\t" << nRang3 << endl;
        err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_ANCHOR:
        err_code = _ERROR_CODE_SUCC;
        break;
    case _DATA_TYPE_UNDEFINED:
        err_code = _ERROR_CODE_FAIL;
        break;
    default:
        err_code = _ERROR_CODE_FAIL;
        break;
    }
    return err_code;
}

void cProcRawData::setThreadStatus(THREAD_STATUS &status)
{
    m_mutex_threadStaus.lock();
    m_threadStatus = status;
    m_mutex_threadStaus.unlock();
}

THREAD_STATUS cProcRawData::getThreadStatus(void)
{
    THREAD_STATUS status;
    m_mutex_threadStaus.lock();
    status = m_threadStatus;
    m_mutex_threadStaus.unlock();
    return status;
}

ERROR_CODE cProcRawData::initialize(const int nTagNum, const int nAnchNum, const COORD_XYZ *anchXYZ)
{
    ERROR_CODE err_code;
    THREAD_STATUS status = _THREAD_STATUS_STOP;
    this->setThreadStatus(status);
    while (this->getThreadStatus() != _THREAD_STATUS_END)
    {
        // wait untill the thread is ended.
    }

    m_mutex_rawData.lock();
    m_rawData.clear();
    m_mutex_rawData.unlock();
    if (m_processedData.dRangArray != nullptr)
        m_processedData.destroy();

    if ((nTagNum < 1) || (nAnchNum < 1))
        err_code = _ERROR_CODE_FAIL;
//    m_rawData.append("mc 01 000009c5 00000000 00000000 00000000 0113 b0 00013ca0 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0113 b0 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 0114 b1 00013db8 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 0114 b1 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 0115 b2 00013ed0 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0115 b2 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 0116 b3 00013fe8 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 0116 b3 40224022 a4:0\r\nmc 01 000009c0 00000000 00000000 00000000 0117 b4 00014100 a4:0\r\nmr 01 00000916 00000000 00000000 00000000 0117 b4 40224022 a4:0\r\nmc 01 000009b7 00000000 00000000 00000000 0118 b5 00014218 a4:0\r\nmr 01 0000090d 00000000 00000000 00000000 0118 b5 40224022 a4:0\r\nmc 01 000009d7 00000000 00000000 00000000 0119 b6 00014330 a4:0\r\nmr 01 0000092d 00000000 00000000 00000000 0119 b6 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 011a b7 00014448 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 011a b7 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 011b b8 00014560 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 011b b8 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 011c b9 00014678 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 011c b9 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 011d ba 00014790 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 011d ba 40224022 a4:0\r\nmc 01 000009bb 00000000 00000000 00000000 011e bb 000148a8 a4:0\r\nmr 01 00000911 00000000 00000000 00000000 011e bb 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 011f bc 000149c0 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 011f bc 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0120 bd 00014ad8 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0120 bd 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0121 be 00014bf0 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0121 be 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 0122 bf 00014d08 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 0122 bf 40224022 a4:0\r\nmc 01 000009dc 00000000 00000000 00000000 0123 c0 00014e20 a4:0\r\nmr 01 00000932 00000000 00000000 00000000 0123 c0 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0124 c1 00014f38 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0124 c1 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0125 c2 00015050 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0125 c2 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0126 c3 00015168 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0126 c3 40224022 a4:0\r\nmc 01 000009d7 00000000 00000000 00000000 0127 c4 00015280 a4:0\r\nmr 01 0000092d 00000000 00000000 00000000 0127 c4 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 0128 c5 00015398 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 0128 c5 40224022 a4:0\r\nmc 01 000009d7 00000000 00000000 00000000 0129 c6 000154b0 a4:0\r\nmr 01 0000092d 00000000 00000000 00000000 0129 c6 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 012a c7 000155c8 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 012a c7 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 012b c8 000156e0 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 012b c8 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 012c c9 000157f8 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 012c c9 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 012d ca 00015910 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 012d ca 40224022 a4:0\r\nmc 01 000009c0 00000000 00000000 00000000 012e cb 00015a28 a4:0\r\nmr 01 00000916 00000000 00000000 00000000 012e cb 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 012f cc 00015b40 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 012f cc 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 0130 cd 00015c58 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0130 cd 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0131 ce 00015d70 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0131 ce 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0132 cf 00015e88 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0132 cf 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0133 d0 00015fa0 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0133 d0 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 0134 d1 000160b8 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 0134 d1 40224022 a4:0\r\nmc 01 000009c9 00000000 00000000 00000000 0135 d2 000161d0 a4:0\r\nmr 01 0000091f 00000000 00000000 00000000 0135 d2 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 0136 d3 000162e8 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0136 d3 40224022 a4:0\r\nmc 01 000009c9 00000000 00000000 00000000 0137 d4 00016400 a4:0\r\nmr 01 0000091f 00000000 00000000 00000000 0137 d4 40224022 a4:0\r\nmc 01 000009dc 00000000 00000000 00000000 0138 d5 00016518 a4:0\r\nmr 01 00000932 00000000 00000000 00000000 0138 d5 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 0139 d6 00016630 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 0139 d6 40224022 a4:0\r\nmc 01 000009bb 00000000 00000000 00000000 013a d7 00016748 a4:0\r\nmr 01 00000911 00000000 00000000 00000000 013a d7 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 013b d8 00016860 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 013b d8 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 013c d9 00016978 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 013c d9 40224022 a4:0\r\nmc 01 000009b2 00000000 00000000 00000000 013d da 00016a90 a4:0\r\nmr 01 00000908 00000000 00000000 00000000 013d da 40224022 a4:0\r\nmc 01 000009b2 00000000 00000000 00000000 013e db 00016ba8 a4:0\r\nmr 01 00000908 00000000 00000000 00000000 013e db 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 013f dc 00016cc0 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 013f dc 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0140 dd 00016dd8 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0140 dd 40224022 a4:0\r\nmc 01 000009d7 00000000 00000000 00000000 0141 de 00016ef0 a4:0\r\nmr 01 0000092d 00000000 00000000 00000000 0141 de 40224022 a4:0\r\nmc 01 000009c9 00000000 00000000 00000000 0142 df 00017008 a4:0\r\nmr 01 0000091f 00000000 00000000 00000000 0142 df 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0143 e0 00017120 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0143 e0 40224022 a4:0\r\nmc 01 000009c0 00000000 00000000 00000000 0144 e1 00017238 a4:0\r\nmr 01 00000916 00000000 00000000 00000000 0144 e1 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0145 e2 00017350 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0145 e2 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 0146 e3 00017468 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0146 e3 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 0147 e4 00017580 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 0147 e4 40224022 a4:0\r\nmc 01 000009c5 00000000 00000000 00000000 0148 e5 00017698 a4:0\r\nmr 01 0000091b 00000000 00000000 00000000 0148 e5 40224022 a4:0\r\nmc 01 000009c9 00000000 00000000 00000000 0149 e6 000177b0 a4:0\r\nmr 01 0000091f 00000000 00000000 00000000 0149 e6 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 014a e7 000178c8 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 014a e7 40224022 a4:0\r\nmc 01 000009ce 00000000 00000000 00000000 014b e8 000179e0 a4:0\r\nmr 01 00000924 00000000 00000000 00000000 014b e8 40224022 a4:0\r\nmc 01 000009dc 00000000 00000000 00000000 014c e9 00017af8 a4:0\r\nmr 01 00000932 00000000 00000000 00000000 014c e9 40224022 a4:0\r\nmc 01 00000a22 00000000 00000000 00000000 014d ea 00017c10 a4:0\r\nmr 01 00000978 00000000 00000000 00000000 014d ea 40224022 a4:0\r\nmc 01 00000a43 00000000 00000000 00000000 014e eb 00017d28 a4:0\r\nmr 01 00000999 00000000 00000000 00000000 014e eb 40224022 a4:0\r\nmc 01 00000a02 00000000 00000000 00000000 014f ec 00017e40 a4:0\r\nmr 01 00000958 00000000 00000000 00000000 014f ec 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0150 ed 00017f58 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0150 ed 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 0151 ee 00018070 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 0151 ee 40224022 a4:0\r\nmc 01 000009f8 00000000 00000000 00000000 0152 ef 00018188 a4:0\r\nmr 01 0000094e 00000000 00000000 00000000 0152 ef 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0153 f0 000182a0 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0153 f0 40224022 a4:0\r\nmc 01 000009fd 00000000 00000000 00000000 0154 f1 000183b8 a4:0\r\nmr 01 00000953 00000000 00000000 00000000 0154 f1 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0155 f2 000184d0 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0155 f2 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0156 f3 000185e8 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0156 f3 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0157 f4 00018700 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0157 f4 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 0158 f5 00018818 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 0158 f5 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0159 f6 00018930 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0159 f6 40224022 a4:0\r\nmc 01 000009ef 00000000 00000000 00000000 015a f7 00018a48 a4:0\r\nmr 01 00000945 00000000 00000000 00000000 015a f7 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 015b f8 00018b60 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 015b f8 40224022 a4:0\r\nmc 01 000009fd 00000000 00000000 00000000 015c f9 00018c78 a4:0\r\nmr 01 00000953 00000000 00000000 00000000 015c f9 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 015d fa 00018d90 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 015d fa 40224022 a4:0\r\nmc 01 000009ef 00000000 00000000 00000000 015e fb 00018ea8 a4:0\r\nmr 01 00000945 00000000 00000000 00000000 015e fb 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 015f fc 00018fc0 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 015f fc 40224022 a4:0\r\nmc 01 000009ef 00000000 00000000 00000000 0160 fd 000190d8 a4:0\r\nmr 01 00000945 00000000 00000000 00000000 0160 fd 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0161 fe 000191f0 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0161 fe 40224022 a4:0\r\nmc 01 000009d7 00000000 00000000 00000000 0162 ff 00019308 a4:0\r\nmr 01 0000092d 00000000 00000000 00000000 0162 ff 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0163 00 00019420 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0163 00 40224022 a4:0\r\nmc 01 000009f8 00000000 00000000 00000000 0164 01 00019538 a4:0\r\nmr 01 0000094e 00000000 00000000 00000000 0164 01 40224022 a4:0\r\nmc 01 000009f8 00000000 00000000 00000000 0165 02 00019650 a4:0\r\nmr 01 0000094e 00000000 00000000 00000000 0165 02 40224022 a4:0\r\nmc 01 00000a02 00000000 00000000 00000000 0166 03 00019768 a4:0\r\nmr 01 00000958 00000000 00000000 00000000 0166 03 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0167 04 00019880 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0167 04 40224022 a4:0\r\nmc 01 000009ef 00000000 00000000 00000000 0168 05 00019998 a4:0\r\nmr 01 00000945 00000000 00000000 00000000 0168 05 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 0169 06 00019ab0 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 0169 06 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 016a 07 00019bc8 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 016a 07 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 016b 08 00019ce0 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 016b 08 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 016c 09 00019df8 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 016c 09 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 016d 0a 00019f10 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 016d 0a 40224022 a4:0\r\nmc 01 000009fd 00000000 00000000 00000000 016e 0b 0001a028 a4:0\r\nmr 01 00000953 00000000 00000000 00000000 016e 0b 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 016f 0c 0001a140 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 016f 0c 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0170 0d 0001a258 a4:0\r\nmr 01 0000093b 00000000 00000000 00000000 0170 0d 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 0171 0e 0001a370 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 0171 0e 40224022 a4:0\r\nmc 01 00000a02 00000000 00000000 00000000 0172 0f 0001a488 a4:0\r\nmr 01 00000958 00000000 00000000 00000000 0172 0f 40224022 a4:0\r\nmc 01 000009f8 00000000 00000000 00000000 0173 10 0001a5a0 a4:0\r\nmr 01 0000094e 00000000 00000000 00000000 0173 10 40224022 a4:0\r\nmc 01 000009f4 00000000 00000000 00000000 0174 11 0001a6b8 a4:0\r\nmr 01 0000094a 00000000 00000000 00000000 0174 11 40224022 a4:0\r\nmc 01 000009d3 00000000 00000000 00000000 0175 12 0001a7d0 a4:0\r\nmr 01 00000929 00000000 00000000 00000000 0175 12 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0176 13 0001a8e8 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0176 13 40224022 a4:0\r\nmc 01 000009f8 00000000 00000000 00000000 0177 14 0001aa00 a4:0\r\nmr 01 0000094e 00000000 00000000 00000000 0177 14 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 0178 15 0001ab18 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 0178 15 40224022 a4:0\r\nmc 01 000009e1 00000000 00000000 00000000 0179 16 0001ac30 a4:0\r\nmr 01 00000937 00000000 00000000 00000000 0179 16 40224022 a4:0\r\nmc 01 000009ea 00000000 00000000 00000000 017a 17 0001ad48 a4:0\r\nmr 01 00000940 00000000 00000000 00000000 017a 17 40224022 a4:0\r\nmc 01 000009fd 00000000 00000000 00000000 017b 18 0001ae60 a4:0\r\nmr 01 00000953 00000000 00000000 00000000 017b 18 40224022 a4:0\r\nmc 01 000009e5 00000000 00000000 00000000 0");
    else{
        m_processedData.initial(nTagNum, nAnchNum);
        for (int id = 0; id < nAnchNum; id++)
        {
            m_processedData.setAnchXYZ(id, anchXYZ[id]);
        }
        err_code = _ERROR_CODE_SUCC;
    }
    if (err_code == _ERROR_CODE_SUCC)
    {
        status = _THREAD_STATUS_WORKING;
        this->setThreadStatus(status);
        emit workReady();
    }
    return  err_code;
}

ERROR_CODE cProcRawData::pauseThread(void)
{
    ERROR_CODE err_code;
    if (this->getThreadStatus() == _THREAD_STATUS_WORKING)
    {
        THREAD_STATUS status = _THREAD_STATUS_PAUSE;
        this->setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Pause data processing thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cProcRawData::continueThread(void)
{
    ERROR_CODE err_code;
    if (this->getThreadStatus() == _THREAD_STATUS_PAUSE)
    {
        THREAD_STATUS status = _THREAD_STATUS_WORKING;
        this->setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
        qDebug() << "Continue data processing thread successfully." << endl;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cProcRawData::stopThread(void)
{
    ERROR_CODE err_code;
    THREAD_STATUS status = this->getThreadStatus();
    if ((status == _THREAD_STATUS_PAUSE) || (status == _THREAD_STATUS_WORKING))
    {
        status = _THREAD_STATUS_STOP;
        this->setThreadStatus(status);
        err_code = _ERROR_CODE_SUCC;
    }
    else
        err_code = _ERROR_CODE_FAIL;
    return err_code;
}

ERROR_CODE cProcRawData::addRawData(QByteArray rawData, cProcRawData* thisClass)
{
    ERROR_CODE err_code;
    if (rawData.isEmpty())
    {
        err_code = _ERROR_CODE_FAIL;
    }
    else{
        thisClass->m_mutex_rawData.lock();
        thisClass->m_rawData.append(rawData);
        thisClass->m_mutex_rawData.unlock();
        err_code = _ERROR_CODE_SUCC;
    }
    return err_code;
}

void cProcRawData::setHandleDataFun(ERROR_CODE (*handleDataFun)(TAG_ANCHOR_DATA&, cTCPCom*), cTCPCom* tcpPort)
{
    m_handleDataFun = handleDataFun;
    m_tcpServer = tcpPort;
}

void cProcRawData::doWorks(void)
{
    THREAD_STATUS status = this->getThreadStatus();
    int nPausePrintCount = 0;
    while (status != _THREAD_STATUS_STOP)
    {
        if (status == _THREAD_STATUS_PAUSE)
        {
            if (getPrintStatus() && (nPausePrintCount < 5))
            {
                nPausePrintCount++;
                qDebug() << "Data processing thread paused." << endl;
            }
        }
        else if (status == _THREAD_STATUS_WORKING)
        {
            nPausePrintCount = 0;
            QByteArray data;  data.clear();
            DATA_TYPE dataType;
            int nTagID; COORD_XYZ tagXYZ;
            ERROR_CODE err_code = this->cutRawData(data,dataType);
            if (err_code == _ERROR_CODE_FAIL)
                continue;
            err_code = this->parseRawData(data,dataType, nTagID);
            if (err_code == _ERROR_CODE_FAIL)
                continue;
            tagXYZ = m_tagLocer->tagLoc(m_processedData, nTagID);
            m_processedData.setTagXYZ(nTagID, tagXYZ);
            m_tagTracker->KalmFilter(m_processedData, nTagID);
            m_handleDataFun(m_processedData, m_tcpServer);
        }
        status = this->getThreadStatus();
    }
    if (getPrintStatus())
    {
        qDebug() << "Data processing thread quits." << endl;
    }
    status = _THREAD_STATUS_END;
    this->setThreadStatus(status);
}
