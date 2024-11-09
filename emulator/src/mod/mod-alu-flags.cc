//===================================================================================================================
//  mod-alu-flags.cc -- This class represents the entirety of theALU flags module
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-27  Initial  v0.0.1   Initial version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/mod-alu-flags.moc.cc"



//
// -- construct a new ALU Flags Module
//    --------------------------------
AluFlagsModule_t::AluFlagsModule_t(const QString name, TriState_t x) : QGroupBox(name), xState(x)
{
    setFixedWidth(95);
    setFixedHeight(50);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- Allocate all the necessary components
//    -------------------------------------
void AluFlagsModule_t::AllocateComponents(void)
{
    // -- allocate the X Flag stuff
    xFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);


    // -- allocate the Z Flag stuff
    zcLatch = new IC_74xx74_t;
    zFlagNandMsb = new IC_74xx30_t;
    zFlagNandLsb = new IC_74xx30_t;
    zFlagInvMsb = new IC_74xx540_t;
    zFlagInvLsb = new IC_74xx540_t;
    nor1 = new IC_74xx02_t;
    and1 = new IC_74xx08_t;
    zFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);


    // -- allocate the additional things for the C Flag
    cMux = new IC_74xx151_t;
    inv1 = new IC_74xx04_t;
    cFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);


    // -- allocate the additional components needed for the N Flag
    nvLatch = new IC_74xx74_t;
    nFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);

    // -- allocate the additional components for the V Flag
    xor1 = new IC_74xx86_t;
    vFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);

    // -- allocate the additional components for the L Flag
    lLatch = new IC_74xx74_t;
    lFlag = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::red);
}



//
// -- Put the GUI components together
//    -------------------------------
void AluFlagsModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid
    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter);
    layout->setSpacing(1);
    setLayout(layout);


    // -- Add the X flag into the layout
    layout->addWidget(xFlag, 0, 0);
    layout->addWidget(new QLabel("X"), 1, 0);
    layout->addWidget(zFlag, 0, 1);
    layout->addWidget(new QLabel("Z"), 1, 1);
    layout->addWidget(cFlag, 0, 2);
    layout->addWidget(new QLabel("C"), 1, 2);
    layout->addWidget(nFlag, 0, 3);
    layout->addWidget(new QLabel("N"), 1, 3);
    layout->addWidget(vFlag, 0, 4);
    layout->addWidget(new QLabel("V"), 1, 4);
    layout->addWidget(lFlag, 0, 5);
    layout->addWidget(new QLabel("L"), 1, 5);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void AluFlagsModule_t::TriggerFirstUpdate(void)
{
    zcLatch->TriggerFirstUpdate();
    zFlagNandMsb->TriggerFirstUpdate();
    zFlagNandLsb->TriggerFirstUpdate();
    zFlagInvMsb->TriggerFirstUpdate();
    zFlagInvLsb->TriggerFirstUpdate();
    nor1->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    cMux->TriggerFirstUpdate();
    inv1->TriggerFirstUpdate();
    nvLatch->TriggerFirstUpdate();
    xor1->TriggerFirstUpdate();
    lLatch->TriggerFirstUpdate();
}



