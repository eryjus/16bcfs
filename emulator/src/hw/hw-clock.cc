//===================================================================================================================
//  hw-clock.cc -- This class represents the clock driving all the circuits
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-clock.moc.cc"



//
// -- This is the clock circuit, which will emulate the computer's clock and its mode
//    -------------------------------------------------------------------------------
HW_Clock_t::HW_Clock_t(HW_Oscillator_t *osc)
{
    sr = new HW_SrLatch_t;
    and1 = new IC_74xx08_t;
    or1 = new IC_74xx32_t;

    // -- connect the unused lines
    and1->ProcessA3Low();
    and1->ProcessB3Low();
    and1->ProcessA4Low();
    and1->ProcessB4Low();

    or1->ProcessA2Low();
    or1->ProcessB2Low();
    or1->ProcessA3Low();
    or1->ProcessB3Low();
    or1->ProcessA4Low();
    or1->ProcessB4Low();

    // -- Connect all the proper signaling
    connect(osc, &HW_Oscillator_t::SignalStateChanged, and1, &IC_74xx08_t::ProcessUpdateA1);

    connect(sr, &HW_SrLatch_t::SignalUpdateQ, and1, &IC_74xx08_t::ProcessUpdateB1);
    connect(sr, &HW_SrLatch_t::SignalUpdateQb, and1, &IC_74xx08_t::ProcessUpdateA2);

    connect(sr, &HW_SrLatch_t::SignalUpdateQ, this, &HW_Clock_t::SignalRunMode);
    connect(sr, &HW_SrLatch_t::SignalUpdateQb, this, &HW_Clock_t::SignalStepMode);

    connect(and1, &IC_74xx08_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateB1);
    connect(and1, &IC_74xx08_t::SignalY2Updated, or1, &IC_74xx32_t::ProcessUpdateA1);

    connect(or1, &IC_74xx32_t::SignalY1Updated, this, &HW_Clock_t::SignalClockCycle);

    // -- trigger the initial updates
    TriggerFirstUpdates();
}


//
// -- Trigger the first updates
//    -------------------------
void HW_Clock_t::TriggerFirstUpdates(void)
{
    sr->TriggerFirstUpdates();
    and1->TriggerFirstUpdates();
    or1->TriggerFirstUpdates();
}



//
// -- some inline functions moved herein
//    ----------------------------------
inline void HW_Clock_t::ProcessSetRunMode(TriState_t state) { sr->ProcessSet(state); }
inline void HW_Clock_t::ProcessSetStepMode(TriState_t state) { sr->ProcessReset(state); }
inline void HW_Clock_t::ProcessStep(TriState_t state) { and1->ProcessUpdateB2(state); }


