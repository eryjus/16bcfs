//===================================================================================================================
//  hw-clock4phase.cc -- This class implements a 4-phase clock
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-28  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-clock4phase.moc.cc"


//
// -- Construct a new 4-phase clock
//    -----------------------------
HW_Clock4Phase_t::HW_Clock4Phase_t(HW_Oscillator_t *osc, QObject *parent) : QObject(parent)
{
    clkPhaseCnt = new IC_74xx193_t;
    clkPhaseDemux = new IC_74xx138_t;
    clkSignalShaper = new IC_74xx08_t;
    clkControlLatch = new IC_74xx574_t;
    clkControlAnd = new IC_74xx08_t;
    clkControlOr = new IC_74xx32_t;

    clkPhaseCnt->ProcessUpdateDown(VCC);
    clkPhaseCnt->ProcessUpdateClr(GND);
    clkPhaseCnt->ProcessUpdateLoad(VCC);
    clkPhaseCnt->ProcessUpdateA(GND);
    clkPhaseCnt->ProcessUpdateB(GND);
    clkPhaseCnt->ProcessUpdateC(GND);
    clkPhaseCnt->ProcessUpdateD(GND);

    clkPhaseDemux->ProcessUpdateG1(VCC);
    clkPhaseDemux->ProcessUpdateG2A(GND);
    clkPhaseDemux->ProcessUpdateG2B(GND);
    clkPhaseDemux->ProcessUpdateC(GND);

    clkSignalShaper->ProcessUpdateA3(GND);
    clkSignalShaper->ProcessUpdateB3(GND);
    clkSignalShaper->ProcessUpdateA4(GND);
    clkSignalShaper->ProcessUpdateB4(GND);

    clkControlLatch->ProcessUpdateD4(GND);
    clkControlLatch->ProcessUpdateD5(GND);
    clkControlLatch->ProcessUpdateD6(GND);
    clkControlLatch->ProcessUpdateD7(GND);
    clkControlLatch->ProcessUpdateD8(GND);
    clkControlLatch->ProcessUpdateOE(GND);

    clkControlOr->ProcessUpdateA4(GND);
    clkControlOr->ProcessUpdateB4(GND);


    // -- connect the control
    connect(clkControlLatch, &IC_74xx574_t::SignalQ1Updated, clkControlAnd, &IC_74xx08_t::ProcessUpdateA1);
    connect(clkControlLatch, &IC_74xx574_t::SignalQ2Updated, clkControlAnd, &IC_74xx08_t::ProcessUpdateA2);
    connect(clkControlLatch, &IC_74xx574_t::SignalQ3Updated, clkControlAnd, &IC_74xx08_t::ProcessUpdateA4);
    connect(osc, &HW_Oscillator_t::SignalStateChanged, clkControlLatch, &IC_74xx574_t::ProcessUpdateClk);

    // -- connect to the oscillator
    connect(osc, &HW_Oscillator_t::SignalStateChanged, clkControlAnd, &IC_74xx08_t::ProcessUpdateB1);
    connect(osc, &HW_Oscillator_t::SignalStateChanged, clkControlAnd, &IC_74xx08_t::ProcessUpdateB2);
    connect(clkControlOr, &IC_74xx32_t::SignalY3Updated, this, &HW_Clock4Phase_t::ProcessClockStateChange);

    // -- Combine the clock control signals
    connect(clkControlAnd, &IC_74xx08_t::SignalY1Updated, clkControlOr, &IC_74xx32_t::ProcessUpdateA2);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY0Updated, clkControlOr, &IC_74xx32_t::ProcessUpdateA1);
    connect(clkControlOr, &IC_74xx32_t::SignalY1Updated, clkControlAnd, &IC_74xx08_t::ProcessUpdateB3);
    connect(clkControlAnd, &IC_74xx08_t::SignalY2Updated, clkControlAnd, &IC_74xx08_t::ProcessUpdateA3);
    connect(clkControlAnd, &IC_74xx08_t::SignalY3Updated, clkControlOr, &IC_74xx32_t::ProcessUpdateB2);
    connect(clkControlOr, &IC_74xx32_t::SignalY2Updated, clkControlOr, &IC_74xx32_t::ProcessUpdateA3);
    connect(clkControlAnd, &IC_74xx08_t::SignalY4Updated, clkControlOr, &IC_74xx32_t::ProcessUpdateB3);

    // -- connect the counter
    connect(clkPhaseCnt, &IC_74xx193_t::SignalQaUpdated, clkPhaseDemux, &IC_74xx138_t::ProcessUpdateA);
    connect(clkPhaseCnt, &IC_74xx193_t::SignalQbUpdated, clkPhaseDemux, &IC_74xx138_t::ProcessUpdateB);
    connect(clkPhaseCnt, &IC_74xx193_t::SignalAllUpdated, clkPhaseDemux, &IC_74xx138_t::ProcessUpdatesComplete);

    // -- connect the inputs of the clock phases
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY1Updated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateA1);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY2Updated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateB1);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY2Updated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateA2);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY3Updated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateB2);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalAllUpdated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateGate1);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalAllUpdated, clkSignalShaper, &IC_74xx08_t::ProcessUpdateGate2);

    connect(clkPhaseDemux, &IC_74xx138_t::SignalY0Updated, this, &HW_Clock4Phase_t::SignalCycle1Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY1Updated, this, &HW_Clock4Phase_t::SignalCycle2Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY2Updated, this, &HW_Clock4Phase_t::SignalCycle3Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::SignalY3Updated, this, &HW_Clock4Phase_t::SignalCycle4Changed);

    // -- connect the outputs from this class
    connect(clkSignalShaper, &IC_74xx08_t::SignalY1Updated, this, &HW_Clock4Phase_t::SignalPhase1Changed);
    connect(clkSignalShaper, &IC_74xx08_t::SignalY2Updated, this, &HW_Clock4Phase_t::SignalPhase2Changed);

    TriggerFirstUpdates();
}


//
// -- Trigger the fitst updates for all children
//    ------------------------------------------
void HW_Clock4Phase_t::TriggerFirstUpdates(void)
{
    clkPhaseCnt->TriggerFirstUpdates();
    clkPhaseDemux->TriggerFirstUpdates();
    clkSignalShaper->TriggerFirstUpdates();
    clkControlLatch->TriggerFirstUpdates();
    clkControlAnd->TriggerFirstUpdates();
    clkControlOr->TriggerFirstUpdates();
}


//
// -- On a clock state change, pass that new state on to the counter
//    --------------------------------------------------------------
inline void HW_Clock4Phase_t::ProcessClockStateChange(TriState_t state) {
    clkPhaseCnt->ProcessUpdateUp(state);
}


