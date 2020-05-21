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


    /// Step 4 Wait and Respond Next Command
    bool isQuit = false;
    do{
        cout << "Please input the command(s=switch print; p=pause; c=continue; q=quit): " << endl;
        char tmpCmd[10];
        QByteArray cmd; cmd.clear();
        cin >> tmpCmd;
        cmd.append(tmpCmd);
        if (cmd == "s")
        {
            device->switchPrintOnOff();
            server->switchPrintOnOff();
            procData->switchPrintOnOff();
        }
        else if (cmd == "p")
        {
            device->pauseSerialPort();
            server->pauseConnection();
            procData->pauseThread();
        }
        else if (cmd == "c")
        {
            device->continueSerialPort();
            server->continueConnection();
            procData->continueThread();
        }
        else if (cmd == "q")
            isQuit = true;
        else
            cout << "The command is undefined." << endl;
    }
    while (!isQuit);

    /// Step 5: delete all news and quit
    spIOThread->quit();
    spIOThread->wait();
    delete[] dAnch_X;
    delete[] dAnch_Y;
    delete[] dAnch_Z;
    delete[] anchXYZ;
    delete device;     delete spIOThread;
    delete server;     delete tcpIOThread;
    delete procData;   delete procDataThread;

    cout << "=====Game Over=====" << endl;
    return coreApplication.exec();
}
