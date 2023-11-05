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

    clkPhaseCnt->UpdateDown(VCC);
    clkPhaseCnt->UpdateClr(GND);
    clkPhaseCnt->UpdateLoad(VCC);
    clkPhaseCnt->UpdateA(GND);
    clkPhaseCnt->UpdateB(GND);
    clkPhaseCnt->UpdateC(GND);
    clkPhaseCnt->UpdateD(GND);

    clkPhaseDemux->UpdateG1(VCC);
    clkPhaseDemux->UpdateG2A(GND);
    clkPhaseDemux->UpdateG2B(GND);
    clkPhaseDemux->UpdateC(GND);

    clkSignalShaper->UpdateA3(GND);
    clkSignalShaper->UpdateB3(GND);
    clkSignalShaper->UpdateA4(GND);
    clkSignalShaper->UpdateB4(GND);

    clkControlLatch->UpdateD4(GND);
    clkControlLatch->UpdateD5(GND);
    clkControlLatch->UpdateD6(GND);
    clkControlLatch->UpdateD7(GND);
    clkControlLatch->UpdateD8(GND);
    clkControlLatch->UpdateOE(GND);

    clkControlOr->UpdateA4(GND);
    clkControlOr->UpdateB4(GND);


    // -- connect the control
    connect(clkControlLatch, &IC_74xx574_t::Q1Updated, clkControlAnd, &IC_74xx08_t::UpdateA1);
    connect(clkControlLatch, &IC_74xx574_t::Q2Updated, clkControlAnd, &IC_74xx08_t::UpdateA2);
    connect(clkControlLatch, &IC_74xx574_t::Q3Updated, clkControlAnd, &IC_74xx08_t::UpdateA4);
    connect(osc, &HW_Oscillator_t::StateChanged, clkControlLatch, &IC_74xx574_t::UpdateClk);

    // -- connect to the oscillator
//    connect(osc, &HW_Oscillator_t::StateChanged, this, &HW_Clock4Phase_t::ClockStateChange);
    connect(osc, &HW_Oscillator_t::StateChanged, clkControlAnd, &IC_74xx08_t::UpdateB1);
    connect(osc, &HW_Oscillator_t::StateChanged, clkControlAnd, &IC_74xx08_t::UpdateB2);
    connect(clkControlOr, &IC_74xx32_t::Y3Updated, this, &HW_Clock4Phase_t::ClockStateChange);

    // -- Combine the clock control signals
    connect(clkControlAnd, &IC_74xx08_t::Y1Updated, clkControlOr, &IC_74xx32_t::UpdateA2);
    connect(clkPhaseDemux, &IC_74xx138_t::Y0Updated, clkControlOr, &IC_74xx32_t::UpdateA1);
    connect(clkControlOr, &IC_74xx32_t::Y1Updated, clkControlAnd, &IC_74xx08_t::UpdateB3);
    connect(clkControlAnd, &IC_74xx08_t::Y2Updated, clkControlAnd, &IC_74xx08_t::UpdateA3);
    connect(clkControlAnd, &IC_74xx08_t::Y3Updated, clkControlOr, &IC_74xx32_t::UpdateB2);
    connect(clkControlOr, &IC_74xx32_t::Y2Updated, clkControlOr, &IC_74xx32_t::UpdateA3);
    connect(clkControlAnd, &IC_74xx08_t::Y4Updated, clkControlOr, &IC_74xx32_t::UpdateB3);

    // -- connect the counter
    connect(clkPhaseCnt, &IC_74xx193_t::QaUpdated, clkPhaseDemux, &IC_74xx138_t::UpdateA);
    connect(clkPhaseCnt, &IC_74xx193_t::QbUpdated, clkPhaseDemux, &IC_74xx138_t::UpdateB);
    connect(clkPhaseCnt, &IC_74xx193_t::AllUpdated, clkPhaseDemux, &IC_74xx138_t::UpdatesComplete);

    // -- connect the inputs of the clock phases
    connect(clkPhaseDemux, &IC_74xx138_t::Y1Updated, clkSignalShaper, &IC_74xx08_t::UpdateA1);
    connect(clkPhaseDemux, &IC_74xx138_t::Y2Updated, clkSignalShaper, &IC_74xx08_t::UpdateB1);
    connect(clkPhaseDemux, &IC_74xx138_t::Y2Updated, clkSignalShaper, &IC_74xx08_t::UpdateA2);
    connect(clkPhaseDemux, &IC_74xx138_t::Y3Updated, clkSignalShaper, &IC_74xx08_t::UpdateB2);
    connect(clkPhaseDemux, &IC_74xx138_t::AllUpdated, clkSignalShaper, &IC_74xx08_t::UpdateGate1);
    connect(clkPhaseDemux, &IC_74xx138_t::AllUpdated, clkSignalShaper, &IC_74xx08_t::UpdateGate2);

    connect(clkPhaseDemux, &IC_74xx138_t::Y0Updated, this, &HW_Clock4Phase_t::Cycle1Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::Y1Updated, this, &HW_Clock4Phase_t::Cycle2Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::Y2Updated, this, &HW_Clock4Phase_t::Cycle3Changed);
    connect(clkPhaseDemux, &IC_74xx138_t::Y3Updated, this, &HW_Clock4Phase_t::Cycle4Changed);

    // -- connect the outputs from this class
    connect(clkSignalShaper, &IC_74xx08_t::Y1Updated, this, &HW_Clock4Phase_t::Phase1Changed);
    connect(clkSignalShaper, &IC_74xx08_t::Y2Updated, this, &HW_Clock4Phase_t::Phase2Changed);
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
inline void HW_Clock4Phase_t::ClockStateChange(TriState_t state) {
    clkPhaseCnt->UpdateUp(state);
}


