//===================================================================================================================
//  mod-ctrl-rom.cc -- This class the control ROM implementation, with EEPROM and SRAM
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Feb-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-ctrl-rom.moc.cc"



//
// -- construct a new Control Rom Module
//    ----------------------------------
CtrlRomModule_t::CtrlRomModule_t(const QString &name, const QString &file) : QGroupBox(name), filename(file)
{
    setFixedWidth(95);
    setFixedHeight(50);
    setObjectName(name);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- allocate components for this module
//    -----------------------------------
void CtrlRomModule_t::AllocateComponents(void)
{
    ctrl = new HW_Bus_8_t(filename, HW_Computer_t::GetClock());
    eeprom = new IC_25lc256_t(filename);
    sram = new IC_AS6C62256_t(eeprom);
    shift = new IC_74xx164_t;
    latch = new IC_74xx574_t;
    driver = new IC_74xx541_t;
    led0 = new GUI_Led_t;
    led1 = new GUI_Led_t;
    led2 = new GUI_Led_t;
    led3 = new GUI_Led_t;
    led4 = new GUI_Led_t;
    led5 = new GUI_Led_t;
    led6 = new GUI_Led_t;
    led7 = new GUI_Led_t;
}



//
// -- Build the GUI for the Control Rom Module
//    ----------------------------------------
void CtrlRomModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid of LEDs and labels
    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter);
    layout->setSpacing(1);
    setLayout(layout);


    layout->addWidget(led7, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel("7"), 1, 0, Qt::AlignHCenter);
    layout->addWidget(led6, 0, 1, Qt::AlignHCenter);
    layout->addWidget(new QLabel("6"), 1, 1, Qt::AlignHCenter);
    layout->addWidget(led5, 0, 2, Qt::AlignHCenter);
    layout->addWidget(new QLabel("5"), 1, 2, Qt::AlignHCenter);
    layout->addWidget(led4, 0, 5, Qt::AlignHCenter);
    layout->addWidget(new QLabel("4"), 1, 5, Qt::AlignHCenter);
    layout->addWidget(led3, 0, 6, Qt::AlignHCenter);
    layout->addWidget(new QLabel("3"), 1, 6, Qt::AlignHCenter);
    layout->addWidget(led2, 0, 7, Qt::AlignHCenter);
    layout->addWidget(new QLabel("2"), 1, 7, Qt::AlignHCenter);
    layout->addWidget(led1, 0, 8, Qt::AlignHCenter);
    layout->addWidget(new QLabel("1"), 1, 8, Qt::AlignHCenter);
    layout->addWidget(led0, 0, 9, Qt::AlignHCenter);
    layout->addWidget(new QLabel("0"), 1, 9, Qt::AlignHCenter);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void CtrlRomModule_t::TriggerFirstUpdate(void)
{
    eeprom->TriggerFirstUpdate();
    sram->TriggerFirstUpdates();
    shift->TriggerFirstUpdate();
    latch->TriggerFirstUpdate();
    driver->TriggerFirstUpdate();
}



