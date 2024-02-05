//===================================================================================================================
//  mod-ctrl-ctrl.cc -- This class the control circuit for the Control ROMs, loading from EEPROM to SRAM
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This class contains all the control signals needed to load the Control ROM from EEPROM into SRAM.  Since all
//  12 (up to 16) ROMs will be loaded at the same time, it is adventageous to pull all the repetitive components
//  into a single control module.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/mod-ctrl-ctrl.moc.cc"



//
// -- construct a new Control Rom Control Module
//    ------------------------------------------
CtrlRomCtrlModule_t::CtrlRomCtrlModule_t(void) : QGroupBox("Control ROM Control")
{
    setFixedWidth(190);
    setFixedHeight(120);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();

    clock->setInterval(0);
}



//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void CtrlRomCtrlModule_t::AllocateComponents(void)
{
    resetting = new IC_74xx74_t;
    nand1 = new IC_74xx00_t;
    and1 = new IC_74xx08_t;
    oAnd1 = new IC_74xx03_t;
    mux0 = new IC_74xx157_t;
    mux4 = new IC_74xx157_t;
    mux8 = new IC_74xx157_t;
    muxC = new IC_74xx157_t;
    bits = new IC_74xx193_t;
    addr0 = new IC_74xx193_t;
    addr4 = new IC_74xx193_t;
    addr8 = new IC_74xx193_t;
    addrC = new IC_74xx193_t;
    shift = new IC_74xx165_t;
    clock = new HW_Oscillator_t;
    clk = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    qr = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::yellow);
    qc = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::yellow);
    led0 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led3 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led4 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led5 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led6 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led7 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led8 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    led9 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    ledA = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    ledB = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    ledC = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    ledD = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
    ledE = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);

    bit0 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit3 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);


//    addr0->setObjectName("DEBUG");
//    bits->setObjectName("DEBUG");
//    resetting->setObjectName("DEBUG");
//    and1->setObjectName("DEBUG");
}