//
// -- complete the wire-up for this module
//    ------------------------------------
void AluFlagsModule_t::WireUp(void)
{
    //
    // -- start with the X flag
    //    ---------------------
    xFlag->ProcessStateChange(xState);


    //
    // -- wire up the Z flag -- start with the MSB Inv from the bus
    //    ---------------------------------------------------------
    HW_Bus_16_t *mainBus = HW_Computer_t::GetMainBus();
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD7);
    connect(mainBus, &HW_Bus_16_t::SignalBitEUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD6);
    connect(mainBus, &HW_Bus_16_t::SignalBitDUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD5);
    connect(mainBus, &HW_Bus_16_t::SignalBitCUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD4);
    connect(mainBus, &HW_Bus_16_t::SignalBitBUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD3);
    connect(mainBus, &HW_Bus_16_t::SignalBitAUpdated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD2);
    connect(mainBus, &HW_Bus_16_t::SignalBit9Updated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD1);
    connect(mainBus, &HW_Bus_16_t::SignalBit8Updated, zFlagInvMsb, &IC_74xx540_t::ProcessUpdateD0);


    // -- LSB Inv from the bus
    connect(mainBus, &HW_Bus_16_t::SignalBit7Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD7);
    connect(mainBus, &HW_Bus_16_t::SignalBit6Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD6);
    connect(mainBus, &HW_Bus_16_t::SignalBit5Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD5);
    connect(mainBus, &HW_Bus_16_t::SignalBit4Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD4);
    connect(mainBus, &HW_Bus_16_t::SignalBit3Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD3);
    connect(mainBus, &HW_Bus_16_t::SignalBit2Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD2);
    connect(mainBus, &HW_Bus_16_t::SignalBit1Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD1);
    connect(mainBus, &HW_Bus_16_t::SignalBit0Updated, zFlagInvLsb, &IC_74xx540_t::ProcessUpdateD0);


    // -- connect the inverting driver to the NAND Gates -- MSB
    zFlagInvMsb->ProcessUpdateOE1(LOW);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY7Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateH);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY6Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateG);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY5Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateF);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY4Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateE);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY3Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateD);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY2Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateC);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY1Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateB);
    connect(zFlagInvMsb, &IC_74xx540_t::SignalY0Updated, zFlagNandMsb, &IC_74xx30_t::ProcessUpdateA);
    zFlagInvMsb->ProcessUpdateOE2(LOW);


    // -- connect the inverting driver to the NAND Gates -- LSB
    zFlagInvLsb->ProcessUpdateOE1(LOW);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY7Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateH);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY6Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateG);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY5Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateF);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY4Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateE);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY3Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateD);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY2Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateC);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY1Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateB);
    connect(zFlagInvLsb, &IC_74xx540_t::SignalY0Updated, zFlagNandLsb, &IC_74xx30_t::ProcessUpdateA);
    zFlagInvLsb->ProcessUpdateOE2(LOW);


    // -- connect the outputs of the 2 8-input NAND gates to the 2 inputs of the NOR gate
    connect(zFlagNandMsb, &IC_74xx30_t::SignalYUpdated, nor1, &IC_74xx02_t::ProcessUpdateA1);
    connect(zFlagNandLsb, &IC_74xx30_t::SignalYUpdated, nor1, &IC_74xx02_t::ProcessUpdateB1);
    nor1->ProcessA2Low();
    nor1->ProcessB2Low();
    nor1->ProcessA3Low();
    nor1->ProcessB3Low();
    nor1->ProcessA4Low();
    nor1->ProcessB4Low();


    // -- now connect the z-side of the latch
    zcLatch->ProcessUpdateClr1(HIGH);
    connect(nor1, &IC_74xx02_t::SignalY1Updated, zcLatch, &IC_74xx74_t::ProcessUpdateD1);
    connect(and1, &IC_74xx08_t::SignalY1Updated, zcLatch, &IC_74xx74_t::ProcessUpdateClockLatch1);
    connect(and1, &IC_74xx08_t::SignalY1Updated, zcLatch, &IC_74xx74_t::ProcessUpdateClockOutput1);
    zcLatch->ProcessUpdatePre1(HIGH);


    // -- the Z-related gate on and1
    // A1 comes from the clock, handled below
    // B1 is the Z-Latch signal, handled below
    // A2 comes from the clock, handled below
    // B2 is the C-Latch signal, handled below
    // A3 comes from the clock, handled below
    // B3 is the NVL-Latch signal, handled below
    // A4 is used in the V calculation
    // B4 is used in the V calculation


    // -- connect up the LED
    connect(zcLatch, &IC_74xx74_t::SignalQ1Updated, zFlag, &GUI_Led_t::ProcessStateChange);


    // -- Connect Set and Clear Carry signals
    // A1 will be handled below for Set Carry
    // A2 will be handled belos for Clear Carry
    // A3 will be handled below for Set Overflow
    // A3 will be handled belos for Clear Overflow
    inv1->ProcessA5Low();
    inv1->ProcessA6Low();


    //
    // -- Connections for the C Flag -- the 151 Mux
    //
    //    It is important to understand how this will be wired up.  There are 3 components to the ALU which
    //    will assert onto the Main Bus.  There are 3 assert signals which handle marshalling the asserts.
    //    These are active HIGH signals coming from the Control Logic.  All 3 of these signals will be wired
    //    into A, B, and C of the Mux.  So, this will have this possible set of selections:
    //    D0 -- no Carry flag input
    //    D1 -- ALU Adder Assert to Main
    //    D2 -- ALU Shift Unit Assert to Main
    //    D4 -- ALU Logic Unit Assert to Main
    //
    //    Now, note that there is nothing from the Logic Unit which will update the C Flag, but I am going to wire
    //    it in anyway.
    //    ----------------------------------------------------------------------------------------------------------
    cMux->ProcessUpdateD0(LOW);                     // -- used if nothing is selected
    // D1 is handled from the Adder Input           // -- used if the Adder is selected
    // D2 is handled from the Shift Unit Input      // -- used if the Shift Unit is selected
    cMux->ProcessUpdateD3(LOW);
    cMux->ProcessUpdateD4(LOW);                     // -- typically unused; allocated to Logic Unit
    cMux->ProcessUpdateD5(LOW);
    cMux->ProcessUpdateD6(LOW);
    cMux->ProcessUpdateD7(LOW);
    // A is handled below                           // -- Adder Assert
    // B is handled below                           // -- Shifter Assert
    // C is handled below                           // -- Logic Assert
    cMux->ProcessUpdateST(LOW);                     // -- always outputting

    // -- TODO: Remove once testing is complete
    cMux->ProcessUpdateA(HIGH);


    // -- connect up the clock and assert to the and1 gate
    // -- A2 comes from the clock, handled below
    // -- B2 is the C-Latch signal, handled below


    // -- connect up the C-side signals to the latch
    connect(inv1, &IC_74xx04_t::SignalY2Updated, zcLatch, &IC_74xx74_t::ProcessUpdateClr2);
    connect(cMux, &IC_74xx151_t::SignalYUpdated, zcLatch, &IC_74xx74_t::ProcessUpdateD2);
    connect(and1, &IC_74xx08_t::SignalY2Updated, zcLatch, &IC_74xx74_t::ProcessUpdateClockLatch2);
    connect(and1, &IC_74xx08_t::SignalY2Updated, zcLatch, &IC_74xx74_t::ProcessUpdateClockOutput2);
    connect(inv1, &IC_74xx04_t::SignalY1Updated, zcLatch, &IC_74xx74_t::ProcessUpdatePre2);


    // -- connect up the LED
    connect(zcLatch, &IC_74xx74_t::SignalQ2Updated, cFlag, &GUI_Led_t::ProcessStateChange);


    // -- Connect the ALU Carry outputs to the respective inputs
    connect(HW_Computer_t::GetAlu()->GetAluAdder(), &HW_AluAdder_t::SignalCarryOutUpdated,
                    cMux, &IC_74xx151_t::ProcessUpdateD1);
    // TODO: add the other inputs as they are built out



    //
    // -- Wire up the N Flag signals; N-side of the latch
    //    -----------------------------------------------
    nvLatch->ProcessUpdateClr1(HIGH);
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, nvLatch, &IC_74xx74_t::ProcessUpdateD1);
    connect(and1, &IC_74xx08_t::SignalY3Updated, nvLatch, &IC_74xx74_t::ProcessUpdateClockLatch1);
    connect(and1, &IC_74xx08_t::SignalY3Updated, nvLatch, &IC_74xx74_t::ProcessUpdateClockOutput1);
    nvLatch->ProcessUpdatePre1(HIGH);

    // -- the N-related gate on and1
    // A3 comes from the clock, handled below
    // B3 is the NVL-Latch signal, handled below

    // -- connect up the LED
    connect(nvLatch, &IC_74xx74_t::SignalQ1Updated, nFlag, &GUI_Led_t::ProcessStateChange);



    //
    // -- Wire up the V Flag signals; starting with the ALU input MSbits
    //    --------------------------------------------------------------
    HW_Bus_16_t *aluA = HW_Computer_t::GetAluABus();
    HW_Bus_16_t *aluB = HW_Computer_t::GetAluBBus();

    connect(aluA, &HW_Bus_16_t::SignalBitFUpdated, xor1, &IC_74xx86_t::ProcessUpdateA1);
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, xor1, &IC_74xx86_t::ProcessUpdateB1);
    connect(aluB, &HW_Bus_16_t::SignalBitFUpdated, xor1, &IC_74xx86_t::ProcessUpdateA2);
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, xor1, &IC_74xx86_t::ProcessUpdateB2);
    // A3 is handled in the L flag calculation
    // B3 is handled in the L flag calculation
    xor1->ProcessA4Low();
    xor1->ProcessB4Low();

    // -- connect up the outputs to the AND1 gate
    connect(xor1, &IC_74xx86_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateA4);
    connect(xor1, &IC_74xx86_t::SignalY2Updated, and1, &IC_74xx08_t::ProcessUpdateB4);

    // -- connect up the V-side signals to the latch
    connect(inv1, &IC_74xx04_t::SignalY4Updated, nvLatch, &IC_74xx74_t::ProcessUpdateClr2);
    connect(and1, &IC_74xx08_t::SignalY4Updated, nvLatch, &IC_74xx74_t::ProcessUpdateD2);
    connect(and1, &IC_74xx08_t::SignalY3Updated, nvLatch, &IC_74xx74_t::ProcessUpdateClockLatch2);
    connect(and1, &IC_74xx08_t::SignalY3Updated, nvLatch, &IC_74xx74_t::ProcessUpdateClockOutput2);
    connect(inv1, &IC_74xx04_t::SignalY3Updated, nvLatch, &IC_74xx74_t::ProcessUpdatePre2);

    // -- the LED
    connect(nvLatch, &IC_74xx74_t::SignalQ2Updated, vFlag, &GUI_Led_t::ProcessStateChange);


    //
    // -- Wire up the L Flag signals; starting with an XOR
    //    ------------------------------------------------
    connect(mainBus, &HW_Bus_16_t::SignalBitFUpdated, xor1, &IC_74xx86_t::ProcessUpdateA3);
    connect(and1, &IC_74xx08_t::SignalY4Updated, xor1, &IC_74xx86_t::ProcessUpdateB3);


    // -- connect up the L-side signals to the latch
    lLatch->ProcessUpdateClr1(HIGH);
    connect(xor1, &IC_74xx86_t::SignalY3Updated, lLatch, &IC_74xx74_t::ProcessUpdateD1);
    connect(and1, &IC_74xx08_t::SignalY3Updated, lLatch, &IC_74xx74_t::ProcessUpdateClockLatch1);
    connect(and1, &IC_74xx08_t::SignalY3Updated, lLatch, &IC_74xx74_t::ProcessUpdateClockOutput1);
    lLatch->ProcessUpdatePre1(HIGH);
    lLatch->ProcessUpdateClr2(HIGH);
    lLatch->ProcessUpdateD2(LOW);
    lLatch->ProcessUpdateClockLatch2(LOW);
    lLatch->ProcessUpdateClockOutput2(LOW);
    lLatch->ProcessUpdatePre2(HIGH);

    // -- the LED
    connect(lLatch, &IC_74xx74_t::SignalQ1Updated, lFlag, &GUI_Led_t::ProcessStateChange);
}



