#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#include "BPNN_PID.h"

#define Sample 40

using namespace std;

int main() {

    double Target = 15,Current = 0;
    double TestData[4][Sample] = {0};
    BPNN_PID bpnnPid;
    bpnnPid.TargetV = Target;

    string Uof = "OutputU.txt";
    string Kpof= "OutputKp.txt";
    string Kiof= "OutputKi.txt";
    string Kdof= "OutputKd.txt";

    ofstream OutputU(Uof);
    ofstream OutputKp(Kpof);
    ofstream OutputKi(Kiof);
    ofstream OutputKd(Kdof);

    for (int i = 0; i < Sample; ++i)
    {
        bpnnPid.CurrentV = Current;
        bpnnPid.GetU_Ctrl();
        Current =Current + bpnnPid.U_Ctrl;
        TestData[0][i] = Current;
        TestData[1][i] = bpnnPid.bpnnWeight.w1;
        TestData[2][i] = bpnnPid.bpnnWeight.w2;
        TestData[3][i] = bpnnPid.bpnnWeight.w3;
    }

    if(OutputU.is_open() && OutputKp.is_open() && OutputKd.is_open() && OutputKi.is_open())
    {
        for (short i = 0 ; i < Sample ; i++)
        {
            OutputU << TestData[0][i] << endl;
            OutputKp << TestData[1][i] << endl;
            OutputKi << TestData[2][i] << endl;
            OutputKd << TestData[3][i] << endl;
        }
    }
    else
    {
        cout << "Error" << endl;
    }

    return 0;
}
