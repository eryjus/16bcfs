//===================================================================================================================
//  hw-alu.hh -- This class represents the ALU, with all its components
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-12  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents the ALU
//    -----------------------------
class HW_Alu_t : public QObject {
    Q_OBJECT


private:
    HW_AluAdder_t *adder;

    IC_74xx541_t *lsbA;
    IC_74xx541_t *msbA;
    IC_74xx541_t *lsbB;
    IC_74xx541_t *msbB;


public:
    HW_Alu_t(HW_Bus_t *a, HW_Bus_t *b, HW_Bus_t *mainBus, QObject *parent = nullptr);
    virtual ~HW_Alu_t() {}


public:
    void TriggerFirstUpdates(void);


signals:
    void SignalAluResultBit0(TriState_t state);
    void SignalAluResultBit1(TriState_t state);
    void SignalAluResultBit2(TriState_t state);
    void SignalAluResultBit3(TriState_t state);
    void SignalAluResultBit4(TriState_t state);
    void SignalAluResultBit5(TriState_t state);
    void SignalAluResultBit6(TriState_t state);
    void SignalAluResultBit7(TriState_t state);
    void SignalAluResultBit8(TriState_t state);
    void SignalAluResultBit9(TriState_t state);
    void SignalAluResultBitA(TriState_t state);
    void SignalAluResultBitB(TriState_t state);
    void SignalAluResultBitC(TriState_t state);
    void SignalAluResultBitD(TriState_t state);
    void SignalAluResultBitE(TriState_t state);
    void SignalAluResultBitF(TriState_t state);
};

