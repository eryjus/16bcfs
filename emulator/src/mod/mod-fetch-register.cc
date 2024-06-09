//===================================================================================================================
//  mod-fetch-register.cc -- This class represents the entirety of a fetch register
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-26  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-fetch-register.moc.cc"


#define TEST


//
// -- construct a new General Purpose Register
//    ----------------------------------------
FetchRegisterModule_t::FetchRegisterModule_t(void) : QGroupBox("Fetch")
{
    setFixedWidth(190);
    setFixedHeight(120);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();
}


//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void FetchRegisterModule_t::AllocateComponents(void)
{
    instrRegBus0 = new IC_74xx574_t;
    instrRegBus1 = new IC_74xx574_t;
    aluB0 = new IC_74xx574_t;
    aluB1 = new IC_74xx574_t;
    addr20 = new IC_74xx574_t;
    addr21 = new IC_74xx574_t;
    main0 = new IC_74xx574_t;
    main1 = new IC_74xx574_t;
    led0 = new IC_74xx574_t;
    led1 = new IC_74xx574_t;
    and1 = new IC_74xx08_t;
    inv1 = new IC_74xx04_t;
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
    assertMain = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAluB = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAddr2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    instrSuppress = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    fetchSuppress = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);

// TODO: Remove testing rig below.
    assertMainTest = new GUI_DipSwitch_t;
    assertAluBTest = new GUI_DipSwitch_t;
    assertAddr2Test = new GUI_DipSwitch_t;
    instrSuppressTest = new GUI_DipSwitch_t;
    fetchSuppressTest = new GUI_DipSwitch_t;
}


//
// -- Put the GUI components together
//    -------------------------------
void FetchRegisterModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter);
    layout->setSpacing(1);
    setLayout(layout);


    // -- Create a new Group Box for the control signals
    QGroupBox *controls = new QGroupBox;
    controls->setAlignment(Qt::AlignHCenter);
    controls->setContentsMargins(0, 0, 0, 0);
    controls->setFixedWidth(180);
    controls->setFixedHeight(50);

    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);
    controlLayout->setColumnMinimumWidth(0, 18);
    controlLayout->setColumnMinimumWidth(1, 18);
    controlLayout->setColumnMinimumWidth(2, 18);
    controlLayout->setColumnMinimumWidth(3, 18);
    controlLayout->setColumnMinimumWidth(4, 18);
    controlLayout->setColumnMinimumWidth(5, 18);
    controlLayout->setColumnMinimumWidth(6, 18);
    controlLayout->setColumnMinimumWidth(7, 18);
    controlLayout->setColumnMinimumWidth(8, 18);

    controlLayout->addWidget(new QLabel("Mn"), 1, 0, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("AB"), 1, 2, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("A2"), 1, 4, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("IS"), 1, 6, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("FS"), 1, 8, Qt::AlignHCenter);

#ifdef TEST
    controlLayout->addWidget(assertMainTest, 0, 0, Qt::AlignHCenter);
    controlLayout->addWidget(assertAluBTest, 0, 2, Qt::AlignHCenter);
    controlLayout->addWidget(assertAddr2Test, 0, 4, Qt::AlignHCenter);
    controlLayout->addWidget(instrSuppressTest, 0, 6, Qt::AlignHCenter);
    controlLayout->addWidget(fetchSuppressTest, 0, 8, Qt::AlignHCenter);
#else
    controlLayout->addWidget(assertMain, 0, 0, Qt::AlignHCenter);
    controlLayout->addWidget(assertAluB, 0, 2, Qt::AlignHCenter);
    controlLayout->addWidget(assertAddr2, 0, 4, Qt::AlignHCenter);
    controlLayout->addWidget(instrSuppress, 0, 6, Qt::AlignHCenter);
    controlLayout->addWidget(fetchSuppress, 0, 8, Qt::AlignHCenter);
#endif


    controls->setLayout(controlLayout);
    layout->addWidget(controls);


    // -- Create a new Group Box for the register contents
    QGroupBox *contents = new QGroupBox;
    contents->setAlignment(Qt::AlignHCenter);
    contents->setFixedWidth(180);
    contents->setFixedHeight(40);

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

    contents->setLayout(contentsLayout);
    layout->addWidget(contents);
}


