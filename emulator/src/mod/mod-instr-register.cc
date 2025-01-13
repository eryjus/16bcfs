//===================================================================================================================
//  mod-instr-register.cc -- This class represents the entirety of a instruction register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jun-17  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-instr-register.moc.cc"



//
// -- construct a new General Purpose Register
//    ----------------------------------------
InstructionRegisterModule_t::InstructionRegisterModule_t(void) : QGroupBox("Instruction")
{
    setFixedWidth(190);
    setFixedHeight(55);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();
}


//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void InstructionRegisterModule_t::AllocateComponents(void)
{
    led0 = new IC_74xx574_t;
    led1 = new IC_74xx574_t;

    bit0 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit3 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit4 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit5 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit6 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit7 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit8 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bit9 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitA = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitB = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitC = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitD = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitE = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    bitF = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
}


//
// -- Put the GUI components together
//    -------------------------------
void InstructionRegisterModule_t::BuildGui(void)
{
    setAlignment(Qt::AlignHCenter);


    // -- Create a new Group Box for the register contents
    QHBoxLayout *contentsLayout = new QHBoxLayout;
    contentsLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setSpacing(1);

    contentsLayout->addWidget(bitF, Qt::AlignHCenter);
    contentsLayout->addWidget(bitE, Qt::AlignHCenter);
    contentsLayout->addWidget(bitD, Qt::AlignHCenter);
    contentsLayout->addWidget(bitC, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bitB, Qt::AlignHCenter);
    contentsLayout->addWidget(bitA, Qt::AlignHCenter);
    contentsLayout->addWidget(bit9, Qt::AlignHCenter);
    contentsLayout->addWidget(bit8, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bit7, Qt::AlignHCenter);
    contentsLayout->addWidget(bit6, Qt::AlignHCenter);
    contentsLayout->addWidget(bit5, Qt::AlignHCenter);
    contentsLayout->addWidget(bit4, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(bit3, Qt::AlignHCenter);
    contentsLayout->addWidget(bit2, Qt::AlignHCenter);
    contentsLayout->addWidget(bit1, Qt::AlignHCenter);
    contentsLayout->addWidget(bit0, Qt::AlignHCenter);

    setLayout(contentsLayout);
}


//
// -- Trigger all the proper initial updates
//    --------------------------------------
void InstructionRegisterModule_t::TriggerFirstUpdate(void)
{
    led0->TriggerFirstUpdate();
    led1->TriggerFirstUpdate();
}



//
// -- complete the wire-up for this module
//    ------------------------------------
void InstructionRegisterModule_t::WireUp(void)
{
    HW_Bus_16_t *instr = HW_Computer_t::GetInstrBus();

    // -- Connect the inputs for the LED latches
    led0->ProcessUpdateOE(LOW);
    connect(instr, &HW_Bus_16_t::SignalBit0Updated, led0, &IC_74xx574_t::ProcessUpdateD1);
    connect(instr, &HW_Bus_16_t::SignalBit1Updated, led0, &IC_74xx574_t::ProcessUpdateD2);
    connect(instr, &HW_Bus_16_t::SignalBit2Updated, led0, &IC_74xx574_t::ProcessUpdateD3);
    connect(instr, &HW_Bus_16_t::SignalBit3Updated, led0, &IC_74xx574_t::ProcessUpdateD4);
    connect(instr, &HW_Bus_16_t::SignalBit4Updated, led0, &IC_74xx574_t::ProcessUpdateD5);
    connect(instr, &HW_Bus_16_t::SignalBit5Updated, led0, &IC_74xx574_t::ProcessUpdateD6);
    connect(instr, &HW_Bus_16_t::SignalBit6Updated, led0, &IC_74xx574_t::ProcessUpdateD7);
    connect(instr, &HW_Bus_16_t::SignalBit7Updated, led0, &IC_74xx574_t::ProcessUpdateD8);
    // -- clock handled below


    led1->ProcessUpdateOE(LOW);
    connect(instr, &HW_Bus_16_t::SignalBit8Updated, led1, &IC_74xx574_t::ProcessUpdateD1);
    connect(instr, &HW_Bus_16_t::SignalBit9Updated, led1, &IC_74xx574_t::ProcessUpdateD2);
    connect(instr, &HW_Bus_16_t::SignalBitAUpdated, led1, &IC_74xx574_t::ProcessUpdateD3);
    connect(instr, &HW_Bus_16_t::SignalBitBUpdated, led1, &IC_74xx574_t::ProcessUpdateD4);
    connect(instr, &HW_Bus_16_t::SignalBitCUpdated, led1, &IC_74xx574_t::ProcessUpdateD5);
    connect(instr, &HW_Bus_16_t::SignalBitDUpdated, led1, &IC_74xx574_t::ProcessUpdateD6);
    connect(instr, &HW_Bus_16_t::SignalBitEUpdated, led1, &IC_74xx574_t::ProcessUpdateD7);
    connect(instr, &HW_Bus_16_t::SignalBitFUpdated, led1, &IC_74xx574_t::ProcessUpdateD8);
    // -- clock handled below

    // -- connect up the LEDs
    connect(led0, &IC_74xx574_t::SignalQ1Updated, bit0, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ2Updated, bit1, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ3Updated, bit2, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ4Updated, bit3, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ5Updated, bit4, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ6Updated, bit5, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ7Updated, bit6, &GUI_Led_t::ProcessStateChange);
    connect(led0, &IC_74xx574_t::SignalQ8Updated, bit7, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ1Updated, bit8, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ2Updated, bit9, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ3Updated, bitA, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ4Updated, bitB, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ5Updated, bitC, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ6Updated, bitD, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ7Updated, bitE, &GUI_Led_t::ProcessStateChange);
    connect(led1, &IC_74xx574_t::SignalQ8Updated, bitF, &GUI_Led_t::ProcessStateChange);

    //
    // -- Finally, we need a clock input
    //    ------------------------------
    ClockModule_t *clk = HW_Computer_t::GetClock();
    connect(clk, &ClockModule_t::SignalCpuClockLatch, this, &InstructionRegisterModule_t::ProcessClockLatch);
    connect(clk, &ClockModule_t::SignalCpuClockOutput, this, &InstructionRegisterModule_t::ProcessClockOutput);
}


//
// -- Handle the clock tick
//    ---------------------
void InstructionRegisterModule_t::ProcessClockLatch(TriState_t state)
{
    led0->ProcessUpdateClockLatch(state);
    led1->ProcessUpdateClockLatch(state);
}



//
// -- Handle the clock tick
//    ---------------------
void InstructionRegisterModule_t::ProcessClockOutput(TriState_t state)
{
    led0->ProcessUpdateClockOutput(state);
    led1->ProcessUpdateClockOutput(state);
}


