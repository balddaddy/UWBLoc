#include <QDebug>
#include <iostream>
#include <string>
// #include "cserialports.h"
using namespace std;
int main(void)
{
    cout << string(1000,'\n');

    cout << "+++This is a demo to test UWB localization System.+++\n"
         << "++++Author: CharleyChang    Data: 2020. 03. 25+++\n";

    cout << "=====Program Started=====\n";

    int nAnchNum = 0; int nTagNum = 0;
    while (nTagNum < 1 || nTagNum > 8)
    {
        cout << "\n";
        cout << "Please input the number of tags[1~8]:";
        cin >> nTagNum;
        if (cin.fail() || nTagNum < 1 || nTagNum > 8){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "The input is not a number, please input a number between 1 to 8!" << endl;
        }
    }
    cout << "There are " << nTagNum << " tags in the system.\n";
    while (nAnchNum < 1 || nAnchNum > 4)
    {
        cout << "\n";
        cout << "Please input the number of anchors[1~4]:";
        cin >> nAnchNum;
        if (cin.fail() || nAnchNum < 1 || nAnchNum > 4){
            cin.clear(); 
            cin.ignore(1000,'\n');
            cout << "The input is not a number, please input a number between 1 to 4!" << endl;
        }
    }
    cout << "There are " << nAnchNum << " anchors in the system.\n";
    for (int nid = 1; nid <= nAnchNum; nid++)
    {
        double dX = 0.0; double dY = 0.0; double dZ = 0.0;
        cout << "Input the coordination of the anchor No." << nid << "\n";
        cout << "X:"; cin >> dX;
        while (cin.fail()){
            cin.clear(); 
            cin.ignore(1000,'\n');
            cout << "Please input a number!" << endl;
            cout << "X:"; cin >> dX;
        }
        cout << "Y:"; cin >> dY;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Please input a number!" << endl;
            cout << "Y:"; cin >> dY;
        }
        cout << "Z:"; cin >> dZ;
        while (cin.fail()){
            cin.clear();
            cin.ignore(1000,'\n');
            cout << "Please input a number!" << endl;
            cout << "Z:"; cin >> dZ;
        }
    }
    // CSerialPorts *device = new CSerialPorts;
    // device->testDevices();

    cout << "End of Processing.\n";
    return 0;
}
