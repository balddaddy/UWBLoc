#include <QCoreApplication>
#include <iostream>
#include <string>
#include <QString>
#include <math.h>
#include <QThread>
#include "cserialports.h"
#include "cprocrawdata.h"
#include "ctaglocalg.h"
#include "ctcpcom.h"
#include "public.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);
    cout.precision(5);

    cout << string(1000,'\n');

    cout << "+++This is the CLIENT DEMO to test UWB localization System.+++" << endl
         << "++++Author: CharleyChang    Data: 2020. 03. 25+++" << endl;

    cout << "=====Program Started=====" << endl;

    /// Step0. Initialize the program
    int nAnchNum = 0; int nTagNum = 0;
    double *dAnch_X, *dAnch_Y, *dAnch_Z;
    dAnch_X = dAnch_Y = dAnch_Z = nullptr;
    while (nTagNum < 1 || nTagNum > 8)
    {
        cout << endl;
        cout << "Please input the number of tags[1~8]:";
        cin >> nTagNum;
        if (cin.fail() || nTagNum < 1 || nTagNum > 8){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input! Please input a number between 1~8." << endl;
        }
    }
    cout << "There are " << nTagNum << " tags in the system." << endl;
    while (nAnchNum < 1 || nAnchNum > 4)
    {
        cout << endl;
        cout << "Please input the number of anchors[1~4]:";
        cin >> nAnchNum;
        if (cin.fail() || nAnchNum < 1 || nAnchNum > 4){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input! Please input a number between 1~4." << endl;
        }
    }
    cout << "There are " << nAnchNum << " anchors in the system." << endl;
    dAnch_X = new double[nAnchNum]; memset(dAnch_X,0,sizeof(double)*nAnchNum);
    dAnch_Y = new double[nAnchNum]; memset(dAnch_Y,0,sizeof(double)*nAnchNum);
    dAnch_Z = new double[nAnchNum]; memset(dAnch_Z,0,sizeof(double)*nAnchNum);
    COORD_XYZ *anchXYZ = new COORD_XYZ[nAnchNum];
    for (int nid = 0; nid < nAnchNum; nid++)
    {
        double dX = 0.0; double dY = 0.0; double dZ = 0.0;
        cout << "\nPlease input the coordination of the anchor No." << nid+1 << endl;
        cout << "X:"; cin >> dX;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input! Please input a number!" << endl;
            cout << "X:"; cin >> dX;
        }
        cout << "Y:"; cin >> dY;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input! Please input a number!" << endl;
            cout << "Y:"; cin >> dY;
        }
        cout << "Z:"; cin >> dZ;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Invalid input! Please input a number!" << endl;
            cout << "Z:"; cin >> dZ;
        }
        dAnch_X[nid] = dX;
        dAnch_Y[nid] = dY;
        dAnch_Z[nid] = dZ;
        anchXYZ[nid].dx = dX;
        anchXYZ[nid].dy = dY;
        anchXYZ[nid].dz = dZ;
    }

    /// Step1. Serial Port Thread
    CSerialPorts *device = new CSerialPorts;
    QThread *spIOThread = new QThread;
    device->moveToThread(spIOThread);
    spIOThread->start();
    ERROR_CODE err_code = device->initialize();
    cout << "Serial port IO thread starts!" << endl;

    /// Step2. TCP/IP Thread
    cTCPCom *server = new cTCPCom;
    QThread *tcpIOThread = new QThread;
    server->moveToThread(tcpIOThread);
    tcpIOThread->start();
    string ipAddr;
    cout << "Please input server's IP address:";
    getline(cin, ipAddr);
    QString ipAddrStr(ipAddr.c_str());
    int serverPort;
    cin >> serverPort;
    err_code = server->initialize(ipAddrStr, serverPort);
    if (err_code == _ERROR_CODE_SUCC)
        cout << "TCP/IP IO thread starts!" << endl;
    else
        cout << "Server connection failed!" << endl;

    /// Step 3. Process data, localize tags and Kalman Filter tracks
    cProcRawData *procData = new cProcRawData;
    QThread *procDataThread = new QThread;
    procData->moveToThread(procDataThread);
    procDataThread->start();
    err_code = procData->initialize(nTagNum,nAnchNum,anchXYZ);
    if (err_code == _ERROR_CODE_SUCC)
        cout << "TCP/IP IO thread starts!" << endl;
    else
        cout << "Server connection failed!" << endl;


