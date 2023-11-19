//===================================================================================================================
//  hw-clock.hh -- This class represents the clock driving all the circuits
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
// -- The computer's clock
//    --------------------
class HW_Clock_t : public QObject {
    Q_OBJECT


private:
    HW_SrLatch_t *sr;
    IC_74xx08_t *and1;
    IC_74xx32_t *or1;


public:
    HW_Clock_t(HW_Oscillator_t *osc);
    virtual ~HW_Clock_t() {}


public:
    IC_74xx08_t *GetClockAnd(void) const { return and1; }


public:
    void TriggerFirstUpdates(void);


signals:
    void SignalClockCycle(TriState_t state);
    void SignalRunMode(TriState_t state);
    void SignalStepMode(TriState_t state);


public slots:
    void ProcessSetRunMode(TriState_t state);
    void ProcessSetStepMode(TriState_t state);
    void ProcessStep(TriState_t state);
};

