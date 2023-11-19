//===================================================================================================================
//  hw-sr-latch.cc -- This class represents a simple SR latch
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-sr-latch.moc.cc"



//
// -- Initialize the SR Latch, tying the unused NAND gates to ground
//    --------------------------------------------------------------
HW_SrLatch_t::HW_SrLatch_t(void)
{
    nand = new IC_74xx00_t;

    nand->ProcessA2Low();
    nand->ProcessB2Low();
    nand->ProcessA3Low();
    nand->ProcessB3Low();

    // -- connect the SR latch
    connect(nand, &IC_74xx00_t::SignalY1Updated, nand, &IC_74xx00_t::ProcessUpdateB4);
    connect(nand, &IC_74xx00_t::SignalY4Updated, nand, &IC_74xx00_t::ProcessUpdateB1);

    // -- re-signal the outputs
    connect(nand, &IC_74xx00_t::SignalY4Updated, this, &HW_SrLatch_t::SignalUpdateQ);
    connect(nand, &IC_74xx00_t::SignalY1Updated, this, &HW_SrLatch_t::SignalUpdateQb);

    // -- perform the initial updates
    nand->TriggerFirstUpdates();
}


