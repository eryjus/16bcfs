//===================================================================================================================
//  hw-alu-adder.cc -- This class represents the adder module in the ALU
//
//      Copyright (c) 2023-2024 - Adam Clark
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
                IC_74xx541_t *aluBLsb, IC_74xx541_t *aluBMsb, HW_Bus_16_t *mainBus,
                QObject *parent) : QObject(parent)
{
    // -- allocate the components
    bits0 = new IC_74xx283_t;
    bits4 = new IC_74xx283_t;
    bits8 = new IC_74xx283_t;
    bitsc = new IC_74xx283_t;

    driver = new HW_BusDriver_t(mainBus);

    bits0->setObjectName("bits0");
    bits4->setObjectName("bits4");
    bits8->setObjectName("bits8");
    bitsc->setObjectName("bitsc");


    // -- connect the ALU A inputs
    connect(aluALsb, &IC_74xx541_t::SignalY0Updated, bits0, &IC_74xx283_t::ProcessA0Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY1Updated, bits0, &IC_74xx283_t::ProcessA1Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY2Updated, bits0, &IC_74xx283_t::ProcessA2Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY3Updated, bits0, &IC_74xx283_t::ProcessA3Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY4Updated, bits4, &IC_74xx283_t::ProcessA0Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY5Updated, bits4, &IC_74xx283_t::ProcessA1Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY6Updated, bits4, &IC_74xx283_t::ProcessA2Update, CNN_TYPE);
    connect(aluALsb, &IC_74xx541_t::SignalY7Updated, bits4, &IC_74xx283_t::ProcessA3Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY0Updated, bits8, &IC_74xx283_t::ProcessA0Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY1Updated, bits8, &IC_74xx283_t::ProcessA1Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY2Updated, bits8, &IC_74xx283_t::ProcessA2Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY3Updated, bits8, &IC_74xx283_t::ProcessA3Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY4Updated, bitsc, &IC_74xx283_t::ProcessA0Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY5Updated, bitsc, &IC_74xx283_t::ProcessA1Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY6Updated, bitsc, &IC_74xx283_t::ProcessA2Update, CNN_TYPE);
    connect(aluAMsb, &IC_74xx541_t::SignalY7Updated, bitsc, &IC_74xx283_t::ProcessA3Update, CNN_TYPE);


    // -- connect the ALU B inputs
    connect(aluBLsb, &IC_74xx541_t::SignalY0Updated, bits0, &IC_74xx283_t::ProcessB0Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY1Updated, bits0, &IC_74xx283_t::ProcessB1Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY2Updated, bits0, &IC_74xx283_t::ProcessB2Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY3Updated, bits0, &IC_74xx283_t::ProcessB3Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY4Updated, bits4, &IC_74xx283_t::ProcessB0Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY5Updated, bits4, &IC_74xx283_t::ProcessB1Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY6Updated, bits4, &IC_74xx283_t::ProcessB2Update, CNN_TYPE);
    connect(aluBLsb, &IC_74xx541_t::SignalY7Updated, bits4, &IC_74xx283_t::ProcessB3Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY0Updated, bits8, &IC_74xx283_t::ProcessB0Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY1Updated, bits8, &IC_74xx283_t::ProcessB1Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY2Updated, bits8, &IC_74xx283_t::ProcessB2Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY3Updated, bits8, &IC_74xx283_t::ProcessB3Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY4Updated, bitsc, &IC_74xx283_t::ProcessB0Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY5Updated, bitsc, &IC_74xx283_t::ProcessB1Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY6Updated, bitsc, &IC_74xx283_t::ProcessB2Update, CNN_TYPE);
    connect(aluBMsb, &IC_74xx541_t::SignalY7Updated, bitsc, &IC_74xx283_t::ProcessB3Update, CNN_TYPE);


    // -- interconnects between the ICs
    connect(bits0, &IC_74xx283_t::SignalCOutUpdated, bits4, &IC_74xx283_t::ProcessCInUpdate, CNN_TYPE);
    connect(bits4, &IC_74xx283_t::SignalCOutUpdated, bits8, &IC_74xx283_t::ProcessCInUpdate, CNN_TYPE);
    connect(bits8, &IC_74xx283_t::SignalCOutUpdated, bitsc, &IC_74xx283_t::ProcessCInUpdate, CNN_TYPE);


    // -- connect the adder outputs to the Bus Driver
    connect(bits0, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBit0, CNN_TYPE);
    connect(bits0, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit1, CNN_TYPE);
    connect(bits0, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBit2, CNN_TYPE);
    connect(bits0, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBit3, CNN_TYPE);
    connect(bits4, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBit4, CNN_TYPE);
    connect(bits4, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit5, CNN_TYPE);
    connect(bits4, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBit6, CNN_TYPE);
    connect(bits4, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBit7, CNN_TYPE);
    connect(bits8, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBit8, CNN_TYPE);
    connect(bits8, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBit9, CNN_TYPE);
    connect(bits8, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBitA, CNN_TYPE);
    connect(bits8, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBitB, CNN_TYPE);
    connect(bitsc, &IC_74xx283_t::SignalS0Updated, driver, &HW_BusDriver_t::ProcessUpdateBitC, CNN_TYPE);
    connect(bitsc, &IC_74xx283_t::SignalS1Updated, driver, &HW_BusDriver_t::ProcessUpdateBitD, CNN_TYPE);
    connect(bitsc, &IC_74xx283_t::SignalS2Updated, driver, &HW_BusDriver_t::ProcessUpdateBitE, CNN_TYPE);
    connect(bitsc, &IC_74xx283_t::SignalS3Updated, driver, &HW_BusDriver_t::ProcessUpdateBitF, CNN_TYPE);


    // -- handle the carry output
    connect(bitsc, &IC_74xx283_t::SignalCOutUpdated, this, &HW_AluAdder_t::SignalCarryOutUpdated, CNN_TYPE);


    // -- here are some temporary connections, which will be replaced later
    bits0->ProcessCInUpdate(LOW);
    driver->ProcessUpdateOE1(HIGH);     // turn off the automatic assert to Main
    driver->ProcessUpdateOE2(LOW);


    TriggerFirstUpdate();
}



//
// -- Handle the initial updates to put everything in order
//    -----------------------------------------------------
void HW_AluAdder_t::TriggerFirstUpdate(void)
{
    bits0->TriggerFirstUpdate();
    bits4->TriggerFirstUpdate();
    bits8->TriggerFirstUpdate();
    bitsc->TriggerFirstUpdate();
}


