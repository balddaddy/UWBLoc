#include <QCoreApplication>
#include <iostream>
#include <string>
#include "cserialports.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);

    cout << string(1000,'\n');

    cout << "+++This is a demo to test UWB localization System.+++" << endl
         << "++++Author: CharleyChang    Data: 2020. 03. 25+++" << endl;

    cout << "=====Program Started=====" << endl;

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
    }

    // Step1. Find the devices
     CSerialPorts *device = new CSerialPorts;
     ERROR_CODE error_code = device->openDevices();
     if (error_code == _ERROR_CODE_NOTFIND){
         cout << "\rCan't find any devices!" << endl;
     }
     else if (error_code == _ERROR_CODE_OPEN_FAIL){
         cout << "\rCan't open the devices!" << endl;
     }
     else if (error_code == _ERROR_CODE_OPEN_SUCC){
         cout << "\rOpen the devices successfully!" << endl;
     }
     device->testDevices();

    cout << "End of Processing." << endl;

    return coreApplication.exec();
}
