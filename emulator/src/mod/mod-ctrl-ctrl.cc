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
//  OK, so this get real confusing real quick.  I will document how the gates are used here in order to maintain
//  some order.
//
//  `resetting` -- 74xx74
//  Latch 1: This is the resetting latch where Qr is set when the circuit is in reset; #Qr is available
//  Latch 2: Unused
//
//  `nand1` -- 74xx00
//  Gates 1 & 2: Copying SR latch where Qc is on Y1 and #Qc is on Y2
//  Gates 3 & 4: Shifting SR latch where Qs is on Y4 and #Qs is on Y3
//
//  `nand2` -- 74xx00
//  Gates 1 & 2: Copying SR latch where Ql is on Y1 and #Ql is on Y2
//  Gate 3: unused
//  Gate 4: #RESET NAND #RESET
//
//  `and1` -- 74xx08
//  Gate 1: (Qc * CLK)
//  Gate 2: ((Qc * CLK) * Qs)
//  Gate 3: ((#Bc * #Bb) * Ba)
//  Gate 4: (#Bc * #Bb)
//
//  `and2` -- 74xx08
//  Gate 1: (#CLK * #Bd)
//  Gate 2: ((#CLK * #Bd) * ((#Bc * #Bb) * Ba))
//  Gate 3: (Bd * ((#Bc * #Bb) * Ba))
//  Gate 4: (#Qr * /BO)
//
//  `and3` -- 74xx08
//  Gate 1: (((Qc * CLK) * Qs) * #Qr)
//  Gate 2: Qc * Qs
//  Gate 3: unused
//  Gate 4: unused
//
//  `or1` -- 74xx32
//  Gate 1: (Ba + Bb)
//  Gate 2: (Bc + Bd)
//  Gate 3: ((Ba + Bb) + (Bc + Bd))
//  Gate 4: (Qr + #Qc)
//
//  `inv1` -- 74xx04
//  Gate 1: #CLK
//  Gate 2: #Bb
//  Gate 3: #Bc
//  Gate 4: #Bd
//  Gate 5: #(Bd * #Bc * #Bb * Ba)
//  Gate 6: #(#CLK * #Bd * #Bc * #Bb * Ba)
//
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
    SetDebug();
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
    nand2 = new IC_74xx00_t;
    and1 = new IC_74xx08_t;
    and2 = new IC_74xx08_t;
    and3 = new IC_74xx08_t;
    or1 = new IC_74xx32_t;
    oNand1 = new IC_74xx03_t;
    inv1 = new IC_74xx04_t;
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

    cpy = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::yellow);
    oe = new GUI_Led_t(GUI_Led_t::OnWhenLow, Qt::yellow);
    we = new GUI_Led_t(GUI_Led_t::OnWhenLow, Qt::yellow);
    ce = new GUI_Led_t(GUI_Led_t::OnWhenLow, Qt::yellow);

    bit0 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
    bit3 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::magenta);
}