//
// -- Handle the inbound Clock
//    ------------------------
void AluFlagsModule_t::ProcessClockLatch(TriState_t state)
{
}



//
// -- Handle the inbound Clock
//    ------------------------
void AluFlagsModule_t::ProcessClockOutput(TriState_t state)
{
    and1->ProcessUpdateA1(state);
    and1->ProcessUpdateA2(state);
    and1->ProcessUpdateA3(state);
}



//
// -- Signal to latch the Z Flag
//    --------------------------
inline void AluFlagsModule_t::ProcessZLatch(TriState_t state)
{
    and1->ProcessUpdateB1(state);
}



//
// -- Signal to latch the C Flag
//    --------------------------
inline void AluFlagsModule_t::ProcessCLatch(TriState_t state)
{
    and1->ProcessUpdateB2(state);
}



//
// -- This is the Assert Signal for the Adder onto the Bus
//    ----------------------------------------------------
inline void AluFlagsModule_t::ProcessAdderAssert(TriState_t state)
{
    cMux->ProcessUpdateA(state);
}



//
// -- This is the Assert Signal for the Shift Unit onto the Bus
//    ---------------------------------------------------------
inline void AluFlagsModule_t::ProcessShifterAssert(TriState_t state)
{
    cMux->ProcessUpdateB(state);
}



