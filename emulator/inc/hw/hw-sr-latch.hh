//===================================================================================================================
//  hw-sr-latch.hh -- This class represents a simple SR latch
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents an SR Latch
//    ---------------------------------
class HW_SrLatch_t : public QObject {
    Q_OBJECT


private:
    IC_74xx00_t *nand;


public:
    explicit HW_SrLatch_t(void);
    virtual ~HW_SrLatch_t() {}


public:
    void TriggerFirstUpdates(void) { nand->TriggerFirstUpdates(); }


signals:
    void SignalUpdateQ(TriState_t state);
    void SignalUpdateQb(TriState_t state);


public slots:
    void ProcessSet(TriState_t state) { nand->ProcessUpdateA4(state); }
    void ProcessReset(TriState_t state) { nand->ProcessUpdateA1(state); }

};
