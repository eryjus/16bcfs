//===================================================================================================================
//  hw-bus-driver.hh -- This is a generic bus driver to push data onto a bus
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-07  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-bus-driver.moc.cc"



//
// -- This is a group of line drivers which will drive a 16-bit bus
//    -------------------------------------------------------------
HW_BusDriver_t::HW_BusDriver_t(HW_Bus_16_t *bus, QObject *parent) : QObject(parent)
{
    msb = new IC_74xx541_t;
    lsb = new IC_74xx541_t;

    // -- connect the output signals to their sources on the IC
    connect(msb, &IC_74xx541_t::SignalY7Updated, bus, &HW_Bus_16_t::ProcessUpdateBitF);
    connect(msb, &IC_74xx541_t::SignalY6Updated, bus, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(msb, &IC_74xx541_t::SignalY5Updated, bus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(msb, &IC_74xx541_t::SignalY4Updated, bus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(msb, &IC_74xx541_t::SignalY3Updated, bus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(msb, &IC_74xx541_t::SignalY2Updated, bus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(msb, &IC_74xx541_t::SignalY1Updated, bus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(msb, &IC_74xx541_t::SignalY0Updated, bus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(lsb, &IC_74xx541_t::SignalY7Updated, bus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(lsb, &IC_74xx541_t::SignalY6Updated, bus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(lsb, &IC_74xx541_t::SignalY5Updated, bus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(lsb, &IC_74xx541_t::SignalY4Updated, bus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(lsb, &IC_74xx541_t::SignalY3Updated, bus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(lsb, &IC_74xx541_t::SignalY2Updated, bus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(lsb, &IC_74xx541_t::SignalY1Updated, bus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(lsb, &IC_74xx541_t::SignalY0Updated, bus, &HW_Bus_16_t::ProcessUpdateBit0);

    TriggerFirstUpdate();
}


//
// -- Trigger all the initial updated to put the 'hardware' into the correct state
//    ----------------------------------------------------------------------------
void HW_BusDriver_t::TriggerFirstUpdate(void)
{
    msb->TriggerFirstUpdate();
    lsb->TriggerFirstUpdate();
}