//    cProcRawData* procRawData = new cProcRawData;
//    procRawData->init(1,4);
//    procRawData->processRawData();

    //    // Step3. locate tag based on ranges
    //    int nAnchNum = 4;
    //    COORD_XYZ *Pos_Anch = new COORD_XYZ[nAnchNum];
    //    int nTagNum = 4;
    //    COORD_XYZ *Pos_Tag = new COORD_XYZ[nTagNum];
    //    Pos_Anch[0].dx = 0;     Pos_Anch[0].dy = 0;      Pos_Anch[0].dz = 0;
    //    Pos_Anch[1].dx = 110;   Pos_Anch[1].dy = 10;     Pos_Anch[1].dz = 0;
    //    Pos_Anch[2].dx = 10;    Pos_Anch[2].dy = 120;    Pos_Anch[2].dz = 0;更高
    //    Pos_Anch[3].dx = 2;     Pos_Anch[3].dy = 7;      Pos_Anch[3].dz = 130;

    //    Pos_Tag[0].dx = 20; Pos_Tag[0].dy = 30; Pos_Tag[0].dz = 25;
    //    Pos_Tag[1].dx = 50; Pos_Tag[1].dy = 25; Pos_Tag[1].dz = 65;
    //    Pos_Tag[2].dx = 70; Pos_Tag[2].dy = 40; Pos_Tag[2].dz = 55;
    //    Pos_Tag[3].dx = 40; Pos_Tag[3].dy = 80; Pos_Tag[3].dz = 0;

    //    cTagLocAlg* tagLoc = new cTagLocAlg;
    //    COORD_XYZ* est_Tag_Pos = new COORD_XYZ[nTagNum];

    //    for(int id = 0; id < nTagNum; id++)
    //    {
    //        double* dRang = new double[nAnchNum];
    //        memset(dRang, 0, sizeof(double)*nAnchNum);
    //        for(int iid = 0; iid < nAnchNum; iid++)
    //        {
    //            double dXX = (Pos_Tag[id].dx - Pos_Anch[iid].dx)*(Pos_Tag[id].dx - Pos_Anch[iid].dx);
    //            double dYY = (Pos_Tag[id].dy - Pos_Anch[iid].dy)*(Pos_Tag[id].dy - Pos_Anch[iid].dy);
    //            double dZZ = (Pos_Tag[id].dz - Pos_Anch[iid].dz)*(Pos_Tag[id].dz - Pos_Anch[iid].dz);
    //            dRang[iid] = sqrt(dXX+dYY+dZZ);
    //        }
    //        est_Tag_Pos[id].dx = 0.; est_Tag_Pos[id].dy = 0.; est_Tag_Pos[id].dz = 0.;
    ////        tagLoc->locEstByMatrix(nAnchNum, Pos_Anch, dRang, est_Tag_Pos+id);
    //        tagLoc->locEstByLSE(nAnchNum, Pos_Anch, dRang, est_Tag_Pos+id);

    //        cout << " The real tag position of Tag" << id << " is: " << Pos_Tag[id].dx <<"\t" << Pos_Tag[id].dy <<"\t" << Pos_Tag[id].dz << endl;
    //        cout << " The estimated tag position of Tag" <<id << " is: " << est_Tag_Pos[id].dx <<"\t" << est_Tag_Pos[id].dy <<"\t" << est_Tag_Pos[id].dz << endl;
    //        cout << " The error is: " << abs(Pos_Tag[id].dx - est_Tag_Pos[id].dx) <<"\t"
    //             << abs(Pos_Tag[id].dy - est_Tag_Pos[id].dy) <<"\t"
    //             << abs(Pos_Tag[id].dz - est_Tag_Pos[id].dz) << endl;
    //    }

    //    delete  tagLoc;
    //    delete [] est_Tag_Pos;
    //    delete [] Pos_Anch;
    //    delete [] Pos_Tag;

//    // Step4. upload results to server
//    cTCPCom *newConnection = new cTCPCom;
//    bool isConnected;
//    do{
//        cout << "Haven't connected to server yet!" << endl;
//        isConnected = newConnection->isConnected();
//        QThread::sleep(2);
//    }
//    while(!isConnected);
//    QByteArray msg("Hello, server!");
//    for (int id = 0; id < 5; id++){
//        newConnection->writeData(msg);
//        QThread::sleep(2);
//    }

    /// Step 4 Wait and Respond Next Command
    bool isQuit = false;
    do{
        cout << "Please input the command: " << endl;
        char tmpCmd[10];
        QByteArray cmd; cmd.clear();
        cin >> tmpCmd;
        cmd.append(tmpCmd);
        if (cmd == "q")
            isQuit = true;
    }
    while (!isQuit);

    /// Step end: delete all news and quit
    spIOThread->quit();
    spIOThread->wait();
    delete[] dAnch_X;
    delete[] dAnch_Y;
    delete[] dAnch_Z;
    delete[] anchXYZ;
    delete device;
    delete spIOThread;

    cout << "=====Game Over=====" << endl;
    return coreApplication.exec();
}