//
// -- Build the GUI for the Control Rom Control Module
//    ------------------------------------------------
void CtrlRomCtrlModule_t::BuildGui(void)
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

    controlLayout->addWidget(clk, 0, 0, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("clk"), 1, 0, Qt::AlignHCenter);
    controlLayout->addWidget(qr, 0, 1, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("rst"), 1, 1, Qt::AlignHCenter);
    controlLayout->addWidget(qc, 0, 2, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("cpy"), 1, 2, Qt::AlignHCenter);
    controlLayout->addWidget(bit3, 0, 5, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 8 "), 1, 5, Qt::AlignHCenter);
    controlLayout->addWidget(bit2, 0, 6, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 4 "), 1, 6, Qt::AlignHCenter);
    controlLayout->addWidget(bit1, 0, 7, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 2 "), 1, 7, Qt::AlignHCenter);
    controlLayout->addWidget(bit0, 0, 8, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 1 "), 1, 8, Qt::AlignHCenter);


    controls->setLayout(controlLayout);
    layout->addWidget(controls);


    // -- Create a new Group Box for the address contents
    QGroupBox *contents = new QGroupBox;
    contents->setAlignment(Qt::AlignHCenter);
    contents->setFixedWidth(180);
    contents->setFixedHeight(40);

    QHBoxLayout *contentsLayout = new QHBoxLayout;
    contentsLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    contentsLayout->setContentsMargins(0, 0, 0, 0);
    contentsLayout->setSpacing(1);

    contentsLayout->addWidget(ledE, Qt::AlignHCenter);
    contentsLayout->addWidget(ledD, Qt::AlignHCenter);
    contentsLayout->addWidget(ledC, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(ledB, Qt::AlignHCenter);
    contentsLayout->addWidget(ledA, Qt::AlignHCenter);
    contentsLayout->addWidget(led9, Qt::AlignHCenter);
    contentsLayout->addWidget(led8, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(led7, Qt::AlignHCenter);
    contentsLayout->addWidget(led6, Qt::AlignHCenter);
    contentsLayout->addWidget(led5, Qt::AlignHCenter);
    contentsLayout->addWidget(led4, Qt::AlignHCenter);
    contentsLayout->addSpacerItem(new QSpacerItem(4, 10));
    contentsLayout->addWidget(led3, Qt::AlignHCenter);
    contentsLayout->addWidget(led2, Qt::AlignHCenter);
    contentsLayout->addWidget(led1, Qt::AlignHCenter);
    contentsLayout->addWidget(led0, Qt::AlignHCenter);

    contents->setLayout(contentsLayout);
    layout->addWidget(contents);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void CtrlRomCtrlModule_t::TriggerFirstUpdate(void)
{
    resetting->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    oAnd1->TriggerFirstUpdate();
    mux0->TriggerFirstUpdate();
    mux4->TriggerFirstUpdate();
    mux8->TriggerFirstUpdate();
    muxC->TriggerFirstUpdate();
    bits->TriggerFirstUpdate();
    addr0->TriggerFirstUpdate();
    addr4->TriggerFirstUpdate();
    addr8->TriggerFirstUpdate();
    addrC->TriggerFirstUpdate();
    shift->TriggerFirstUpdate();
}



//
// -- Complete the wire-up for the Control ROM Control Module
//    -------------------------------------------------------
void CtrlRomCtrlModule_t::WireUp(void)
{
    //
    // -- start with the 74xx74 resetting latch
    //    -------------------------------------
    resetting->ProcessUpdateClr1(HIGH);
    connect(nand1, &IC_74xx00_t::SignalY3Updated, resetting, &IC_74xx74_t::ProcessUpdateD1);
    connect(clock, &HW_Oscillator_t::SignalStateChanged, resetting, &IC_74xx74_t::ProcessUpdateClk1);
    // pin 4 is handled in ProcessReset
    // pin 5 is Qr
    // pin 6 is Qrb
    // pin 8 is unused
    // pin 9 is unused
    resetting->ProcessUpdatePre2(HIGH);
    resetting->ProcessUpdateClk2(LOW);
    resetting->ProcessUpdateD2(LOW);
    resetting->ProcessUpdateClr2(HIGH);


    //
    // -- wire up the NAND gate as an SR Latch
    //    ------------------------------------
    // pin 1 is handled in ProcessReset
    connect(nand1, &IC_74xx00_t::SignalY2Updated, nand1, &IC_74xx00_t::ProcessUpdateB1);
    // -- pin 3 output is in the next line
    connect(nand1, &IC_74xx00_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateA2);
    connect(addrC, &IC_74xx193_t::SignalCoUpdated, nand1, &IC_74xx00_t::ProcessUpdateB2);
    // pin 6 handled below
    // pin 8 already handled above in 74xx74
    // pin 9 handled in ProcessReset
    // pin 9 handled in ProcessReset
    // pin 11 unused
    nand1->ProcessB4Low();
    nand1->ProcessA4Low();


    //
    // -- wire up the AND gate to handle the Qc*CLK signal
    //    ------------------------------------------------
    connect(nand1, &IC_74xx00_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateA1);
    connect(clock, &HW_Oscillator_t::SignalStateChanged, and1, &IC_74xx08_t::ProcessUpdateB1);
    connect(and1, &IC_74xx08_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::SignalClkAndQcUpdated);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, and1, &IC_74xx08_t::ProcessUpdateA2);
    connect(bits, &IC_74xx193_t::SignalBoUpdated, and1, &IC_74xx08_t::ProcessUpdateB2);
    // pin 6 handled with `bits` below
    // pin 8 unused
    and1->ProcessB3Low();
    and1->ProcessA3Low();
    // pin 11 unused
    and1->ProcessB4Low();
    and1->ProcessA4Low();


    //
    // -- connect the and gate used for #RHLD
    //    -----------------------------------
    connect(nand1, &IC_74xx00_t::SignalY2Updated, oAnd1, &IC_74xx03_t::ProcessUpdateA1);
    connect(nand1, &IC_74xx00_t::SignalY2Updated, oAnd1, &IC_74xx03_t::ProcessUpdateB1);
    connect(oAnd1, &IC_74xx03_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::SignalRHldUpdated);
    oAnd1->ProcessA2High();
    oAnd1->ProcessB2High();
    // pin 6 unused
    // pin 8 unused
    oAnd1->ProcessB3High();
    oAnd1->ProcessA3High();
    // pin 11 unused
    oAnd1->ProcessB4High();
    oAnd1->ProcessA4High();


    //
    // -- connect up the muxes; for that we need the instruction bus
    //    ----------------------------------------------------------
    HW_Bus_t *instrBus = HW_Computer_t::Get()->GetInstrBus();
    HW_Bus_t *ctrlBus = HW_Computer_t::Get()->GetCtrlBus();

    connect(nand1, &IC_74xx00_t::SignalY1Updated, mux0, &IC_74xx157_t::ProcessUpdateAB);
    connect(instrBus, &HW_Bus_t::SignalBit0Updated, mux0, &IC_74xx157_t::ProcessUpdateA1);
    connect(addr0, &IC_74xx193_t::SignalQaUpdated, mux0, &IC_74xx157_t::ProcessUpdateB1);
    connect(mux0, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit0);
    connect(instrBus, &HW_Bus_t::SignalBit1Updated, mux0, &IC_74xx157_t::ProcessUpdateA2);
    connect(addr0, &IC_74xx193_t::SignalQbUpdated, mux0, &IC_74xx157_t::ProcessUpdateB2);
    connect(mux0, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit1);
    connect(mux0, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit2);
    connect(addr0, &IC_74xx193_t::SignalQcUpdated, mux0, &IC_74xx157_t::ProcessUpdateB3);
    connect(instrBus, &HW_Bus_t::SignalBit2Updated, mux0, &IC_74xx157_t::ProcessUpdateA3);
    connect(mux0, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit3);
    connect(addr0, &IC_74xx193_t::SignalQdUpdated, mux0, &IC_74xx157_t::ProcessUpdateB4);
    connect(instrBus, &HW_Bus_t::SignalBit3Updated, mux0, &IC_74xx157_t::ProcessUpdateA4);
    mux0->ProcessUpdateGb(LOW);


    connect(nand1, &IC_74xx00_t::SignalY1Updated, mux4, &IC_74xx157_t::ProcessUpdateAB);
    connect(instrBus, &HW_Bus_t::SignalBit0Updated, mux4, &IC_74xx157_t::ProcessUpdateA1);
    connect(addr4, &IC_74xx193_t::SignalQaUpdated, mux4, &IC_74xx157_t::ProcessUpdateB1);
    connect(mux4, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit4);
    connect(instrBus, &HW_Bus_t::SignalBit1Updated, mux4, &IC_74xx157_t::ProcessUpdateA2);
    connect(addr4, &IC_74xx193_t::SignalQbUpdated, mux4, &IC_74xx157_t::ProcessUpdateB2);
    connect(mux4, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit5);
    connect(mux4, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit6);
    connect(addr4, &IC_74xx193_t::SignalQcUpdated, mux4, &IC_74xx157_t::ProcessUpdateB3);
    connect(instrBus, &HW_Bus_t::SignalBit2Updated, mux4, &IC_74xx157_t::ProcessUpdateA3);
    connect(mux4, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit7);
    connect(addr4, &IC_74xx193_t::SignalQdUpdated, mux4, &IC_74xx157_t::ProcessUpdateB4);
    connect(instrBus, &HW_Bus_t::SignalBit3Updated, mux4, &IC_74xx157_t::ProcessUpdateA4);
    mux4->ProcessUpdateGb(LOW);


    connect(nand1, &IC_74xx00_t::SignalY1Updated, mux8, &IC_74xx157_t::ProcessUpdateAB);
    connect(instrBus, &HW_Bus_t::SignalBit0Updated, mux8, &IC_74xx157_t::ProcessUpdateA1);
    connect(addr8, &IC_74xx193_t::SignalQaUpdated, mux8, &IC_74xx157_t::ProcessUpdateB1);
    connect(mux8, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit8);
    connect(instrBus, &HW_Bus_t::SignalBit1Updated, mux8, &IC_74xx157_t::ProcessUpdateA2);
    connect(addr8, &IC_74xx193_t::SignalQbUpdated, mux8, &IC_74xx157_t::ProcessUpdateB2);
    connect(mux8, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBit9);
    connect(mux8, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitA);
    connect(addr8, &IC_74xx193_t::SignalQcUpdated, mux8, &IC_74xx157_t::ProcessUpdateB3);
    connect(instrBus, &HW_Bus_t::SignalBit2Updated, mux8, &IC_74xx157_t::ProcessUpdateA3);
    connect(mux8, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitB);
    connect(addr8, &IC_74xx193_t::SignalQdUpdated, mux8, &IC_74xx157_t::ProcessUpdateB4);
    connect(instrBus, &HW_Bus_t::SignalBit3Updated, mux8, &IC_74xx157_t::ProcessUpdateA4);
    mux8->ProcessUpdateGb(LOW);


    connect(nand1, &IC_74xx00_t::SignalY1Updated, muxC, &IC_74xx157_t::ProcessUpdateAB);
    connect(instrBus, &HW_Bus_t::SignalBit0Updated, muxC, &IC_74xx157_t::ProcessUpdateA1);
    connect(addrC, &IC_74xx193_t::SignalQaUpdated, muxC, &IC_74xx157_t::ProcessUpdateB1);
    connect(muxC, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitC);
    connect(instrBus, &HW_Bus_t::SignalBit1Updated, muxC, &IC_74xx157_t::ProcessUpdateA2);
    connect(addrC, &IC_74xx193_t::SignalQbUpdated, muxC, &IC_74xx157_t::ProcessUpdateB2);
    connect(muxC, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitD);
    connect(muxC, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitE);
    connect(addrC, &IC_74xx193_t::SignalQcUpdated, muxC, &IC_74xx157_t::ProcessUpdateB3);
    connect(instrBus, &HW_Bus_t::SignalBit2Updated, muxC, &IC_74xx157_t::ProcessUpdateA3);
    connect(muxC, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_t::ProcessUpdateBitF);
    connect(addrC, &IC_74xx193_t::SignalQdUpdated, muxC, &IC_74xx157_t::ProcessUpdateB4);
    connect(instrBus, &HW_Bus_t::SignalBit3Updated, muxC, &IC_74xx157_t::ProcessUpdateA4);
    muxC->ProcessUpdateGb(LOW);


    //
    // -- Counter for the bits
    //    --------------------
    bits->ProcessUpdateB(LOW);
    // bit 2 unused
    // bit 3 unused
    connect(and1, &IC_74xx08_t::SignalY1Updated, bits, &IC_74xx193_t::ProcessUpdateDown);
    bits->ProcessUpdateUp(HIGH);
    // bit 6 unused
    // bit 7 unused
    bits->ProcessUpdateD(HIGH);
    bits->ProcessUpdateC(LOW);
    connect(and1, &IC_74xx08_t::SignalY2Updated, bits, &IC_74xx193_t::ProcessUpdateLoad);
    // bit 12 unused
    // bit 13 handled in `and1` above
    bits->ProcessUpdateClr(LOW);
    bits->ProcessUpdateA(LOW);



    //
    // -- Counters for the address
    //    ------------------------
    addr0->ProcessUpdateB(LOW);
    // pin 2 handled in the muxes above
    // pin 3 handled in the muxes above
    addr0->ProcessUpdateDown(HIGH);
    connect(bits, &IC_74xx193_t::SignalBoUpdated, addr0, &IC_74xx193_t::ProcessUpdateUp);
    // pin 6 is handled in the muxes above
    // pin 7 is handled in the muxes above
    addr0->ProcessUpdateD(HIGH);
    addr0->ProcessUpdateC(HIGH);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr0, &IC_74xx193_t::ProcessUpdateLoad);
    // pin 12 is connected to the next IC
    // pin 13 is connected to the next IC, though it is not used
    addr0->ProcessUpdateClr(LOW);
    addr0->ProcessUpdateA(HIGH);


    addr4->ProcessUpdateB(HIGH);
    // pin 2 handled in the muxes above
    // pin 3 handled in the muxes above
    connect(addr0, &IC_74xx193_t::SignalBoUpdated, addr4, &IC_74xx193_t::ProcessUpdateDown);
    connect(addr0, &IC_74xx193_t::SignalCoUpdated, addr4, &IC_74xx193_t::ProcessUpdateUp);
    // pin 6 is handled in the muxes above
    // pin 7 is handled in the muxes above
    addr4->ProcessUpdateD(HIGH);
    addr4->ProcessUpdateC(HIGH);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr4, &IC_74xx193_t::ProcessUpdateLoad);
    // pin 12 is connected to the next IC
    // pin 13 is connected to the next IC, though it is not used
    addr4->ProcessUpdateClr(LOW);
    addr4->ProcessUpdateA(HIGH);


    addr8->ProcessUpdateB(HIGH);
    // pin 2 handled in the muxes above
    // pin 3 handled in the muxes above
    connect(addr4, &IC_74xx193_t::SignalBoUpdated, addr8, &IC_74xx193_t::ProcessUpdateDown);
    connect(addr4, &IC_74xx193_t::SignalCoUpdated, addr8, &IC_74xx193_t::ProcessUpdateUp);
    // pin 6 is handled in the muxes above
    // pin 7 is handled in the muxes above
    addr8->ProcessUpdateD(HIGH);
    addr8->ProcessUpdateC(HIGH);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr8, &IC_74xx193_t::ProcessUpdateLoad);
    // pin 12 is connected to the next IC
    // pin 13 is connected to the next IC, though it is not used
    addr8->ProcessUpdateClr(LOW);
    addr8->ProcessUpdateA(HIGH);


    addrC->ProcessUpdateB(HIGH);
    // pin 2 handled in the muxes above
    // pin 3 handled in the muxes above
    connect(addr8, &IC_74xx193_t::SignalBoUpdated, addrC, &IC_74xx193_t::ProcessUpdateDown);
    connect(addr8, &IC_74xx193_t::SignalCoUpdated, addrC, &IC_74xx193_t::ProcessUpdateUp);
    // pin 6 is handled in the muxes above
    // pin 7 is handled in the muxes above
    addrC->ProcessUpdateD(LOW);
    addrC->ProcessUpdateC(HIGH);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addrC, &IC_74xx193_t::ProcessUpdateLoad);
    // pin 12 is unused
    // pin 13 is unused
    addrC->ProcessUpdateClr(LOW);
    addrC->ProcessUpdateA(HIGH);


    //
    // -- Wire up the command/address shifter
    //    -----------------------------------
    connect(resetting, &IC_74xx74_t::SignalQ1Updated, shift, &IC_74xx165_t::ProcessUpdateShLd);
    connect(and1, &IC_74xx08_t::SignalY1Updated, shift, &IC_74xx165_t::ProcessUpdateClk);
    shift->ProcessUpdateE(LOW);
    shift->ProcessUpdateF(LOW);
    shift->ProcessUpdateG(LOW);
    shift->ProcessUpdateH(LOW);
    // pin 7 unused
    connect(shift, &IC_74xx165_t::SignalQHUpdated, this, &CtrlRomCtrlModule_t::SignalCommandAddresssLine);
    shift->ProcessUpdateSer(LOW);
    shift->ProcessUpdateA(HIGH);
    shift->ProcessUpdateB(HIGH);
    shift->ProcessUpdateC(LOW);
    shift->ProcessUpdateD(LOW);
    shift->ProcessUpdateClkInh(LOW);


    //
    // -- Wire up the LEDs
    //    ----------------
    connect(clock, &HW_Oscillator_t::SignalStateChanged, clk, &GUI_Led_t::ProcessStateChange);
    connect(resetting, &IC_74xx74_t::SignalQ1Updated, qr, &GUI_Led_t::ProcessStateChange);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, qc, &GUI_Led_t::ProcessStateChange);

    connect(mux0, &IC_74xx157_t::SignalY1Updated, led0, &GUI_Led_t::ProcessStateChange);
    connect(mux0, &IC_74xx157_t::SignalY2Updated, led1, &GUI_Led_t::ProcessStateChange);
    connect(mux0, &IC_74xx157_t::SignalY3Updated, led2, &GUI_Led_t::ProcessStateChange);
    connect(mux0, &IC_74xx157_t::SignalY4Updated, led3, &GUI_Led_t::ProcessStateChange);
    connect(mux4, &IC_74xx157_t::SignalY1Updated, led4, &GUI_Led_t::ProcessStateChange);
    connect(mux4, &IC_74xx157_t::SignalY2Updated, led5, &GUI_Led_t::ProcessStateChange);
    connect(mux4, &IC_74xx157_t::SignalY3Updated, led6, &GUI_Led_t::ProcessStateChange);
    connect(mux4, &IC_74xx157_t::SignalY4Updated, led7, &GUI_Led_t::ProcessStateChange);
    connect(mux8, &IC_74xx157_t::SignalY1Updated, led8, &GUI_Led_t::ProcessStateChange);
    connect(mux8, &IC_74xx157_t::SignalY2Updated, led9, &GUI_Led_t::ProcessStateChange);
    connect(mux8, &IC_74xx157_t::SignalY3Updated, ledA, &GUI_Led_t::ProcessStateChange);
    connect(mux8, &IC_74xx157_t::SignalY4Updated, ledB, &GUI_Led_t::ProcessStateChange);
    connect(muxC, &IC_74xx157_t::SignalY1Updated, ledC, &GUI_Led_t::ProcessStateChange);
    connect(muxC, &IC_74xx157_t::SignalY2Updated, ledD, &GUI_Led_t::ProcessStateChange);
    connect(muxC, &IC_74xx157_t::SignalY3Updated, ledE, &GUI_Led_t::ProcessStateChange);


    connect(bits, &IC_74xx193_t::SignalQaUpdated, bit0, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQbUpdated, bit1, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQcUpdated, bit2, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQdUpdated, bit3, &GUI_Led_t::ProcessStateChange);
}



//
// -- Start the clock when reset hits
//    -------------------------------
inline void CtrlRomCtrlModule_t::ProcessReset(TriState_t state)
{
    nand1->ProcessUpdateA1(state);
    nand1->ProcessUpdateB3(state);
    nand1->ProcessUpdateA3(state);
    resetting->ProcessUpdatePre1(state);

    if (state == HIGH) {
        clock->StartTimer();
    }
}






