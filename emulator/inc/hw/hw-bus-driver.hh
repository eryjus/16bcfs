//===================================================================================================================
//  hw-bus-driver.hh -- This is a generic bus driver to push data onto a bus
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-07  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- Push data onto a bus using a generic bus driver
//    -----------------------------------------------
class HW_BusDriver_t : public QObject {
    Q_OBJECT


private:
    IC_74xx541_t *msb;
    IC_74xx541_t *lsb;


public:
    HW_BusDriver_t(HW_Bus_t *bus, QObject *parent = nullptr);
    virtual ~HW_BusDriver_t() {}


private:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessUpdateOE1(TriState_t state) { lsb->ProcessUpdateOE1(state); msb->ProcessUpdateOE1(state); }
    void ProcessUpdateOE2(TriState_t state) { lsb->ProcessUpdateOE2(state); msb->ProcessUpdateOE2(state); }
    void ProcessUpdateBit0(TriState_t state) { lsb->ProcessUpdateD0(state); }
    void ProcessUpdateBit1(TriState_t state) { lsb->ProcessUpdateD1(state); }
    void ProcessUpdateBit2(TriState_t state) { lsb->ProcessUpdateD2(state); }
    void ProcessUpdateBit3(TriState_t state) { lsb->ProcessUpdateD3(state); }
    void ProcessUpdateBit4(TriState_t state) { lsb->ProcessUpdateD4(state); }
    void ProcessUpdateBit5(TriState_t state) { lsb->ProcessUpdateD5(state); }
    void ProcessUpdateBit6(TriState_t state) { lsb->ProcessUpdateD6(state); }
    void ProcessUpdateBit7(TriState_t state) { lsb->ProcessUpdateD7(state); }
    void ProcessUpdateBit8(TriState_t state) { msb->ProcessUpdateD0(state); }
    void ProcessUpdateBit9(TriState_t state) { msb->ProcessUpdateD1(state); }
    void ProcessUpdateBitA(TriState_t state) { msb->ProcessUpdateD2(state); }
    void ProcessUpdateBitB(TriState_t state) { msb->ProcessUpdateD3(state); }
    void ProcessUpdateBitC(TriState_t state) { msb->ProcessUpdateD4(state); }
    void ProcessUpdateBitD(TriState_t state) { msb->ProcessUpdateD5(state); }
    void ProcessUpdateBitE(TriState_t state) { msb->ProcessUpdateD6(state); }
    void ProcessUpdateBitF(TriState_t state) { msb->ProcessUpdateD7(state); }
};