//
// -- Complete the wire-up for the Control ROM Control Module
//    -------------------------------------------------------
void CtrlRomModule_t::WireUp(void)
{
    //
    // -- handle the inputs for the eeprom
    //    --------------------------------
    // pin 1 (#CS) is handled in the ProcessUpdateChipSelect() slot
    // pin 2 is Serial Out
    eeprom->ProcessUpdateWp(LOW);
    // pin5 is handled in the cmd/addr slot
    // pin6 is handled in the sclk slot
    eeprom->ProcessUpdateHold(HIGH);



    //
    // -- handle the sram inputs
    //    ----------------------
    HW_Bus_16_t *ctrlBus = HW_Computer_t::GetCtrlMidPlane()->GetCtrlBus();
    connect(ctrlBus, &HW_Bus_16_t::SignalBitEUpdated, sram, &IC_AS6C62256_t::ProcessUpdateA14);
    connect(ctrlBus, &HW_Bus_16_t::SignalBitCUpdated, sram, &IC_AS6C62256_t::ProcessUpdateA12);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit7Updated, sram, &IC_AS6C62256_t::ProcessUpdateA7);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit6Updated, sram, &IC_AS6C62256_t::ProcessUpdateA6);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit5Updated, sram, &IC_AS6C62256_t::ProcessUpdateA5);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit4Updated, sram, &IC_AS6C62256_t::ProcessUpdateA4);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit3Updated, sram, &IC_AS6C62256_t::ProcessUpdateA3);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit2Updated, sram, &IC_AS6C62256_t::ProcessUpdateA2);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit1Updated, sram, &IC_AS6C62256_t::ProcessUpdateA1);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit0Updated, sram, &IC_AS6C62256_t::ProcessUpdateA0);
    connect(ctrl, &HW_Bus_8_t::SignalBit0Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq0);
    connect(ctrl, &HW_Bus_8_t::SignalBit1Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq1);
    connect(ctrl, &HW_Bus_8_t::SignalBit2Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq2);
    connect(ctrl, &HW_Bus_8_t::SignalBit3Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq3);
    connect(ctrl, &HW_Bus_8_t::SignalBit4Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq4);
    connect(ctrl, &HW_Bus_8_t::SignalBit5Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq5);
    connect(ctrl, &HW_Bus_8_t::SignalBit6Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq6);
    connect(ctrl, &HW_Bus_8_t::SignalBit7Updated, sram, &IC_AS6C62256_t::ProcessUpdateDq7);
    // pin 20 handled below (#CE)
    connect(ctrlBus, &HW_Bus_16_t::SignalBitAUpdated, sram, &IC_AS6C62256_t::ProcessUpdateA10);
    // pin 22 handled in the header (#OE)
    connect(ctrlBus, &HW_Bus_16_t::SignalBitBUpdated, sram, &IC_AS6C62256_t::ProcessUpdateA11);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit9Updated, sram, &IC_AS6C62256_t::ProcessUpdateA9);
    connect(ctrlBus, &HW_Bus_16_t::SignalBit8Updated, sram, &IC_AS6C62256_t::ProcessUpdateA8);
    connect(ctrlBus, &HW_Bus_16_t::SignalBitDUpdated, sram, &IC_AS6C62256_t::ProcessUpdateA13);
    // pin 27 handled in the header (#E)



    //
    // -- connect up the shift register inputs
    //    ------------------------------------
    connect(eeprom, &IC_25lc256_t::SignalSoUpdated, shift, &IC_74xx164_t::ProcessUpdateA);
    shift->ProcessUpdateB(HIGH);
    // pin 3 is output Qa
    // pin 4 is output Qb
    // pin 5 is output Qc
    // pin 6 is output Qd
    // pin 8 is handled shift clock signal in header
    // pin 9 is handled with #Qr in the header
    // pin 10 is output Qe
    // pin 11 is output Qf
    // pin 12 is output Qg
    // pin 13 is output Qh



    //
    // -- hook up the line driver
    //    -----------------------
    driver->ProcessUpdateOE1(LOW);
    connect(shift, &IC_74xx164_t::SignalQAUpdated, driver, &IC_74xx541_t::ProcessUpdateD0);
    connect(shift, &IC_74xx164_t::SignalQBUpdated, driver, &IC_74xx541_t::ProcessUpdateD1);
    connect(shift, &IC_74xx164_t::SignalQCUpdated, driver, &IC_74xx541_t::ProcessUpdateD2);
    connect(shift, &IC_74xx164_t::SignalQDUpdated, driver, &IC_74xx541_t::ProcessUpdateD3);
    connect(shift, &IC_74xx164_t::SignalQEUpdated, driver, &IC_74xx541_t::ProcessUpdateD4);
    connect(shift, &IC_74xx164_t::SignalQFUpdated, driver, &IC_74xx541_t::ProcessUpdateD5);
    connect(shift, &IC_74xx164_t::SignalQGUpdated, driver, &IC_74xx541_t::ProcessUpdateD6);
    connect(shift, &IC_74xx164_t::SignalQHUpdated, driver, &IC_74xx541_t::ProcessUpdateD7);
    // pin 11 is output Q7
    // pin 12 is output Q6
    // pin 13 is output Q5
    // pin 14 is output Q4
    // pin 15 is output Q3
    // pin 16 is output Q2
    // pin 17 is output Q1
    // pin 18 is output Q0
    // pin 19 is handled with the Qcb signal from the header



    //
    // -- connect in the latch from the internal bus to the output bus
    //    ------------------------------------------------------------
    // pin 1 is handled with the Qc signal in the header
    connect(ctrl, &HW_Bus_8_t::SignalBit0Updated, latch, &IC_74xx574_t::ProcessUpdateD1);
    connect(ctrl, &HW_Bus_8_t::SignalBit1Updated, latch, &IC_74xx574_t::ProcessUpdateD2);
    connect(ctrl, &HW_Bus_8_t::SignalBit2Updated, latch, &IC_74xx574_t::ProcessUpdateD3);
    connect(ctrl, &HW_Bus_8_t::SignalBit3Updated, latch, &IC_74xx574_t::ProcessUpdateD4);
    connect(ctrl, &HW_Bus_8_t::SignalBit4Updated, latch, &IC_74xx574_t::ProcessUpdateD5);
    connect(ctrl, &HW_Bus_8_t::SignalBit5Updated, latch, &IC_74xx574_t::ProcessUpdateD6);
    connect(ctrl, &HW_Bus_8_t::SignalBit6Updated, latch, &IC_74xx574_t::ProcessUpdateD7);
    connect(ctrl, &HW_Bus_8_t::SignalBit7Updated, latch, &IC_74xx574_t::ProcessUpdateD8);
    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockState, latch, &IC_74xx574_t::ProcessUpdateClk);



    //
    // -- the internal bus has needs to have 2 inputs: driver and sram
    //    ------------------------------------------------------------
    connect(driver, &IC_74xx541_t::SignalY0Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit0);
    connect(driver, &IC_74xx541_t::SignalY1Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit1);
    connect(driver, &IC_74xx541_t::SignalY2Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit2);
    connect(driver, &IC_74xx541_t::SignalY3Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit3);
    connect(driver, &IC_74xx541_t::SignalY4Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit4);
    connect(driver, &IC_74xx541_t::SignalY5Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit5);
    connect(driver, &IC_74xx541_t::SignalY6Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit6);
    connect(driver, &IC_74xx541_t::SignalY7Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit7);



    // -- be careful of infinite recursion for these bidirectional pins!!!
    connect(sram, &IC_AS6C62256_t::SignalDq0Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit0);
    connect(sram, &IC_AS6C62256_t::SignalDq1Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit1);
    connect(sram, &IC_AS6C62256_t::SignalDq2Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit2);
    connect(sram, &IC_AS6C62256_t::SignalDq3Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit3);
    connect(sram, &IC_AS6C62256_t::SignalDq4Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit4);
    connect(sram, &IC_AS6C62256_t::SignalDq5Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit5);
    connect(sram, &IC_AS6C62256_t::SignalDq6Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit6);
    connect(sram, &IC_AS6C62256_t::SignalDq7Updated, ctrl, &HW_Bus_8_t::ProcessUpdateBit7);



    //
    // -- The latch needs to be passed off board
    //    --------------------------------------
    connect(latch, &IC_74xx574_t::SignalQ1Updated, this, &CtrlRomModule_t::SignalBit0Updated);
    connect(latch, &IC_74xx574_t::SignalQ2Updated, this, &CtrlRomModule_t::SignalBit1Updated);
    connect(latch, &IC_74xx574_t::SignalQ3Updated, this, &CtrlRomModule_t::SignalBit2Updated);
    connect(latch, &IC_74xx574_t::SignalQ4Updated, this, &CtrlRomModule_t::SignalBit3Updated);
    connect(latch, &IC_74xx574_t::SignalQ5Updated, this, &CtrlRomModule_t::SignalBit4Updated);
    connect(latch, &IC_74xx574_t::SignalQ6Updated, this, &CtrlRomModule_t::SignalBit5Updated);
    connect(latch, &IC_74xx574_t::SignalQ7Updated, this, &CtrlRomModule_t::SignalBit6Updated);
    connect(latch, &IC_74xx574_t::SignalQ8Updated, this, &CtrlRomModule_t::SignalBit7Updated);



    //
    // -- hook up the LEDs (to the output of the latch)
    //    ---------------------------------------------
    connect(ctrl, &HW_Bus_8_t::SignalBit0Updated, led0, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit1Updated, led1, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit2Updated, led2, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit3Updated, led3, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit4Updated, led4, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit5Updated, led5, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit6Updated, led6, &GUI_Led_t::ProcessStateChange);
    connect(ctrl, &HW_Bus_8_t::SignalBit7Updated, led7, &GUI_Led_t::ProcessStateChange);
}

