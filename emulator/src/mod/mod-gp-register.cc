//===================================================================================================================
//  mod-gp-register.cc -- This class represents the entirety of a general purpose register
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-26  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-gp-register.moc.cc"


//
// -- construct a new General Purpose Register
//    ----------------------------------------
GpRegisterModule_t::GpRegisterModule_t(const QString name) : QGroupBox(name)
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
void GpRegisterModule_t::AllocateComponents(void)
{
    counter0 = new IC_74xx193_t;
    counter1 = new IC_74xx193_t;
    counter2 = new IC_74xx193_t;
    counter3 = new IC_74xx193_t;
    aluA0 = new IC_74xx574_t;
    aluA1 = new IC_74xx574_t;
    aluB0 = new IC_74xx574_t;
    aluB1 = new IC_74xx574_t;
    addr10 = new IC_74xx574_t;
    addr11 = new IC_74xx574_t;
    addr20 = new IC_74xx574_t;
    addr21 = new IC_74xx574_t;
    main0 = new IC_74xx574_t;
    main1 = new IC_74xx574_t;
    swap0 = new IC_74xx574_t;
    swap1 = new IC_74xx574_t;
    led0 = new IC_74xx574_t;
    led1 = new IC_74xx574_t;
    nand1 = new IC_74xx00_t;
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
    load = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    inc = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    dec = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertSwap = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertMain = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAluA = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAluB = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAddr1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    assertAddr2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
}


//
// -- Put the GUI components together
//    -------------------------------
void GpRegisterModule_t::BuildGui(void)
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

    controlLayout->addWidget(load, 0, 0, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("ld"), 1, 0, Qt::AlignHCenter);
    controlLayout->addWidget(inc, 0, 1, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("inc"), 1, 1, Qt::AlignHCenter);
    controlLayout->addWidget(dec, 0, 2, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("dec"), 1, 2, Qt::AlignHCenter);
    controlLayout->addWidget(assertSwap, 0, 3, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("Sw"), 1, 3, Qt::AlignHCenter);
    controlLayout->addWidget(assertMain, 0, 4, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("Mn"), 1, 4, Qt::AlignHCenter);
    controlLayout->addWidget(assertAluA, 0, 5, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("AA"), 1, 5, Qt::AlignHCenter);
    controlLayout->addWidget(assertAluB, 0, 6, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("AB"), 1, 6, Qt::AlignHCenter);
    controlLayout->addWidget(assertAddr1, 0, 7, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("A1"), 1, 7, Qt::AlignHCenter);
    controlLayout->addWidget(assertAddr2, 0, 8, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("A2"), 1, 8, Qt::AlignHCenter);


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
void GpRegisterModule_t::TriggerFirstUpdate(void)
{
    counter0->TriggerFirstUpdate();
    counter1->TriggerFirstUpdate();
    counter2->TriggerFirstUpdate();
    counter3->TriggerFirstUpdate();
    aluA0->TriggerFirstUpdate();
    aluA1->TriggerFirstUpdate();
    aluB0->TriggerFirstUpdate();
    aluB1->TriggerFirstUpdate();
    addr10->TriggerFirstUpdate();
    addr11->TriggerFirstUpdate();
    addr20->TriggerFirstUpdate();
    addr21->TriggerFirstUpdate();
    main0->TriggerFirstUpdate();
    main1->TriggerFirstUpdate();
    swap0->TriggerFirstUpdate();
    swap1->TriggerFirstUpdate();
    led0->TriggerFirstUpdate();
    led1->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
    inv1->TriggerFirstUpdate();
}



