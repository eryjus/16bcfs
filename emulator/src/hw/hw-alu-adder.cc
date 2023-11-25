//===================================================================================================================
//  hw-alu-adder.cc -- This class represents the adder module in the ALU
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-11  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-alu-adder.moc.cc"



//
// -- construct an adder for the ALU
//    ------------------------------
HW_AluAdder_t::HW_AluAdder_t(IC_74xx541_t *aluALsb, IC_74xx541_t *aluAMsb,
                IC_74xx541_t *aluBLsb, IC_74xx541_t *aluBMsb, HW_Bus_t *mainBus,
                QObject *parent) : QObject(parent)
{
    // -- allocate the components
    bits0 = new IC_74xx283_t;
    bits3 = new IC_74xx283_t;
    bits7 = new IC_74xx283_t;
    bitsb = new IC_74xx283_t;
    bitsf = new IC_74xx283_t;

    driver = new HW_BusDriver_t(mainBus);

    bits0->setObjectName("bits0");
    bits3->setObjectName("bits3");
    bits7->setObjectName("bits7");
    bitsb->setObjectName("bitsb");
    bitsf->setObjectName("bitsf");


    // -- several lines will be pulled high or low directly
    bits0->ProcessA0Update(LOW);
    bits0->ProcessB0Update(HIGH);

    bitsf->ProcessA1Update(LOW);
    bitsf->ProcessB1Update(HIGH);
    bitsf->ProcessA2Update(LOW);
    bitsf->ProcessB2Update(HIGH);
    bitsf->ProcessA3Update(LOW);
    bitsf->ProcessB3Update(HIGH);


    // -- connect the ALU A inputs
    connect(aluALsb, &IC_74xx541_t::SignalY0Updated, bits0, &IC_74xx283_t::ProcessA1Update);
    connect(aluALsb, &IC_74xx541_t::SignalY1Updated, bits0, &IC_74xx283_t::ProcessA2Update);
    connect(aluALsb, &IC_74xx541_t::SignalY2Updated, bits0, &IC_74xx283_t::ProcessA3Update);

    connect(aluALsb, &IC_74xx541_t::SignalY3Updated, bits3, &IC_74xx283_t::ProcessA0Update);
    connect(aluALsb, &IC_74xx541_t::SignalY4Updated, bits3, &IC_74xx283_t::ProcessA1Update);
    connect(aluALsb, &IC_74xx541_t::SignalY5Updated, bits3, &IC_74xx283_t::ProcessA2Update);
    connect(aluALsb, &IC_74xx541_t::SignalY6Updated, bits3, &IC_74xx283_t::ProcessA3Update);

    connect(aluALsb, &IC_74xx541_t::SignalY7Updated, bits7, &IC_74xx283_t::ProcessA0Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY0Updated, bits7, &IC_74xx283_t::ProcessA1Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY1Updated, bits7, &IC_74xx283_t::ProcessA2Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY2Updated, bits7, &IC_74xx283_t::ProcessA3Update);

    connect(aluAMsb, &IC_74xx541_t::SignalY3Updated, bitsb, &IC_74xx283_t::ProcessA0Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY4Updated, bitsb, &IC_74xx283_t::ProcessA1Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY5Updated, bitsb, &IC_74xx283_t::ProcessA2Update);
    connect(aluAMsb, &IC_74xx541_t::SignalY6Updated, bitsb, &IC_74xx283_t::ProcessA3Update);

    connect(aluAMsb, &IC_74xx541_t::SignalY7Updated, bitsf, &IC_74xx283_t::ProcessA0Update);


    // -- connect the ALU B inputs
    connect(aluBLsb, &IC_74xx541_t::SignalY0Updated, bits0, &IC_74xx283_t::ProcessB1Update);
    connect(aluBLsb, &IC_74xx541_t::SignalY1Updated, bits0, &IC_74xx283_t::ProcessB2Update);
    connect(aluBLsb, &IC_74xx541_t::SignalY2Updated, bits0, &IC_74xx283_t::ProcessB3Update);

    connect(aluBLsb, &IC_74xx541_t::SignalY3Updated, bits3, &IC_74xx283_t::ProcessB0Update);
    connect(aluBLsb, &IC_74xx541_t::SignalY4Updated, bits3, &IC_74xx283_t::ProcessB1Update);
    connect(aluBLsb, &IC_74xx541_t::SignalY5Updated, bits3, &IC_74xx283_t::ProcessB2Update);
    connect(aluBLsb, &IC_74xx541_t::SignalY6Updated, bits3, &IC_74xx283_t::ProcessB3Update);

    connect(aluBLsb, &IC_74xx541_t::SignalY7Updated, bits7, &IC_74xx283_t::ProcessB0Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY0Updated, bits7, &IC_74xx283_t::ProcessB1Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY1Updated, bits7, &IC_74xx283_t::ProcessB2Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY2Updated, bits7, &IC_74xx283_t::ProcessB3Update);

    connect(aluBMsb, &IC_74xx541_t::SignalY3Updated, bitsb, &IC_74xx283_t::ProcessB0Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY4Updated, bitsb, &IC_74xx283_t::ProcessB1Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY5Updated, bitsb, &IC_74xx283_t::ProcessB2Update);
    connect(aluBMsb, &IC_74xx541_t::SignalY6Updated, bitsb, &IC_74xx283_t::ProcessB3Update);

    connect(aluBMsb, &IC_74xx541_t::SignalY7Updated, bitsf, &IC_74xx283_t::ProcessB0Update);


    // -- interconnects between the ICs
    connect(bits0, &IC_74xx283_t::SignalCOutUpdated, bits3, &IC_74xx283_t::ProcessCInUpdate);
    connect(bits3, &IC_74xx283_t::SignalCOutUpdated, bits7, &IC_74xx283_t::ProcessCInUpdate);
    connect(bits7, &IC_74xx283_t::SignalCOutUpdated, bitsb, &IC_74xx283_t::ProcessCInUpdate);
    connect(bitsb, &IC_74xx283_t::SignalCOutUpdated, bitsf, &IC_74xx283_t::ProcessCInUpdate);


    // -- connect the adder outputs to the Bus Driver
    connect(bits0, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit0);
    connect(bits0, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBit1);
    connect(bits0, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBit2);

    connect(bits3, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBit3);
    connect(bits3, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit4);
    connect(bits3, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBit5);
    connect(bits3, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBit6);

    connect(bits7, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBit7);
    connect(bits7, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit8);
    connect(bits7, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBit9);
    connect(bits7, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBitA);

    connect(bitsb, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBitB);
    connect(bitsb, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBitC);
    connect(bitsb, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBitD);
    connect(bitsb, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBitE);

    connect(bitsf, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBitF);


    // -- handle the carry output
    connect(bitsf, &IC_74xx283_t::SignalCOutUpdated, this, &HW_AluAdder_t::SignalCarryOutUpdated);


    // -- here are some temporary connections, which will be replaced later
    bits0->ProcessCInUpdate(LOW);
    driver->ProcessUpdateOE1(LOW);
    driver->ProcessUpdateOE2(LOW);


    TriggerFirstUpdate();
}



//
// -- Handle the initial updates to put everything in order
//    -----------------------------------------------------
void HW_AluAdder_t::TriggerFirstUpdate(void)
{
    bits0->TriggerFirstUpdate();
    bits3->TriggerFirstUpdate();
    bits7->TriggerFirstUpdate();
    bitsb->TriggerFirstUpdate();
    bitsf->TriggerFirstUpdate();
}


