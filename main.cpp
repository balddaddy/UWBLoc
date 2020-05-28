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
#include "cproccookeddata.h"
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
    COORD_XYZ *anchXYZ = new COORD_XYZ[nAnchNum];
    memset(anchXYZ, 0, sizeof(COORD_XYZ)*nAnchNum);
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
        anchXYZ[nid].dx = dX;
        anchXYZ[nid].dy = dY;
        anchXYZ[nid].dz = dZ;
    }

    // creat the instances of all classes
    CSerialPorts *spDevice = new CSerialPorts;
    QThread *spIOThread = new QThread;
    cTCPCom *tcpServer = new cTCPCom;
    QThread *tcpIOThread = new QThread;
    cProcRawData *procRawData = new cProcRawData;
    QThread *procRawDataThread = new QThread;
    cProcCookedData *procCookedData = new cProcCookedData;
    QThread *procCookedDataThread = new QThread;

    /// Step1. Serial Port Thread
    spDevice->setHandleDataFun(procRawData->addRawData, procRawData);
    spDevice->moveToThread(spIOThread);
    spIOThread->start();
    ERROR_CODE err_code = spDevice->initialize();
    cout << "Serial port IO thread starts!" << endl;

    /// Step2. TCP/IP Thread
    tcpServer->setHandleDataFun(procCookedData->procCookedData, procCookedData);
    tcpServer->moveToThread(tcpIOThread);
    tcpIOThread->start();
    string ipAddr;
    cout << "Please input server's IP address:";
    getline(cin, ipAddr);
    QString ipAddrStr(ipAddr.c_str());
    cout << "Please input server's connection port:";
    int serverPort;
    cin >> serverPort;
    err_code = tcpServer->initialize(ipAddrStr, serverPort);
    if (err_code == _ERROR_CODE_SUCC)
        cout << "TCP/IP IO thread starts!" << endl;
    else
        cout << "Server connection failed!" << endl;

    /// Step 3. Process raw data, localize tags and Kalman Filter tracks
    procRawData->setHandleDataFun(tcpServer->setDataToSend,tcpServer);
    procRawData->moveToThread(procRawDataThread);
    procRawDataThread->start();
    err_code = procRawData->initialize(nTagNum,nAnchNum,anchXYZ);
    if (err_code == _ERROR_CODE_SUCC)
        cout << "TCP/IP IO thread starts!" << endl;
    else
        cout << "Server connection failed!" << endl;

    /// Step 4. Process cooked data which is received from server
    procCookedData->moveToThread(procCookedDataThread);
    procCookedDataThread->start();


    /// Step 5 Wait and Respond Next Command
    bool isQuit = false;
    do{
        cout << "Please input the command(s=switch print; p=pause; c=continue; q=quit): " << endl;
        char tmpCmd[10];
        QByteArray cmd; cmd.clear();
        cin >> tmpCmd;
        cmd.append(tmpCmd);
        if (cmd == "s")
        {
            switchPrintOnOff();
        }
        else if (cmd == "p")
        {
            spDevice->pauseSerialPort();
            tcpServer->pauseConnection();
            procRawData->pauseThread();
        }
        else if (cmd == "c")
        {
            spDevice->continueSerialPort();
            tcpServer->continueConnection();
            procRawData->continueThread();
        }
        else if (cmd == "q")
            isQuit = true;
        else
            cout << "The command is undefined." << endl;
    }
    while (!isQuit);

    /// Step 5: delete all news and quit
    spIOThread->quit();     spIOThread->wait();
    tcpIOThread->quit();    tcpIOThread->wait();
    procRawDataThread->quit(); procRawDataThread->wait();
    delete spDevice;     delete spIOThread;
    delete tcpServer;     delete tcpIOThread;
    delete procRawData;   delete procRawDataThread;
    delete[] anchXYZ;

    cout << "=====Game Over=====" << endl;
    return coreApplication.exec();
}
