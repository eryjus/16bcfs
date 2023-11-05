//===================================================================================================================
//  hw-clock4phase.hh -- This class implements a 4-phase clock
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-28  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- This ia a 4-phase clock
//    -----------------------
class HW_Clock4Phase_t : public QObject {
    Q_OBJECT


private:
    IC_74xx193_t *clkPhaseCnt;          // counts phases from 0 to 3
    IC_74xx138_t *clkPhaseDemux;        // dmultiplexes the count to 4 active-low signals
    IC_74xx08_t *clkSignalShaper;       // 2 of 4 gates used; combines the demuxed signals into clocks offset by 90d
    IC_74xx574_t *clkControlLatch;      // uses 3 signals to latch the control signals from the buttons
    IC_74xx08_t *clkControlAnd;         // used to combine signals with the clock 4/4 used
    IC_74xx32_t *clkControlOr;          // used to combine signals with the clock and output the internal clk version 3/4 used


public:
    HW_Clock4Phase_t(HW_Oscillator_t *osc, QObject *parent = nullptr);
    virtual ~HW_Clock4Phase_t() {}


public:
    void TriggerFirstUpdates(void);

public:
    IC_74xx574_t *GetClkControlLatch(void) const { return clkControlLatch; }
    IC_74xx08_t *GetClkControlAnd(void) const { return clkControlAnd; }
    IC_74xx32_t *GetClkControlOr(void) const { return clkControlOr; }


public slots:
    void ClockStateChange(TriState_t state);

    void Phase1StateChange(TriState_t state) { emit Phase1Changed(state); }
    void Phase2StateChange(TriState_t state) { emit Phase2Changed(state); }

    void ClockCycle1Change(TriState_t state) { emit Cycle1Changed(state); }
    void ClockCycle2Change(TriState_t state) { emit Cycle2Changed(state); }
    void ClockCycle3Change(TriState_t state) { emit Cycle3Changed(state); }
    void ClockCycle4Change(TriState_t state) { emit Cycle4Changed(state); }


signals:
    void Phase1Changed(TriState_t state);
    void Phase2Changed(TriState_t state);

    void Cycle1Changed(TriState_t state);
    void Cycle2Changed(TriState_t state);
    void Cycle3Changed(TriState_t state);
    void Cycle4Changed(TriState_t state);
};
