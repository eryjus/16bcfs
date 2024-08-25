//===================================================================================================================
//  hw-alu.cc -- This class represents the ALU, with all its components
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-12  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-alu.moc.cc"


//
// -- Consutruct the ALU
//    ------------------
HW_Alu_t::HW_Alu_t(HW_Bus_16_t *a, HW_Bus_16_t *b,
        HW_Bus_16_t *mainBus, QObject *parent)
        : QObject(parent)
{
    // -- first allocate the local hardware members
    lsbA = new IC_74xx541_t;
    msbA = new IC_74xx541_t;
    lsbB = new IC_74xx541_t;
    msbB = new IC_74xx541_t;

    adder = new HW_AluAdder_t(lsbA, msbA, lsbB, msbB, mainBus);


    // -- Some things need to be hard-wired
    lsbA->ProcessUpdateOE1(LOW);
    lsbA->ProcessUpdateOE2(LOW);
    lsbB->ProcessUpdateOE1(LOW);
    lsbB->ProcessUpdateOE2(LOW);
    msbA->ProcessUpdateOE1(LOW);
    msbA->ProcessUpdateOE2(LOW);
    msbB->ProcessUpdateOE1(LOW);
    msbB->ProcessUpdateOE2(LOW);


    // -- Connect the buses to the drivers
    connect(a, &HW_Bus_16_t::SignalBit0Updated, lsbA, &IC_74xx541_t::ProcessUpdateD0, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit1Updated, lsbA, &IC_74xx541_t::ProcessUpdateD1, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit2Updated, lsbA, &IC_74xx541_t::ProcessUpdateD2, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit3Updated, lsbA, &IC_74xx541_t::ProcessUpdateD3, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit4Updated, lsbA, &IC_74xx541_t::ProcessUpdateD4, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit5Updated, lsbA, &IC_74xx541_t::ProcessUpdateD5, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit6Updated, lsbA, &IC_74xx541_t::ProcessUpdateD6, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit7Updated, lsbA, &IC_74xx541_t::ProcessUpdateD7, CNN_TYPE);

    connect(a, &HW_Bus_16_t::SignalBit8Updated, msbA, &IC_74xx541_t::ProcessUpdateD0, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBit9Updated, msbA, &IC_74xx541_t::ProcessUpdateD1, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitAUpdated, msbA, &IC_74xx541_t::ProcessUpdateD2, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitBUpdated, msbA, &IC_74xx541_t::ProcessUpdateD3, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitCUpdated, msbA, &IC_74xx541_t::ProcessUpdateD4, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitDUpdated, msbA, &IC_74xx541_t::ProcessUpdateD5, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitEUpdated, msbA, &IC_74xx541_t::ProcessUpdateD6, CNN_TYPE);
    connect(a, &HW_Bus_16_t::SignalBitFUpdated, msbA, &IC_74xx541_t::ProcessUpdateD7, CNN_TYPE);

    connect(b, &HW_Bus_16_t::SignalBit0Updated, lsbB, &IC_74xx541_t::ProcessUpdateD0, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit1Updated, lsbB, &IC_74xx541_t::ProcessUpdateD1, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit2Updated, lsbB, &IC_74xx541_t::ProcessUpdateD2, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit3Updated, lsbB, &IC_74xx541_t::ProcessUpdateD3, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit4Updated, lsbB, &IC_74xx541_t::ProcessUpdateD4, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit5Updated, lsbB, &IC_74xx541_t::ProcessUpdateD5, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit6Updated, lsbB, &IC_74xx541_t::ProcessUpdateD6, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit7Updated, lsbB, &IC_74xx541_t::ProcessUpdateD7, CNN_TYPE);

    connect(b, &HW_Bus_16_t::SignalBit8Updated, msbB, &IC_74xx541_t::ProcessUpdateD0, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBit9Updated, msbB, &IC_74xx541_t::ProcessUpdateD1, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitAUpdated, msbB, &IC_74xx541_t::ProcessUpdateD2, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitBUpdated, msbB, &IC_74xx541_t::ProcessUpdateD3, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitCUpdated, msbB, &IC_74xx541_t::ProcessUpdateD4, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitDUpdated, msbB, &IC_74xx541_t::ProcessUpdateD5, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitEUpdated, msbB, &IC_74xx541_t::ProcessUpdateD6, CNN_TYPE);
    connect(b, &HW_Bus_16_t::SignalBitFUpdated, msbB, &IC_74xx541_t::ProcessUpdateD7, CNN_TYPE);


    // -- Perform the initial updates
    TriggerFirstUpdate();
}


//
// -- Ensure the ALU is in the proper state to perform calculations
//    -------------------------------------------------------------
void HW_Alu_t::TriggerFirstUpdate(void)
{
    adder->TriggerFirstUpdate();
    lsbA->TriggerFirstUpdate();
    msbA->TriggerFirstUpdate();
    lsbB->TriggerFirstUpdate();
    msbB->TriggerFirstUpdate();
}