//
// -- Build the GUI for the Control Rom Control Module
//    ------------------------------------------------
void CtrlRomCtrlModule_t::BuildGui(void)
{
    QGridLayout *controlLayout = new QGridLayout;
    controlLayout->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    controlLayout->setSpacing(0);
    controlLayout->setColumnMinimumWidth(0, 12);
    controlLayout->setColumnMinimumWidth(1, 12);
    controlLayout->setColumnMinimumWidth(2, 12);
    controlLayout->setColumnMinimumWidth(3, 12);
    controlLayout->setColumnMinimumWidth(4, 12);
    controlLayout->setColumnMinimumWidth(5, 12);
    controlLayout->setColumnMinimumWidth(6, 12);
    controlLayout->setColumnMinimumWidth(7, 12);
    controlLayout->setColumnMinimumWidth(8, 12);
    controlLayout->setColumnMinimumWidth(9, 12);
    controlLayout->setColumnMinimumWidth(10, 12);
    controlLayout->setColumnMinimumWidth(11, 12);

    controlLayout->addWidget(clk, 0, 0, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("clk"), 1, 0, Qt::AlignHCenter);
    controlLayout->addWidget(cpy, 0, 2, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("cp"), 1, 2, Qt::AlignHCenter);
    controlLayout->addWidget(oe, 0, 4, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("oe"), 1, 4, Qt::AlignHCenter);
    controlLayout->addWidget(we, 0, 5, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("we"), 1, 5, Qt::AlignHCenter);
    controlLayout->addWidget(ce, 0, 6, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel("ce"), 1, 6, Qt::AlignHCenter);
    controlLayout->addWidget(bit3, 0, 8, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 8 "), 1, 8, Qt::AlignHCenter);
    controlLayout->addWidget(bit2, 0, 9, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 4 "), 1, 9, Qt::AlignHCenter);
    controlLayout->addWidget(bit1, 0, 10, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 2 "), 1, 10, Qt::AlignHCenter);
    controlLayout->addWidget(bit0, 0, 11, Qt::AlignHCenter);
    controlLayout->addWidget(new QLabel(" 1 "), 1, 11, Qt::AlignHCenter);

    setAlignment(Qt::AlignHCenter);
    controlLayout->setSpacing(1);
    setLayout(controlLayout);
    setFixedWidth(180);
    setFixedHeight(60);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void CtrlRomCtrlModule_t::TriggerFirstUpdate(void)
{
    resetting->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
    nand2->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    and2->TriggerFirstUpdate();
    and3->TriggerFirstUpdate();
    inv1->TriggerFirstUpdate();
    or1->TriggerFirstUpdate();
    oNand1->TriggerFirstUpdate();
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
    // -- First connect up the inputs on the 74xx74 IC
    //    --------------------------------------------

    //
    // -- Gate 1 is used for the In Reset condition
    //    -----------------------------------------
    resetting->ProcessUpdateClr1(HIGH);                                                             // pin 1: #CLR1
    connect(nand2, &IC_74xx00_t::SignalY4Updated, resetting, &IC_74xx74_t::ProcessUpdateD1);        // pin 2: D1
    connect(inv1, &IC_74xx04_t::SignalY1Updated, resetting, &IC_74xx74_t::ProcessUpdateClockLatch1);       // pin 3: CLK1 from local clock
    connect(inv1, &IC_74xx04_t::SignalY1Updated, resetting, &IC_74xx74_t::ProcessUpdateClockOutput1);       // pin 3: CLK1 from local clock
    // pin 4 is handled in ProcessResetUpdate() -- below
    // pin 5 is the Q output pin
    // pin 6 is the #Q output pin


    //
    // -- Gate 2 is #Done
    //    ---------------
    // pin 8 is the #Q pin (unused)
    // pin 9 is the Q pin (handled by its inputs)
    // pin 10 is handled in ProcessResetUpdate() -- below
    connect(inv1, &IC_74xx04_t::SignalY1Updated, resetting, &IC_74xx74_t::ProcessUpdateClockLatch2);       // pin 11: CLK2 from local clock
    connect(inv1, &IC_74xx04_t::SignalY1Updated, resetting, &IC_74xx74_t::ProcessUpdateClockOutput2);       // pin 11: CLK2 from local clock
    connect(nand1, &IC_74xx00_t::SignalY1Updated, resetting, &IC_74xx74_t::ProcessUpdateD2);        // pin 12: D2
    resetting->ProcessUpdateClr2(HIGH);                                                             // pin 13: #CLR2




    //
    // -- connect up nand1 for 2 SR latches.  These are handled in pairs for this section
    //    -------------------------------------------------------------------------------

    //
    // -- Gates 1&2 are used for the Copying (Qc) SR Latch
    //    ------------------------------------------------
    // pin 1 is the set pin, and handled in ProcessResetUpdate() -- below
    connect(nand1, &IC_74xx00_t::SignalY2Updated, nand1, &IC_74xx00_t::ProcessUpdateB1);            // pin 2: 1B Input
    // pin 3 is the output of the set-side gate -- Qc
    connect(nand1, &IC_74xx00_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateA2);            // pin 4: 2A input
    connect(addrC, &IC_74xx193_t::SignalCoUpdated, nand1, &IC_74xx00_t::ProcessUpdateB2);           // pin 5: 2B input
    // pin 6 is the output of the reset-side gate -- #Qc


    //
    // -- Gates 3&4 are for the Shifting (Qs) SR latch
    //    --------------------------------------------
    // pin 8 is the output of the reset-side gate -- #Qs
    connect(inv1, &IC_74xx04_t::SignalY6Updated, nand1, &IC_74xx00_t::ProcessUpdateA3);             // pin 9: 3A Input
    connect(nand1, &IC_74xx00_t::SignalY4Updated, nand1, &IC_74xx00_t::ProcessUpdateB3);            // pin 10: 3B Input
    // pin 11 is the output of the set-side gate -- Qs
    connect(nand1, &IC_74xx00_t::SignalY3Updated, nand1, &IC_74xx00_t::ProcessUpdateA4);            // pin 12: 4A Input
    connect(inv1, &IC_74xx04_t::SignalY5Updated, nand1, &IC_74xx00_t::ProcessUpdateB4);             // pin 13: 4B Input



    //
    // -- connect up nand2 for 1 additional SR latche.  These are handled in pairs for this section
    //    -----------------------------------------------------------------------------------------

    //
    // -- Gates 1&2 are used for the Copying (Ql) SR Latch
    //    ------------------------------------------------
    connect(or1, &IC_74xx32_t::SignalY3Updated, nand2, &IC_74xx00_t::ProcessUpdateA1);              // pin 1: 2B input (Set)
    connect(nand2, &IC_74xx00_t::SignalY2Updated, nand2, &IC_74xx00_t::ProcessUpdateB1);            // pin 2: 1B Input
    // pin 3 is the output of the set-side gate -- Ql
    connect(nand2, &IC_74xx00_t::SignalY1Updated, nand2, &IC_74xx00_t::ProcessUpdateA2);            // pin 4: 2A input
    connect(inv1, &IC_74xx04_t::SignalY6Updated, nand2, &IC_74xx00_t::ProcessUpdateB2);             // pin 5: 1A Input (Reset)
    // pin 6 is the output of the reset-side gate -- #Ql


    //
    // -- Gates 3 is unused
    //    -----------------
    // pin 8 is unused
    nand2->ProcessA3Low();                                                                          // pin 9: Qs
    nand2->ProcessB3Low();                                                                          // pin 10: Qc



    //
    // -- Gates 4 is used as an inverter
    //    ------------------------------
    // pin 11 is unused
    // pin 12 is #RESET, handled in ProcessReset below
    // pin 13 is #RESET, handled in ProcessReset below



    //
    // -- connect up the first AND gate
    //    -----------------------------

    //
    // -- Gate 1 is used for (Qc * CLK)
    //    -----------------------------
    connect(nand1, &IC_74xx00_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateA1);             // pin 1: Qc input
    connect(clock, &HW_Oscillator_t::SignalStateChanged, and1, &IC_74xx08_t::ProcessUpdateB1);      // pin 2: CLK input
    // pin 3 is the output (Qc * CLK)


    //
    // -- Gate 2 is ued for ((Qc * CLK) * Qs)
    //    -----------------------------------
    connect(and1, &IC_74xx08_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateA2);              // pin 4: (Qc * CLK) input
    connect(nand1, &IC_74xx00_t::SignalY4Updated, and1, &IC_74xx08_t::ProcessUpdateB2);             // pin 5: Qs
    // pin 6 is the output ((Qc * CLK) * Qs)


    //
    // -- Gate 3 is used for ((#Bc * #Bb) * Ba)
    //    -------------------------------------
    // pin 8 is the output ((#Bc * #Bb) * Ba)
    connect(bits, &IC_74xx193_t::SignalQaUpdated, and1, &IC_74xx08_t::ProcessUpdateA3);             // pin 9: Ba input
    connect(and1, &IC_74xx08_t::SignalY4Updated, and1, &IC_74xx08_t::ProcessUpdateB3);              // pin 10: (#Bc * #Bb) input


    //
    // -- Gate 4 is used for (#Bc * #Bb)
    //    ------------------------------
    // pin 11 is the output (#Bc * #Bb)
    connect(inv1, &IC_74xx04_t::SignalY3Updated, and1, &IC_74xx08_t::ProcessUpdateA4);              // pin 12: #Bc Input
    connect(inv1, &IC_74xx04_t::SignalY2Updated, and1, &IC_74xx08_t::ProcessUpdateB4);              // pin 13: #Bb Input



    //
    // -- connect up the second AND gate
    //    ------------------------------

    //
    // -- Gate 1 is used for (#CLK * #Bd)
    //    -------------------------------
    connect(inv1, &IC_74xx04_t::SignalY1Updated, and2, &IC_74xx08_t::ProcessUpdateA1);              // pin 1: #CLK Input
    connect(inv1, &IC_74xx04_t::SignalY4Updated, and2, &IC_74xx08_t::ProcessUpdateB1);              // pin 2: #Bd Input
    // pin 3 is the output (#CLK * #Bd)


    //
    // -- Gate 2 is used for ((#CLK * #Bd) * ((#Bc * #Bb) * Ba))
    //    ------------------------------------------------------
    connect(and2, &IC_74xx08_t::SignalY1Updated, and2, &IC_74xx08_t::ProcessUpdateA2);              // pin 4: (#CLK * #Bd) Input
    connect(and1, &IC_74xx08_t::SignalY3Updated, and2, &IC_74xx08_t::ProcessUpdateB2);              // pin 5: ((#Bc * #Bb) * Ba) Input
    // pin 6 is the complete ((#CLK * #Bd) * ((#Bc * #Bb) * Ba)) Output


    //
    // -- Gate 3 is used for (Bd * ((#Bc * #Bb) * Ba))
    //    --------------------------------------------
    // pin 8 is the output (Bd * ((#Bc * #Bb) * Ba))
    connect(bits, &IC_74xx193_t::SignalQdUpdated, and2, &IC_74xx08_t::ProcessUpdateA3);             // pin 9: Bd Input
    connect(and1, &IC_74xx08_t::SignalY3Updated, and2, &IC_74xx08_t::ProcessUpdateB3);              // pin 10: ((#Bc * #Bb) * Ba) Input


    //
    // -- Gate 4 is used for (#Qr * /BO)
    //    ------------------------------
    // pin 11 is the output (#Qr * /BO)
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, and2, &IC_74xx08_t::ProcessUpdateA4);        // pin 12: #Qr Input
    connect(bits, &IC_74xx193_t::SignalBoUpdated, and2, &IC_74xx08_t::ProcessUpdateB4);             // pin 13: /BO Input



    //
    // -- connect up the third AND gate
    //    -----------------------------

    //
    // -- Gate 1 is used for (((Qc * CLK) * Qs) * #Qr)
    //    --------------------------------------------
    connect(and1, &IC_74xx08_t::SignalY2Updated, and3, &IC_74xx08_t::ProcessUpdateA1);              // pin 1:
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, and3, &IC_74xx08_t::ProcessUpdateB1);        // pin 2:
    // pin 3 is the output (((Qc * CLK) * Qs) * #Qr)


    //
    // -- Gate 2 is (Qs * Qc)
    //    -------------------
    connect(nand1, &IC_74xx00_t::SignalY4Updated, and3, &IC_74xx08_t::ProcessUpdateA2);             // pin 4
    connect(nand1, &IC_74xx00_t::SignalY1Updated, and3, &IC_74xx08_t::ProcessUpdateB2);             // pin 5
    // pin 6 is (Qs * Qc)


    //
    // -- Gate 3 is unused
    //    ----------------
    // pin 8 is unused
    and3->ProcessA3Low();                                                                           // pin 9
    and3->ProcessB3Low();                                                                           // pin 10


    //
    // -- Gate 4 is unused
    //    ----------------
    // pin 11 is unused
    and3->ProcessA4Low();                                                                           // pin 12
    and3->ProcessB4Low();                                                                           // pin 13



    //
    // -- connect up the OR gate IC
    //    -------------------------

    //
    // -- Gate 1 is (Ba + Bb)
    //    -------------------
    connect(bits, &IC_74xx193_t::SignalQaUpdated, or1, &IC_74xx32_t::ProcessUpdateA1);              // pin 1: Ba
    connect(bits, &IC_74xx193_t::SignalQbUpdated, or1, &IC_74xx32_t::ProcessUpdateB1);              // pin 2: Bb
    // pin 3 is the (Ba + Bb) output


    //
    // -- Gate 2 is (Bc + Bd)
    //    -------------------
    connect(bits, &IC_74xx193_t::SignalQcUpdated, or1, &IC_74xx32_t::ProcessUpdateA2);              // pin 4: Bc
    connect(bits, &IC_74xx193_t::SignalQdUpdated, or1, &IC_74xx32_t::ProcessUpdateB2);              // pin 5: Bd
    // pin 6 is the (Bc + Bd) output


    //
    // -- Gate 3 is ((Ba + Bb) + (Bc + Bd))
    //    ---------------------------------
    // pin 8 is the ((Ba + Bb) + (Bc + Bd)) output
    connect(or1, &IC_74xx32_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateA3);                // pin 9: (Ba + Bb)
    connect(or1, &IC_74xx32_t::SignalY2Updated, or1, &IC_74xx32_t::ProcessUpdateB3);                // pin 10: (Bc + Bd)


    //
    // -- Gate 4 is (Qr + #Qc)
    //    --------------------
    // pin 11 is (Qr + #Qc)
    connect(nand1, &IC_74xx00_t::SignalY2Updated, or1, &IC_74xx32_t::ProcessUpdateA4);              // pin 12: #Qc Input
    connect(resetting, &IC_74xx74_t::SignalQ1Updated, or1, &IC_74xx32_t::ProcessUpdateB4);          // pin 13: Qr Input



    //
    // -- connect up the Open Drain/Open Collector AND gate
    //    -------------------------------------------------

    //
    // -- Gate 1 is (#Qc * #Qc)
    //    ---------------------
    connect(resetting, &IC_74xx74_t::SignalQ2Updated, oNand1, &IC_74xx03_t::ProcessUpdateA1);          // pin 1: #Done Input
    connect(resetting, &IC_74xx74_t::SignalQ2Updated, oNand1, &IC_74xx03_t::ProcessUpdateB1);          // pin 2: #Done Input
    // pin 3 is the output (#Qc + #Qc)


    //
    // -- Gate 2 is Unused
    //    --------------------
    oNand1->ProcessA2Low();                                                                          // pin 9 is unused
    oNand1->ProcessB2Low();                                                                          // pin 10 is unused
    // pin 6 is unused


    //
    // -- Gate 3 is unused
    //    ----------------
    // pin 8 is unused
    oNand1->ProcessA3Low();                                                                          // pin 9 is unused
    oNand1->ProcessB3Low();                                                                          // pin 10 is unused


    //
    // -- Gate 4 is unused
    //    ----------------
    // pin 11 is unused
    oNand1->ProcessA4Low();                                                                          // pin 12 is unused
    oNand1->ProcessB4Low();                                                                          // pin 13 is unused



    //
    // -- connect up inverter 1
    //    ---------------------

    //
    // -- Gate 1 is #CLK
    //    --------------
    connect(clock, &HW_Oscillator_t::SignalStateChanged, inv1, &IC_74xx04_t::ProcessUpdateA1);      // pin 1: CLK
    // pin 2 is #CLK


    //
    // -- Gate 2 is #Bb
    //    -------------
    connect(bits, &IC_74xx193_t::SignalQbUpdated, inv1, &IC_74xx04_t::ProcessUpdateA2);             // pin 3: Bb
    // pin 4 is #Bb output


    //
    // -- Gate 3 is #Bc
    //    -------------
    connect(bits, &IC_74xx193_t::SignalQcUpdated, inv1, &IC_74xx04_t::ProcessUpdateA3);             // pin 5: Bb
    // pin 6 is #Bb output


    //
    // -- Gate 4 is #Bd
    //    -------------
    // pin 8 is #Bd
    connect(bits, &IC_74xx193_t::SignalQdUpdated, inv1, &IC_74xx04_t::ProcessUpdateA4);             // pin 9: Bb


    //
    // -- Gate 5 is #(Bd * #Bc * #Bb * Ba)
    //    --------------------------------
    // pin 10 is #(Bd * #Bc * #Bb * Ba)
    connect(and2, &IC_74xx08_t::SignalY3Updated, inv1, &IC_74xx04_t::ProcessUpdateA5);              // pin 11: (Bd * #Bc * #Bb * Ba)


    //
    // -- Gate 6 is #(#CLK * #Bd * #Bc * #Bb * Ba)
    //    ----------------------------------------
    // pin 12 is #(#CLK * #Bd * #Bc * #Bb * Ba)
    connect(and2, &IC_74xx08_t::SignalY2Updated, inv1, &IC_74xx04_t::ProcessUpdateA6);              // pin 13: (#CLK * #Bd * #Bc * #Bb * Ba)



    //
    // -- Connect up MUX0 -- the least significant nibble
    //    -----------------------------------------------
    HW_Bus_16_t *instrBus = HW_Computer_t::Get()->GetInstrBus();

    connect(resetting, &IC_74xx74_t::SignalQ2Updated, mux0, &IC_74xx157_t::ProcessUpdateAB);        // pin 1: #Done
    connect(instrBus, &HW_Bus_16_t::SignalBit0Updated, mux0, &IC_74xx157_t::ProcessUpdateA1);       // pin 2: Input A1: bit 0
    connect(addr0, &IC_74xx193_t::SignalQaUpdated, mux0, &IC_74xx157_t::ProcessUpdateB1);           // pin 3: Input B1: bit 0
    // pin 4: Output Y1
    connect(instrBus, &HW_Bus_16_t::SignalBit1Updated, mux0, &IC_74xx157_t::ProcessUpdateA2);       // pin 5: Input A2: bit 1
    connect(addr0, &IC_74xx193_t::SignalQbUpdated, mux0, &IC_74xx157_t::ProcessUpdateB2);           // pin 6: Input B2: bit 1
    // pin 6: Output Y2
    // pin 9: Output Y3
    connect(addr0, &IC_74xx193_t::SignalQcUpdated, mux0, &IC_74xx157_t::ProcessUpdateB3);           // pin 10: Input B3: bit 2
    connect(instrBus, &HW_Bus_16_t::SignalBit2Updated, mux0, &IC_74xx157_t::ProcessUpdateA3);       // pin 11: Input A3: bit 2
    // pin 12: Output Y4
    connect(addr0, &IC_74xx193_t::SignalQdUpdated, mux0, &IC_74xx157_t::ProcessUpdateB4);           // pin 13: Input B4: bit 3
    connect(instrBus, &HW_Bus_16_t::SignalBit3Updated, mux0, &IC_74xx157_t::ProcessUpdateA4);       // pin 14: Input A4: bit 3
    mux0->ProcessUpdateGb(LOW);                                                                     // pin 15: Input /G


    //
    // -- Connect up MUX4 -- the next least significant nibble
    //    ----------------------------------------------------
    connect(resetting, &IC_74xx74_t::SignalQ2Updated, mux4, &IC_74xx157_t::ProcessUpdateAB);        // pin 1: #Done
    connect(instrBus, &HW_Bus_16_t::SignalBit4Updated, mux4, &IC_74xx157_t::ProcessUpdateA1);       // pin 2: Input A1: bit 4
    connect(addr4, &IC_74xx193_t::SignalQaUpdated, mux4, &IC_74xx157_t::ProcessUpdateB1);           // pin 3: Input B1: bit 4
    // pin 4: Output Y1
    connect(instrBus, &HW_Bus_16_t::SignalBit5Updated, mux4, &IC_74xx157_t::ProcessUpdateA2);       // pin 5: Input A2: bit 5
    connect(addr4, &IC_74xx193_t::SignalQbUpdated, mux4, &IC_74xx157_t::ProcessUpdateB2);           // pin 6: Input B2: bit 5
    // pin 6: Output Y2
    // pin 9: Output Y3
    connect(addr4, &IC_74xx193_t::SignalQcUpdated, mux4, &IC_74xx157_t::ProcessUpdateB3);           // pin 10: Input B3: bit 6
    connect(instrBus, &HW_Bus_16_t::SignalBit6Updated, mux4, &IC_74xx157_t::ProcessUpdateA3);       // pin 11: Input A3: bit 6
    // pin 12: Output Y4
    connect(addr4, &IC_74xx193_t::SignalQdUpdated, mux4, &IC_74xx157_t::ProcessUpdateB4);           // pin 13: Input B4: bit 7
    connect(instrBus, &HW_Bus_16_t::SignalBit7Updated, mux4, &IC_74xx157_t::ProcessUpdateA4);       // pin 14: Input A4: bit 7
    mux4->ProcessUpdateGb(LOW);                                                                     // pin 15: Input /G



    //
    // -- Connect up MUX8 -- the next most significant nibble
    //    ---------------------------------------------------
    connect(resetting, &IC_74xx74_t::SignalQ2Updated, mux8, &IC_74xx157_t::ProcessUpdateAB);        // pin 1: #Done
    connect(instrBus, &HW_Bus_16_t::SignalBit8Updated, mux8, &IC_74xx157_t::ProcessUpdateA1);       // pin 2: Input A1: bit 8
    connect(addr8, &IC_74xx193_t::SignalQaUpdated, mux8, &IC_74xx157_t::ProcessUpdateB1);           // pin 3: Input B1: bit 8
    // pin 4: Output Y1
    connect(instrBus, &HW_Bus_16_t::SignalBit9Updated, mux8, &IC_74xx157_t::ProcessUpdateA2);       // pin 5: Input A2: bit 9
    connect(addr8, &IC_74xx193_t::SignalQbUpdated, mux8, &IC_74xx157_t::ProcessUpdateB2);           // pin 6: Input B2: bit 9
    // pin 6: Output Y2
    // pin 9: Output Y3
    connect(addr8, &IC_74xx193_t::SignalQcUpdated, mux8, &IC_74xx157_t::ProcessUpdateB3);           // pin 10: Input B3: bit 10
    connect(instrBus, &HW_Bus_16_t::SignalBitAUpdated, mux8, &IC_74xx157_t::ProcessUpdateA3);       // pin 11: Input A3: bit 10
    // pin 12: Output Y4
    connect(addr8, &IC_74xx193_t::SignalQdUpdated, mux8, &IC_74xx157_t::ProcessUpdateB4);           // pin 13: Input B4: bit 11
    connect(instrBus, &HW_Bus_16_t::SignalBitBUpdated, mux8, &IC_74xx157_t::ProcessUpdateA4);       // pin 14: Input A4: bit 11
    mux8->ProcessUpdateGb(LOW);                                                                     // pin 15: Input /G



    //
    // -- Connect up MUXc -- the most significant nibble
    //    ----------------------------------------------
    connect(resetting, &IC_74xx74_t::SignalQ2Updated, muxC, &IC_74xx157_t::ProcessUpdateAB);        // pin 1: #Done
    connect(instrBus, &HW_Bus_16_t::SignalBitCUpdated, muxC, &IC_74xx157_t::ProcessUpdateA1);       // pin 2: Input A1: bit 12
    connect(addrC, &IC_74xx193_t::SignalQaUpdated, muxC, &IC_74xx157_t::ProcessUpdateB1);           // pin 3: Input B1: bit 12
    // pin 4: Output Y1
    connect(instrBus, &HW_Bus_16_t::SignalBitDUpdated, muxC, &IC_74xx157_t::ProcessUpdateA2);       // pin 5: Input A2: bit 13
    connect(addrC, &IC_74xx193_t::SignalQbUpdated, muxC, &IC_74xx157_t::ProcessUpdateB2);           // pin 6: Input B2: bit 13
    // pin 6: Output Y2
    // pin 9: Output Y3
    connect(addrC, &IC_74xx193_t::SignalQcUpdated, muxC, &IC_74xx157_t::ProcessUpdateB3);           // pin 10: Input B3: bit 14
    connect(instrBus, &HW_Bus_16_t::SignalBitEUpdated, muxC, &IC_74xx157_t::ProcessUpdateA3);       // pin 11: Input A3: bit 14
    // pin 12: Output Y4
    connect(addrC, &IC_74xx193_t::SignalQdUpdated, muxC, &IC_74xx157_t::ProcessUpdateB4);           // pin 13: Input B4: bit 15
    muxC->ProcessUpdateA4(LOW);                                                                     // pin 14: Input A4: bit 15
    muxC->ProcessUpdateGb(LOW);                                                                     // pin 15: Input /G



    //
    // -- Counter for the bits 74HC193
    //    ----------------------------
    bits->ProcessUpdateB(LOW);                                                                      // pin 1: Input B
    // bit 2: output Bb
    // bit 3: output Ba
    connect(clock, &HW_Oscillator_t::SignalStateChanged, bits, &IC_74xx193_t::ProcessUpdateDown);   // pin 4: Down count clock
    bits->ProcessUpdateUp(HIGH);                                                                    // pin 5: Up Count clock (tied high)
    // bit 6: output Bc
    // bit 7: output Bd
    bits->ProcessUpdateD(HIGH);                                                                     // pin 9: Input D
    bits->ProcessUpdateC(LOW);                                                                      // pin 10: Input C
    connect(and2, &IC_74xx08_t::SignalY4Updated, bits, &IC_74xx193_t::ProcessUpdateLoad);           // pin 11: Load (#Qr * /BO)
    // bit 12: /CO Output
    // bit 13: /BO Output
    connect(nand1, &IC_74xx00_t::SignalY2Updated, bits, &IC_74xx193_t::ProcessUpdateClr);           // pin 14: CLR
    bits->ProcessUpdateA(HIGH);                                                                     // pin 15: Input A



    //
    // -- Counters for the address the least significant 74HC193
    //    ------------------------------------------------------
    addr0->ProcessUpdateB(LOW);                                                                     // pin 1: Input B
    // bit 2: output Addr0 Qb
    // bit 3: output Addr0 Qa
    addr0->ProcessUpdateDown(HIGH);                                                                 // pin 4: Down count clock (tied high)
    connect(nand1, &IC_74xx00_t::SignalY4Updated, addr0, &IC_74xx193_t::ProcessUpdateUp);           // pin 5: Up Count clock
    // bit 6: output Addr0 Qc
    // bit 7: output Addr0 Qd
    addr0->ProcessUpdateD(HIGH);                                                                    // pin 9: Input D
    addr0->ProcessUpdateC(HIGH);                                                                    // pin 10: Input C
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr0, &IC_74xx193_t::ProcessUpdateLoad);    // pin 11: Load (#Qr * /BO)
    // bit 12: /CO Output
    // bit 13: /BO Output
    addr0->ProcessUpdateClr(LOW);                                                                   // pin 14: CLR (never clear, only load)
    addr0->ProcessUpdateA(HIGH);                                                                    // pin 15: Input A



    //
    // -- Counters for the address the next least significant 74HC193
    //    -----------------------------------------------------------
    addr4->ProcessUpdateB(HIGH);                                                                    // pin 1: Input B
    // bit 2: output Addr4 Qb
    // bit 3: output Addr4 Qa
    connect(addr0, &IC_74xx193_t::SignalBoUpdated, addr4, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Down Count clock
    connect(addr0, &IC_74xx193_t::SignalCoUpdated, addr4, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Up Count clock
    // bit 6: output Addr4 Qc
    // bit 7: output Addr4 Qd
    addr4->ProcessUpdateD(HIGH);                                                                    // pin 9: Input D
    addr4->ProcessUpdateC(HIGH);                                                                    // pin 10: Input C
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr4, &IC_74xx193_t::ProcessUpdateLoad);    // pin 11: Load (#Qr * /BO)
    // bit 12: /CO Output
    // bit 13: /BO Output
    addr4->ProcessUpdateClr(LOW);                                                                   // pin 14: CLR (never clear, only load)
    addr4->ProcessUpdateA(HIGH);                                                                    // pin 15: Input A



    //
    // -- Counters for the address the next most significant 74HC193
    //    ----------------------------------------------------------
    addr8->ProcessUpdateB(HIGH);                                                                    // pin 1: Input B
    // bit 2: output Addr8 Qb
    // bit 3: output Addr8 Qa
    connect(addr4, &IC_74xx193_t::SignalBoUpdated, addr8, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Down Count clock
    connect(addr4, &IC_74xx193_t::SignalCoUpdated, addr8, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Up Count clock
    // bit 6: output Addr8 Qc
    // bit 7: output Addr8 Qd
    addr8->ProcessUpdateD(HIGH);                                                                    // pin 9: Input D
    addr8->ProcessUpdateC(HIGH);                                                                    // pin 10: Input C
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addr8, &IC_74xx193_t::ProcessUpdateLoad);    // pin 11: Load (#Qr * /BO)
    // bit 12: /CO Output
    // bit 13: /BO Output
    addr8->ProcessUpdateClr(LOW);                                                                   // pin 14: CLR (never clear, only load)
    addr8->ProcessUpdateA(HIGH);                                                                    // pin 15: Input A



    //
    // -- Counters for the address the most significant 74HC193
    //    -----------------------------------------------------
    addrC->ProcessUpdateB(HIGH);                                                                    // pin 1: Input B
    // bit 2: output AddrC Qb
    // bit 3: output AddrC Qa
    connect(addr8, &IC_74xx193_t::SignalBoUpdated, addrC, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Down Count clock
    connect(addr8, &IC_74xx193_t::SignalCoUpdated, addrC, &IC_74xx193_t::ProcessUpdateUp);          // pin 5: Up Count clock
    // bit 6: output AddrC Qc
    // bit 7: output AddrC Qd
    addrC->ProcessUpdateD(LOW);                                                                     // pin 9: Input D
    addrC->ProcessUpdateC(HIGH);                                                                    // pin 10: Input C
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, addrC, &IC_74xx193_t::ProcessUpdateLoad);    // pin 11: Load (#Qr * /BO)
    // bit 12: /CO Output
    // bit 13: /BO Output
    addrC->ProcessUpdateClr(LOW);                                                                   // pin 14: CLR (never clear, only load)
    addrC->ProcessUpdateA(HIGH);                                                                    // pin 15: Input A



    //
    // -- Wire up the command/address shifter
    //    -----------------------------------
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, shift, &IC_74xx165_t::ProcessUpdateShLd);    // pin 1: SH or /LD selection
    connect(and1, &IC_74xx08_t::SignalY2Updated, shift, &IC_74xx165_t::ProcessUpdateClk);           // pin 2: Clock (Qc * CLK * #Qs)
    shift->ProcessUpdateE(LOW);                                                                     // pin 3: bit E
    shift->ProcessUpdateF(LOW);                                                                     // pin 4: bit F
    shift->ProcessUpdateG(LOW);                                                                     // pin 5: bit G
    shift->ProcessUpdateH(LOW);                                                                     // pin 6: bit H
    // pin 7: output #Qh for chaining ICs
    // pin 9: output Qh for chaining ICs
    shift->ProcessUpdateSer(LOW);                                                                   // pin 10: serial input (always 0)
    shift->ProcessUpdateA(HIGH);                                                                    // pin 11: bit A
    shift->ProcessUpdateB(HIGH);                                                                    // pin 12: bit B
    shift->ProcessUpdateC(LOW);                                                                     // pin 13: bit C
    shift->ProcessUpdateD(LOW);                                                                     // pin 14: bit D
    shift->ProcessUpdateClkInh(LOW);                                                                // pin 15: Clock Inhibit


    //
    // -- Wire up the LEDs
    //    ----------------
    connect(clock, &HW_Oscillator_t::SignalStateChanged, clk, &GUI_Led_t::ProcessStateChange);

    connect(nand1, &IC_74xx00_t::SignalY1Updated, cpy, &GUI_Led_t::ProcessStateChange);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, oe, &GUI_Led_t::ProcessStateChange);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, we, &GUI_Led_t::ProcessStateChange);
    connect(and3, &IC_74xx08_t::SignalY2Updated, ce, &GUI_Led_t::ProcessStateChange);

    connect(bits, &IC_74xx193_t::SignalQaUpdated, bit0, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQbUpdated, bit1, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQcUpdated, bit2, &GUI_Led_t::ProcessStateChange);
    connect(bits, &IC_74xx193_t::SignalQdUpdated, bit3, &GUI_Led_t::ProcessStateChange);



    //
    // -- Connect up the outputs: individual signals first
    //    ------------------------------------------------
    HW_Bus_1_t *cpyHld = HW_Computer_t::GetCpyHldBus();
    oNand1->setObjectName("CtrlCtrl:oNand1");
    connect(oNand1, &IC_74xx03_t::SignalY1Updated, cpyHld, &HW_Bus_1_t::ProcessUpdateBit0);
    connect(resetting, &IC_74xx74_t::SignalQ1Updated, this, &CtrlRomCtrlModule_t::SignalQrUpdated);
    connect(resetting, &IC_74xx74_t::SignalQ1bUpdated, this, &CtrlRomCtrlModule_t::SignalQrbUpdated);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::SignalQcUpdated);
    connect(resetting, &IC_74xx74_t::SignalQ2bUpdated, this, &CtrlRomCtrlModule_t::SignalQcbUpdated);
    connect(nand1, &IC_74xx00_t::SignalY2Updated, this, &CtrlRomCtrlModule_t::ProcessQcb);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, this, &CtrlRomCtrlModule_t::SignalQsUpdated);
    connect(nand1, &IC_74xx00_t::SignalY3Updated, this, &CtrlRomCtrlModule_t::SignalQsbUpdated);
    connect(and3, &IC_74xx08_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::SignalShiftClockUpdated);
    connect(or1, &IC_74xx32_t::SignalY4Updated, this, &CtrlRomCtrlModule_t::SignalEepromCsUpdated);
    connect(shift, &IC_74xx165_t::SignalQHUpdated, this, &CtrlRomCtrlModule_t::SignalEepromCmdAddrUpdated);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::SignalSramOeUpdated);
    connect(nand2, &IC_74xx00_t::SignalY2Updated, this, &CtrlRomCtrlModule_t::SignalSramWeUpdated);
    connect(and3, &IC_74xx08_t::SignalY2Updated, this, &CtrlRomCtrlModule_t::SignalSramCeUpdated);



    //
    // -- Connect up the outputs: Ctrl Logic Bus
    //    --------------------------------------
    HW_Bus_16_t *ctrlBus = HW_Computer_t::Get()->GetCtrlMidPlane()->GetCtrlBus();
    connect(mux0, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(mux0, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(mux0, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(mux0, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(mux4, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(mux4, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(mux4, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(mux4, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(mux8, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(mux8, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(mux8, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(mux8, &IC_74xx157_t::SignalY4Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(muxC, &IC_74xx157_t::SignalY1Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(muxC, &IC_74xx157_t::SignalY2Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(muxC, &IC_74xx157_t::SignalY3Updated, ctrlBus, &HW_Bus_16_t::ProcessUpdateBitE);
    ctrlBus->SignalBitFUpdated(LOW);



    //
    // -- Connect up the outputs: Address Copy Bus
    //    ----------------------------------------
    HW_Bus_16_t *addrCopyBus = HW_Computer_t::GetAddrCopyBus();
    connect(addr0, &IC_74xx193_t::SignalQaUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit0);
    connect(addr0, &IC_74xx193_t::SignalQbUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit1);
    connect(addr0, &IC_74xx193_t::SignalQcUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit2);
    connect(addr0, &IC_74xx193_t::SignalQdUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit3);
    connect(addr4, &IC_74xx193_t::SignalQaUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit4);
    connect(addr4, &IC_74xx193_t::SignalQbUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit5);
    connect(addr4, &IC_74xx193_t::SignalQcUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit6);
    connect(addr4, &IC_74xx193_t::SignalQdUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit7);
    connect(addr8, &IC_74xx193_t::SignalQaUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit8);
    connect(addr8, &IC_74xx193_t::SignalQbUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBit9);
    connect(addr8, &IC_74xx193_t::SignalQcUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBitA);
    connect(addr8, &IC_74xx193_t::SignalQdUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBitB);
    connect(addrC, &IC_74xx193_t::SignalQaUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBitC);
    connect(addrC, &IC_74xx193_t::SignalQbUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBitD);
    connect(addrC, &IC_74xx193_t::SignalQdUpdated, addrCopyBus, &HW_Bus_16_t::ProcessUpdateBitE);
    addrCopyBus->ProcessUpdateBitF(LOW);
}



//
// -- Start the clock when reset hits
//    -------------------------------
inline void CtrlRomCtrlModule_t::ProcessResetUpdate(TriState_t state)
{
#if defined(PEDANTIC_COPY) && (PEDANTIC_COPY == 1)
    // -- This is still an active low signal!!!

//    DEBUG << "CtrlCtrl: Reset is " << state;

    nand1->ProcessUpdateA1(state);          // SR Set
    nand2->ProcessUpdateA4(state);          // #Reset
    nand2->ProcessUpdateB4(state);          // #Reset
    resetting->ProcessUpdatePre1(state);    // set the D-Latch
    resetting->ProcessUpdatePre2(state);    // set the D-Latch

    if (state == HIGH) {
        clock->StartTimer();
    }
#else
    if (state != HIGH) {
        HW_Bus_1_t *rHld = HW_Computer_t::GetRHldBus();
        rHld->ProcessUpdateBit0(LOW);
        emit CopyEeprom();
        rHld->ProcessUpdateBit0(Z);
    } else {
        // -- disable #WE immediately
        nand2->ProcessB2Low();

        // -- reset both D-Latches
        resetting->ProcessUpdateClr1(LOW);
        resetting->ProcessUpdateClr2(LOW);
        resetting->ProcessUpdateClr1(HIGH);
        resetting->ProcessUpdateClr2(HIGH);

        // -- reset the bits counter
        bits->ProcessUpdateClr(HIGH);
        bits->ProcessUpdateClr(LOW);

        // -- keep an eye on this: it may change at inopportune times
        emit nand1->SignalY1Updated(LOW);
    }

#endif
}




//
// -- A helper functions to set up the debugging connections
//    ------------------------------------------------------
void CtrlRomCtrlModule_t::SetDebug(void)
{
//    connect(resetting, &IC_74xx74_t::SignalQ1Updated, this, &CtrlRomCtrlModule_t::DebugQr);
//    connect(nand1, &IC_74xx00_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::DebugQc);
//    connect(nand2, &IC_74xx00_t::SignalY1Updated, this, &CtrlRomCtrlModule_t::DebugQl);
//    connect(nand1, &IC_74xx00_t::SignalY4Updated, this, &CtrlRomCtrlModule_t::DebugQs);

    // -- indicate the clock counts for copying the ROM
//    connect(nand1, &IC_74xx00_t::SignalY4Updated, this, &CtrlRomCtrlModule_t::DebugClock);
}