//
// -- complete the wire-up for this module
//    ------------------------------------
void GpRegisterModule_t::WireUp(void)
{
    // -- start with the inputs to the 74xx00 NAND gate
    //    A1 -- CLK
    //    B1 -- INC
    //    A2 -- CLK
    //    B2 -- DEC
    //    A3 -- LOAD
    //    B3 -- LOAD (used as an inverter)
    //    A4 -- #RESET
    //    B4 -- #RESET (used as an inverter)


    // -- the 74xx04 inverter
    //    A1 -- Swap
    //    A2 -- Main
    //    A3 -- ALUA
    //    A4 -- ALUB
    //    A5 -- Addr1
    //    A6 -- Addr2


    // -- 74xx193 counter 0
    //    B -- bit 1
    connect(nand1, &IC_74xx00_t::SignalY2Updated, counter0, &IC_74xx193_t::ProcessUpdateDown, CNN_TYPE);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, counter0, &IC_74xx193_t::ProcessUpdateUp, CNN_TYPE);
    //    D -- bit 3
    //    C -- bit 2
    connect(nand1, &IC_74xx00_t::SignalY3Updated, counter0, &IC_74xx193_t::ProcessUpdateLoad, CNN_TYPE);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, counter0, &IC_74xx193_t::ProcessUpdateClr, CNN_TYPE);
    //    A -- bit 0


    // -- 74xx193 counter 1
    //    B -- bit 5
    connect(counter0, &IC_74xx193_t::SignalBoUpdated, counter1, &IC_74xx193_t::ProcessUpdateDown, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalCoUpdated, counter1, &IC_74xx193_t::ProcessUpdateUp, CNN_TYPE);
    //    D -- bit 7
    //    C -- bit 6
    connect(nand1, &IC_74xx00_t::SignalY3Updated, counter1, &IC_74xx193_t::ProcessUpdateLoad, CNN_TYPE);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, counter1, &IC_74xx193_t::ProcessUpdateClr, CNN_TYPE);
    //    A -- bit 4


    // -- 74xx193 counter 2
    //    B -- bit 5
    connect(counter1, &IC_74xx193_t::SignalBoUpdated, counter2, &IC_74xx193_t::ProcessUpdateDown, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalCoUpdated, counter2, &IC_74xx193_t::ProcessUpdateUp, CNN_TYPE);
    //    D -- bit 7
    //    C -- bit 6
    connect(nand1, &IC_74xx00_t::SignalY3Updated, counter2, &IC_74xx193_t::ProcessUpdateLoad, CNN_TYPE);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, counter2, &IC_74xx193_t::ProcessUpdateClr, CNN_TYPE);
    //    A -- bit 4


    // -- 74xx193 counter 3
    //    B -- bit 5
    connect(counter2, &IC_74xx193_t::SignalBoUpdated, counter3, &IC_74xx193_t::ProcessUpdateDown, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalCoUpdated, counter3, &IC_74xx193_t::ProcessUpdateUp, CNN_TYPE);
    //    D -- bit 7
    //    C -- bit 6
    connect(nand1, &IC_74xx00_t::SignalY3Updated, counter3, &IC_74xx193_t::ProcessUpdateLoad, CNN_TYPE);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, counter3, &IC_74xx193_t::ProcessUpdateClr, CNN_TYPE);
    //    A -- bit 4


    // -- swap Bus MSB
    connect(inv1, &IC_74xx04_t::SignalY1Updated, swap1, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, swap1, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, swap1, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, swap1, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, swap1, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, swap1, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, swap1, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, swap1, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, swap1, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- swap Bus LSB
    connect(inv1, &IC_74xx04_t::SignalY1Updated, swap0, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, swap0, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, swap0, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, swap0, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, swap0, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, swap0, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, swap0, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, swap0, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, swap0, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- main Bus MSB
    connect(inv1, &IC_74xx04_t::SignalY2Updated, main1, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, main1, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, main1, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, main1, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, main1, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, main1, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, main1, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, main1, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, main1, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- main Bus LSB
    connect(inv1, &IC_74xx04_t::SignalY2Updated, main0, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, main0, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, main0, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, main0, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, main0, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, main0, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, main0, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, main0, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, main0, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- ALU A MSB
    connect(inv1, &IC_74xx04_t::SignalY3Updated, aluA1, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, aluA1, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- ALU A LSB
    connect(inv1, &IC_74xx04_t::SignalY3Updated, aluA0, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, aluA0, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- ALU B MSB
    connect(inv1, &IC_74xx04_t::SignalY4Updated, aluB1, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, aluB1, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- ALU B LSB
    connect(inv1, &IC_74xx04_t::SignalY4Updated, aluB0, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, aluB0, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Addr1 MSB
    connect(inv1, &IC_74xx04_t::SignalY5Updated, addr11, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, addr11, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, addr11, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, addr11, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, addr11, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, addr11, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, addr11, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, addr11, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, addr11, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Addr1 LSB
    connect(inv1, &IC_74xx04_t::SignalY5Updated, addr10, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, addr10, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, addr10, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, addr10, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, addr10, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, addr10, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, addr10, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, addr10, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, addr10, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Addr2 MSB
    connect(inv1, &IC_74xx04_t::SignalY6Updated, addr21, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, addr21, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, addr21, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, addr21, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, addr21, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, addr21, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, addr21, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, addr21, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, addr21, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Addr2 LSB
    connect(inv1, &IC_74xx04_t::SignalY6Updated, addr20, &IC_74xx574_t::ProcessUpdateOE, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, addr20, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, addr20, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, addr20, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, addr20, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, addr20, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, addr20, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, addr20, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, addr20, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Led MSB
    led1->ProcessUpdateOE(LOW);
    connect(counter2, &IC_74xx193_t::SignalQaUpdated, led1, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter2, &IC_74xx193_t::SignalQbUpdated, led1, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQcUpdated, led1, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter2, &IC_74xx193_t::SignalQdUpdated, led1, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQaUpdated, led1, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQbUpdated, led1, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQcUpdated, led1, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter3, &IC_74xx193_t::SignalQdUpdated, led1, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- Led LSB
    led0->ProcessUpdateOE(LOW);
    connect(counter0, &IC_74xx193_t::SignalQaUpdated, led0, &IC_74xx574_t::ProcessUpdateD8, CNN_TYPE);   // lsb
    connect(counter0, &IC_74xx193_t::SignalQbUpdated, led0, &IC_74xx574_t::ProcessUpdateD7, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQcUpdated, led0, &IC_74xx574_t::ProcessUpdateD6, CNN_TYPE);
    connect(counter0, &IC_74xx193_t::SignalQdUpdated, led0, &IC_74xx574_t::ProcessUpdateD5, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQaUpdated, led0, &IC_74xx574_t::ProcessUpdateD4, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQbUpdated, led0, &IC_74xx574_t::ProcessUpdateD3, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQcUpdated, led0, &IC_74xx574_t::ProcessUpdateD2, CNN_TYPE);
    connect(counter1, &IC_74xx193_t::SignalQdUpdated, led0, &IC_74xx574_t::ProcessUpdateD1, CNN_TYPE);   // msb
    // -- Clock from below


    // -- connect contents the LED inputs
    connect(led0, &IC_74xx574_t::SignalQ8Updated, bit0, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ7Updated, bit1, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ6Updated, bit2, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ5Updated, bit3, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ4Updated, bit4, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ3Updated, bit5, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ2Updated, bit6, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led0, &IC_74xx574_t::SignalQ1Updated, bit7, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ8Updated, bit8, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ7Updated, bit9, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ6Updated, bitA, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ5Updated, bitB, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ4Updated, bitC, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ3Updated, bitD, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ2Updated, bitE, &GUI_Led_t::ProcessStateChange, CNN_TYPE);
    connect(led1, &IC_74xx574_t::SignalQ1Updated, bitF, &GUI_Led_t::ProcessStateChange, CNN_TYPE);


    // -- Control LED inputs are handled below


    // -- connect the module to accept inputs from the main bus
    HW_Bus_16_t *mainBus = HW_Computer_t::Get()->GetMainBus();
    connect(mainBus, &HW_Bus_16_t::SignalBit0Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit0, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit1Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit1, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit2Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit2, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit3Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit3, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit4Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit4, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit5Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit5, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit6Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit6, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit7Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit7, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit8Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit8, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBit9Updated, this, &GpRegisterModule_t::ProcessUpdateMainBit9, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitAUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitA, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitBUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitB, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitCUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitC, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitDUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitD, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitEUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitE, CNN_TYPE);
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, this, &GpRegisterModule_t::ProcessUpdateMainBitF, CNN_TYPE);


    // -- Connect up to the Main Bus with the main bus assert
    connect(main0, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(main0, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(main1, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);


    // -- Connect up to the Main Bus with the swap bus assert (already swapped)
    connect(swap0, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(swap0, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ8Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ7Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ6Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ5Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ4Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ3Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ2Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(swap1, &IC_74xx574_t::SignalQ1Updated, mainBus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);


    // -- Connect up to the ALU A Bus
    HW_Bus_16_t *aluABus = HW_Computer_t::Get()->GetAluABus();
    connect(aluA0, &IC_74xx574_t::SignalQ8Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ7Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ6Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ5Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ4Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ3Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ2Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(aluA0, &IC_74xx574_t::SignalQ1Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ8Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ7Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ6Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ5Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ4Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ3Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ2Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(aluA1, &IC_74xx574_t::SignalQ1Updated, aluABus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);


    // -- Connect up to the ALU B Bus
    HW_Bus_16_t *aluBBus = HW_Computer_t::Get()->GetAluBBus();
    connect(aluB0, &IC_74xx574_t::SignalQ8Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ7Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ6Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ5Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ4Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ3Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ2Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(aluB0, &IC_74xx574_t::SignalQ1Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ8Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ7Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ6Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ5Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ4Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ3Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ2Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(aluB1, &IC_74xx574_t::SignalQ1Updated, aluBBus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);


    // -- Connect up to the Addr1 Bus
    HW_Bus_16_t *addr1Bus = HW_Computer_t::Get()->GetAddr1Bus();
    connect(addr10, &IC_74xx574_t::SignalQ8Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ7Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ6Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ5Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ4Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ3Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ2Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(addr10, &IC_74xx574_t::SignalQ1Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ8Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ7Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ6Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ5Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ4Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ3Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ2Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(addr11, &IC_74xx574_t::SignalQ1Updated, addr1Bus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);


    // -- Connect up to the Addr1 Bus
    HW_Bus_16_t *addr2Bus = HW_Computer_t::Get()->GetAddr2Bus();
    connect(addr20, &IC_74xx574_t::SignalQ8Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit0, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ7Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit1, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ6Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit2, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ5Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit3, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ4Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit4, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ3Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit5, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ2Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit6, CNN_TYPE);
    connect(addr20, &IC_74xx574_t::SignalQ1Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit7, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ8Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit8, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ7Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBit9, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ6Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitA, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ5Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitB, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ4Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitC, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ3Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitD, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ2Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitE, CNN_TYPE);
    connect(addr21, &IC_74xx574_t::SignalQ1Updated, addr2Bus, &HW_Bus_16_t::ProcessUpdateBitF, CNN_TYPE);
}


//
// -- Handle a reset signal
//    ---------------------
void GpRegisterModule_t::ProcessReset(TriState_t state)
{
    nand1->ProcessUpdateA4(state);
    nand1->ProcessUpdateB4(state);
}


//
// -- Handle a clock signal
//    ---------------------
void GpRegisterModule_t::ProcessClk(TriState_t state)
{
    nand1->ProcessUpdateA1(state);
    nand1->ProcessUpdateA2(state);
    aluA0->ProcessUpdateClk(state);
    aluA1->ProcessUpdateClk(state);
    aluB0->ProcessUpdateClk(state);
    aluB1->ProcessUpdateClk(state);
    addr10->ProcessUpdateClk(state);
    addr11->ProcessUpdateClk(state);
    addr20->ProcessUpdateClk(state);
    addr21->ProcessUpdateClk(state);
    main0->ProcessUpdateClk(state);
    main1->ProcessUpdateClk(state);
    swap0->ProcessUpdateClk(state);
    swap1->ProcessUpdateClk(state);
    led0->ProcessUpdateClk(state);
    led1->ProcessUpdateClk(state);
}


//
// -- Handle the load signal
//    ----------------------
void GpRegisterModule_t::ProcessLoad(TriState_t state)
{
    load->ProcessStateChange(state);
    nand1->ProcessUpdateA3(state);
    nand1->ProcessUpdateB3(state);
}


//
// -- Process Increment
//    -----------------
void GpRegisterModule_t::ProcessInc(TriState_t state)
{
    inc->ProcessStateChange(state);
    nand1->ProcessUpdateB1(state);
}


//
// -- Process Decrement
//    -----------------
void GpRegisterModule_t::ProcessDec(TriState_t state)
{
    dec->ProcessStateChange(state);
    nand1->ProcessUpdateB2(state);
}


//
// -- Process Assert Swap
//    -------------------
void GpRegisterModule_t::ProcessAssertSwap(TriState_t state)
{
    assertSwap->ProcessStateChange(state);
    inv1->ProcessUpdateA1(state);
}


//
// -- Process Assert Main
//    -------------------
void GpRegisterModule_t::ProcessAssertMain(TriState_t state)
{
    assertMain->ProcessStateChange(state);
    inv1->ProcessUpdateA2(state);
}


//
// -- Process Assert ALU A
//    --------------------
void GpRegisterModule_t::ProcessAssertAluA(TriState_t state)
{
    assertAluA->ProcessStateChange(state);
    inv1->ProcessUpdateA3(state);
}


//
// -- Process Assert ALU B
//    --------------------
void GpRegisterModule_t::ProcessAssertAluB(TriState_t state)
{
    assertAluB->ProcessStateChange(state);
    inv1->ProcessUpdateA4(state);
}


//
// -- Process Assert Addr1
//    --------------------
void GpRegisterModule_t::ProcessAssertAddr1(TriState_t state)
{
    assertAddr1->ProcessStateChange(state);
    inv1->ProcessUpdateA5(state);
}


//
// -- Process Assert Addr2
//    --------------------
void GpRegisterModule_t::ProcessAssertAddr2(TriState_t state)
{
    assertAddr2->ProcessStateChange(state);
    inv1->ProcessUpdateA6(state);
}