//
// -- Trigger all the proper initial updates
//    --------------------------------------
void FetchRegisterModule_t::TriggerFirstUpdate(void)
{
    aluB0->TriggerFirstUpdate();
    aluB1->TriggerFirstUpdate();
    addr20->TriggerFirstUpdate();
    addr21->TriggerFirstUpdate();
    main0->TriggerFirstUpdate();
    main1->TriggerFirstUpdate();
    instrRegBus0->TriggerFirstUpdate();
    instrRegBus1->TriggerFirstUpdate();
    led0->TriggerFirstUpdate();
    led1->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    inv1->TriggerFirstUpdate();

    assertMainTest->setValue(1);
    assertAluBTest->setValue(1);
    assertAddr2Test->setValue(1);
    instrSuppressTest->setValue(1);
    fetchSuppressTest->setValue(1);

    assertMainTest->setValue(0);
    assertAluBTest->setValue(0);
    assertAddr2Test->setValue(0);
    instrSuppressTest->setValue(0);
    fetchSuppressTest->setValue(0);
}



//
// -- complete the wire-up for this module
//    ------------------------------------
void FetchRegisterModule_t::WireUp(void)
{
    // -- start with the inputs to the 74xx08 AND gate
    connect(inv1, &IC_74xx04_t::SignalY4Updated, and1, &IC_74xx08_t::ProcessUpdateA1);
    //    B1 -- CLK -- handled in header
    and1->ProcessA2Low();
    and1->ProcessB2Low();
    and1->ProcessA3Low();
    and1->ProcessB3Low();
    and1->ProcessA4Low();
    and1->ProcessB4Low();


    // -- the 74xx04 inverter
    //    A1 -- Main Assert -- handled below
    //    A2 -- ALUB Assert -- handled below
    //    A3 -- Addr2 Assert -- handled below
    //    A4 -- InstrSuppress -- handled below
    inv1->ProcessA5Low();
    inv1->ProcessA6Low();


//    HW_Bus_16_t *fetch = HW_Computer_t::GetFetchBus();
// TODO: revert this back to the fetch bus
    HW_Bus_16_t *fetch = HW_Computer_t::GetAddr1Bus();

    // -- instruction Bus LSB
    // pin 1 (OE) -- handled below
    connect(fetch, &HW_Bus_16_t::SignalBit0Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit1Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBit2Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBit3Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBit4Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBit5Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBit6Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBit7Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, instrRegBus0, &IC_74xx574_t::ProcessUpdateClk);


    // -- instruction Bus MSB
    // pin 1 (OE) -- handled below
    connect(fetch, &HW_Bus_16_t::SignalBit8Updated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit9Updated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBitAUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBitBUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBitCUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBitDUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBitEUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBitFUpdated, instrRegBus1, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, instrRegBus1, &IC_74xx574_t::ProcessUpdateClk);


    // -- main Bus LSB
    connect(inv1, &IC_74xx04_t::SignalY1Updated, main0, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit0Updated, main0, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit1Updated, main0, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBit2Updated, main0, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBit3Updated, main0, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBit4Updated, main0, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBit5Updated, main0, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBit6Updated, main0, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBit7Updated, main0, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, main0, &IC_74xx574_t::ProcessUpdateClk);


    // -- main Bus MSB
    connect(inv1, &IC_74xx04_t::SignalY1Updated, main1, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit8Updated, main1, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit9Updated, main1, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBitAUpdated, main1, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBitBUpdated, main1, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBitCUpdated, main1, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBitDUpdated, main1, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBitEUpdated, main1, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBitFUpdated, main1, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, main1, &IC_74xx574_t::ProcessUpdateClk);


    // -- ALU B LSB
    connect(inv1, &IC_74xx04_t::SignalY2Updated, aluB0, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit0Updated, aluB0, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit1Updated, aluB0, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBit2Updated, aluB0, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBit3Updated, aluB0, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBit4Updated, aluB0, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBit5Updated, aluB0, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBit6Updated, aluB0, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBit7Updated, aluB0, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, aluB0, &IC_74xx574_t::ProcessUpdateClk);


    // -- ALU B MSB
    connect(inv1, &IC_74xx04_t::SignalY2Updated, aluB1, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit8Updated, aluB1, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit9Updated, aluB1, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBitAUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBitBUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBitCUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBitDUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBitEUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBitFUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, aluB1, &IC_74xx574_t::ProcessUpdateClk);


    // -- Addr2 LSB
    connect(inv1, &IC_74xx04_t::SignalY3Updated, addr20, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit0Updated, addr20, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit1Updated, addr20, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBit2Updated, addr20, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBit3Updated, addr20, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBit4Updated, addr20, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBit5Updated, addr20, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBit6Updated, addr20, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBit7Updated, addr20, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, addr20, &IC_74xx574_t::ProcessUpdateClk);


    // -- Addr2 MSB
    connect(inv1, &IC_74xx04_t::SignalY3Updated, addr21, &IC_74xx574_t::ProcessUpdateOE);
    connect(fetch, &HW_Bus_16_t::SignalBit8Updated, addr21, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit9Updated, addr21, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBitAUpdated, addr21, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBitBUpdated, addr21, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBitCUpdated, addr21, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBitDUpdated, addr21, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBitEUpdated, addr21, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBitFUpdated, addr21, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, addr21, &IC_74xx574_t::ProcessUpdateClk);


    // -- Led LSB
    led0->ProcessUpdateOE(LOW);
    connect(fetch, &HW_Bus_16_t::SignalBit0Updated, led0, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit1Updated, led0, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBit2Updated, led0, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBit3Updated, led0, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBit4Updated, led0, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBit5Updated, led0, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBit6Updated, led0, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBit7Updated, led0, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, led0, &IC_74xx574_t::ProcessUpdateClk);


    // -- Led MSB
    led1->ProcessUpdateOE(LOW);
    connect(fetch, &HW_Bus_16_t::SignalBit8Updated, led1, &IC_74xx574_t::ProcessUpdateD1);   // lsb
    connect(fetch, &HW_Bus_16_t::SignalBit9Updated, led1, &IC_74xx574_t::ProcessUpdateD2);
    connect(fetch, &HW_Bus_16_t::SignalBitAUpdated, led1, &IC_74xx574_t::ProcessUpdateD3);
    connect(fetch, &HW_Bus_16_t::SignalBitBUpdated, led1, &IC_74xx574_t::ProcessUpdateD4);
    connect(fetch, &HW_Bus_16_t::SignalBitCUpdated, led1, &IC_74xx574_t::ProcessUpdateD5);
    connect(fetch, &HW_Bus_16_t::SignalBitDUpdated, led1, &IC_74xx574_t::ProcessUpdateD6);
    connect(fetch, &HW_Bus_16_t::SignalBitEUpdated, led1, &IC_74xx574_t::ProcessUpdateD7);
    connect(fetch, &HW_Bus_16_t::SignalBitFUpdated, led1, &IC_74xx574_t::ProcessUpdateD8);   // msb
    connect(and1, &IC_74xx08_t::SignalY1Updated, led1, &IC_74xx574_t::ProcessUpdateClk);


    // -- connect contents the LED inputs
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


    // -- output to the instruction bus
    HW_Bus_16_t *instrBus = HW_Computer_t::Get()->GetInstrBus();
    connect(instrRegBus0, &IC_74xx574_t::SignalQ1Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ2Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ3Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ4Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ5Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ6Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ7Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(instrRegBus0, &IC_74xx574_t::SignalQ8Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ1Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ2Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ3Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ4Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ5Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ6Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ7Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(instrRegBus1, &IC_74xx574_t::SignalQ8Updated, instrBus, &HW_Bus_16_t::ProcessUpdateBitF);



    // -- Connect up to the Main Bus with the main bus assert
    HW_Bus_16_t *mainBus = HW_Computer_t::Get()->GetMainBus();
    connect(main0, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(main0, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(main0, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(main0, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(main0, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(main0, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(main0, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(main0, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(main1, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(main1, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(main1, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(main1, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(main1, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(main1, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(main1, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(main1, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitF);



    // -- Connect up to the ALU B Bus
    HW_Bus_16_t *aluBBus = HW_Computer_t::Get()->GetAluBBus();
    connect(aluB0, &IC_74xx574_t::SignalQ1Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(aluB0, &IC_74xx574_t::SignalQ2Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(aluB0, &IC_74xx574_t::SignalQ3Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(aluB0, &IC_74xx574_t::SignalQ4Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(aluB0, &IC_74xx574_t::SignalQ5Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(aluB0, &IC_74xx574_t::SignalQ6Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(aluB0, &IC_74xx574_t::SignalQ7Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(aluB0, &IC_74xx574_t::SignalQ8Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(aluB1, &IC_74xx574_t::SignalQ1Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(aluB1, &IC_74xx574_t::SignalQ2Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(aluB1, &IC_74xx574_t::SignalQ3Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(aluB1, &IC_74xx574_t::SignalQ4Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(aluB1, &IC_74xx574_t::SignalQ5Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(aluB1, &IC_74xx574_t::SignalQ6Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(aluB1, &IC_74xx574_t::SignalQ7Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(aluB1, &IC_74xx574_t::SignalQ8Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitF);


    // -- Connect up to the Addr2 Bus
    HW_Bus_16_t *addr2Bus = HW_Computer_t::Get()->GetAddr2Bus();
    connect(addr20, &IC_74xx574_t::SignalQ1Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(addr20, &IC_74xx574_t::SignalQ2Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(addr20, &IC_74xx574_t::SignalQ3Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(addr20, &IC_74xx574_t::SignalQ4Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(addr20, &IC_74xx574_t::SignalQ5Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(addr20, &IC_74xx574_t::SignalQ6Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(addr20, &IC_74xx574_t::SignalQ7Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(addr20, &IC_74xx574_t::SignalQ8Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(addr21, &IC_74xx574_t::SignalQ1Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(addr21, &IC_74xx574_t::SignalQ2Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(addr21, &IC_74xx574_t::SignalQ3Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(addr21, &IC_74xx574_t::SignalQ4Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(addr21, &IC_74xx574_t::SignalQ5Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(addr21, &IC_74xx574_t::SignalQ6Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(addr21, &IC_74xx574_t::SignalQ7Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitE);
    connect(addr21, &IC_74xx574_t::SignalQ8Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitF);


    //
    // -- Finally, we need a clock input
    //    ------------------------------
    ClockModule_t *clk = HW_Computer_t::GetClock();
    connect(clk, &ClockModule_t::SignalClockState, this, &FetchRegisterModule_t::ProcessClk);


// TODO: remove debugging code here
    connect(assertMainTest, &GUI_DipSwitch_t::SignalSwitchChanged, this, &FetchRegisterModule_t::ProcessAssertMain);
    connect(assertAluBTest, &GUI_DipSwitch_t::SignalSwitchChanged, this, &FetchRegisterModule_t::ProcessAssertAluB);
    connect(assertAddr2Test, &GUI_DipSwitch_t::SignalSwitchChanged, this, &FetchRegisterModule_t::ProcessAssertAddr2);
    connect(instrSuppressTest, &GUI_DipSwitch_t::SignalSwitchChanged, this, &FetchRegisterModule_t::ProcessInstructionSuppress);
    connect(fetchSuppressTest, &GUI_DipSwitch_t::SignalSwitchChanged, this, &FetchRegisterModule_t::ProcessFetchSuppress);
}


//
// -- Process Assert Main
//    -------------------
void FetchRegisterModule_t::ProcessAssertMain(TriState_t state)
{
    inv1->ProcessUpdateA1(state);
    assertMain->ProcessStateChange(state);
}


//
// -- Process Assert ALU B
//    --------------------
void FetchRegisterModule_t::ProcessAssertAluB(TriState_t state)
{
    inv1->ProcessUpdateA2(state);
    assertAluB->ProcessStateChange(state);
}


//
// -- Process Assert Addr2
//    --------------------
void FetchRegisterModule_t::ProcessAssertAddr2(TriState_t state)
{
    inv1->ProcessUpdateA3(state);
    assertAddr2->ProcessStateChange(state);
}


//
// -- Process Fetch Suppress
//    ----------------------
void FetchRegisterModule_t::ProcessFetchSuppress(TriState_t state)
{
    inv1->ProcessUpdateA4(state);
    fetchSuppress->ProcessStateChange(state);
}


//
// -- Suppress the assert to the instruction register
//    -----------------------------------------------
void FetchRegisterModule_t::ProcessInstructionSuppress(TriState_t state)
{
    instrRegBus0->ProcessUpdateOE(state);
    instrRegBus1->ProcessUpdateOE(state);
    instrSuppress->ProcessStateChange(state);
}