//
// -- This is the Assert Signal for the Logic Unit onto the Bus
//    ---------------------------------------------------------
inline void AluFlagsModule_t::ProcessLogicAssert(TriState_t state)
{
    cMux->ProcessUpdateC(state);
}



//
// -- Handle the Carry Flag from the Adder
//    ------------------------------------
inline void AluFlagsModule_t::ProcessAdderCarry(TriState_t state)
{
    cMux->ProcessUpdateD1(state);
}



//
// -- Handle the Carry Flag from the Shifter
//    --------------------------------------
inline void AluFlagsModule_t::ProcessShifterCarry(TriState_t state)
{
    cMux->ProcessUpdateD2(state);
}



//
// -- Handle the change of the Set Carry signal
//    -----------------------------------------
inline void AluFlagsModule_t::ProcessSetCarry(TriState_t state)
{
    inv1->ProcessUpdateA1(state);
}



//
// -- Handle the change of the Clear Carry signal
//    -------------------------------------------
inline void AluFlagsModule_t::ProcessClearCarry(TriState_t state)
{
    inv1->ProcessUpdateA2(state);
}



//
// -- Signal to latch the NVL Flags
//    -----------------------------
inline void AluFlagsModule_t::ProcessNVLLatch(TriState_t state)
{
    and1->ProcessUpdateB3(state);
}



//
// -- Handle the change of the Set Overflow signal
//    --------------------------------------------
inline void AluFlagsModule_t::ProcessSetOverflow(TriState_t state)
{
    inv1->ProcessUpdateA3(state);
}



//
// -- Handle the change of the Clear Overflow signal
//    ----------------------------------------------
inline void AluFlagsModule_t::ProcessClearOverflow(TriState_t state)
{
    inv1->ProcessUpdateA4(state);
}



