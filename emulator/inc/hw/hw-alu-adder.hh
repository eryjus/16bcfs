//===================================================================================================================
//  hw-alu-adder.hh -- This class represents the adder module in the ALU
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-11  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents the adder module in the ALU
//    -------------------------------------------------
class HW_AluAdder_t : public QObject {
    Q_OBJECT

private:
    IC_74xx283_t *bits0;
    IC_74xx283_t *bits4;
    IC_74xx283_t *bits8;
    IC_74xx283_t *bitsc;

    HW_BusDriver_t *driver;


public:
    HW_AluAdder_t(IC_74xx541_t *aluALsb, IC_74xx541_t *aluAMsb,
            IC_74xx541_t *aluBLsb, IC_74xx541_t *aluBMsb, HW_Bus_16_t *mainBus,
            QObject *parent = nullptr);
    virtual ~HW_AluAdder_t() {}


public:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessCarryInUpdate(TriState_t state) { bits0->ProcessCInUpdate(state); }


signals:
    void SignalCarryOutUpdated(TriState_t state);
};


