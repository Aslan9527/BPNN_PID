/*
 * BPNN_PID.h
 *
 *  Created on: 2020年11月5日
 *      Author: Administrator
 */

#ifndef INC_BPNN_PID_H_
#define INC_BPNN_PID_H_

class BPNN_PID
{

protected:

    double DeltaE_k0,DeltaE_k1;
    double Error_K0,Error_K1,Error_K2;
    struct BpnnInputStatic { double x1,x2,x3; } bpnnInput;
    struct ProgressiveSignal { double r1,r2,r3; } progSignal;

public:

    struct LearnRateStruct { double LRp,LRi,LRd; } learnRate;
    double TargetV,CurrentV;
    double U_Ctrl,K_constant;

    struct weightStatic
    {
        double w1,w2,w3;
    } bpnnWeight;

    BPNN_PID()
    {
        bpnnWeight.w1=0.1,bpnnWeight.w2=0.1,bpnnWeight.w3=0.1;
        bpnnInput.x1=0,bpnnInput.x2=0,bpnnInput.x3 = 0;
        TargetV=0,CurrentV=0,U_Ctrl=0,Error_K0=0,Error_K1=0,Error_K2=0;
        learnRate.LRp = 0.1;
        learnRate.LRi = 0.05;
        learnRate.LRd = 0.02;
        DeltaE_k1 = 0,DeltaE_k0 = 0;
        K_constant = 0.05;
    }

    BPNN_PID(double W1, double W2,double W3)
    {
        bpnnInput.x1=0,bpnnInput.x2=0,bpnnInput.x3 = 0;
        TargetV=0,CurrentV=0,U_Ctrl=0,Error_K0=0,Error_K1=0,Error_K2=0;
        bpnnWeight.w1=W1,bpnnWeight.w2=W2,bpnnWeight.w3=W3;
        learnRate.LRp = 0.1;
        learnRate.LRi = 0.04;
        learnRate.LRd = 0.02;
        DeltaE_k1 = 0,DeltaE_k0 = 0;
        K_constant = 0.01;
    }

    BPNN_PID(double W1, double W2,double W3,
             double LRp,double LRi,double LRd)
    {
        bpnnInput.x1=0,bpnnInput.x2=0,bpnnInput.x3 = 0;
        TargetV=0,CurrentV=0,U_Ctrl=0,Error_K0=0,Error_K1=0,Error_K2=0;
        learnRate.LRp = LRp;learnRate.LRi = LRi;learnRate.LRd = LRd;
        bpnnWeight.w1=W1,bpnnWeight.w2=W2,bpnnWeight.w3=W3;
        DeltaE_k1 = 0,DeltaE_k0 = 0;
        K_constant = 0.05;
    }


    void SetK_Constant(double K){ K_constant = K; }

    void SetU_Ctrl(double u)
    {
        U_Ctrl = u;
    }

    void SetLearnRate(double LRp,double LRi,double LRd)
    {
        learnRate.LRp = LRp;
        learnRate.LRi = LRi;
        learnRate.LRd = LRd;
    };

    void GenerateInput(void)
    {
        Error_K2 = Error_K1;
        Error_K1 = Error_K0;
        Error_K0 = TargetV - CurrentV;

        DeltaE_k0 = Error_K0 - Error_K1;
        DeltaE_k1 = Error_K1 - Error_K2;

        bpnnInput.x1 = DeltaE_k0;
        bpnnInput.x2 = Error_K0;
        bpnnInput.x3 = DeltaE_k0 - DeltaE_k1;
    }

    void UpdataWeight(void)
    {

        progSignal.r1 = Error_K0 * U_Ctrl * bpnnInput.x1;
        progSignal.r2 = Error_K1 * U_Ctrl * bpnnInput.x2;
        progSignal.r3 = Error_K1 * U_Ctrl * bpnnInput.x3;

        double EUdE = U_Ctrl * Error_K0 * (Error_K0 + DeltaE_k0);
        bpnnWeight.w1 = bpnnWeight.w1 + learnRate.LRp *EUdE;
        bpnnWeight.w2 = bpnnWeight.w2 + learnRate.LRi *EUdE;
        bpnnWeight.w3 = bpnnWeight.w3 + learnRate.LRd *EUdE;
    }

    double GetU_Ctrl(void)
    {

        this->GenerateInput();
        this->UpdataWeight();

        U_Ctrl = U_Ctrl + K_constant * (
                bpnnInput.x1 * bpnnWeight.w1 +
                bpnnInput.x2 * bpnnWeight.w2 +
                bpnnInput.x3 * bpnnWeight.w3 );

        return U_Ctrl ;
    }

};

#endif /* INC_BPNN_PID_H_ */
