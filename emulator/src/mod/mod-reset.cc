//===================================================================================================================
//  mod-reset.cc -- This class the reset circuit, holding the system in reset until everything is ready
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/mod-reset.moc.cc"


//
// -- construct a new Control Rom Control Module
//    ------------------------------------------
ResetModule_t::ResetModule_t(void) : QGroupBox("Reset")
{
    setFixedWidth(130);
    setFixedHeight(50);
    setAlignment(Qt::AlignCenter);

    AllocateComponents();
    BuildGui();
    WireUp();
    SetDebug();
    TriggerFirstUpdate();
}



//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void ResetModule_t::AllocateComponents(void)
{
    nand1 = new IC_74xx00_t;
    nand2 = new IC_74xx00_t;
    and1 = new IC_74xx08_t;
    or1 = new IC_74xx32_t;
    latch = new IC_74xx74_t;
    resetting = new GUI_Led_t(GUI_Led_t::OnWhenLow, Qt::red);
    reset = new GUI_MomentarySwitch_t("Reset", LOW, HIGH); // -- fake the pull-up resisitor here
    counter = new IC_74xx193_t;
    nand3 = new IC_74xx00_t;
    nor1 = new IC_74xx02_t;
    vib1 = new IC_74xx123_t;
    oNand1 = new IC_74xx03_t;

    bitA = new GUI_DipSwitch_t;
    bitB = new GUI_DipSwitch_t;
    bitC = new GUI_DipSwitch_t;
    bitD = new GUI_DipSwitch_t;

    bitD->setSliderPosition(0);
    bitC->setSliderPosition(1);
    bitB->setSliderPosition(1);
    bitA->setSliderPosition(1);
}



//
// -- Build the GUI for the Control Rom Control Module
//    ------------------------------------------------
void ResetModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid of LEDs and labels
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter);
    layout->setSpacing(1);
    setLayout(layout);


    layout->addWidget(bitD, Qt::AlignHCenter);
    layout->addWidget(bitC, Qt::AlignHCenter);
    layout->addWidget(bitB, Qt::AlignHCenter);
    layout->addWidget(bitA, Qt::AlignHCenter);

    layout->addWidget(resetting, Qt::AlignHCenter);
    layout->addWidget(reset, Qt::AlignHCenter);
}



//
// -- Trigger all the proper initial updates
//    --------------------------------------
void ResetModule_t::TriggerFirstUpdate(void)
{
    nand1->TriggerFirstUpdate();
    nand2->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    or1->TriggerFirstUpdate();
    latch->TriggerFirstUpdate();
    counter->TriggerFirstUpdate();
    nand3->TriggerFirstUpdate();
    nor1->TriggerFirstUpdate();
    vib1->TriggerFirstUpdate();
    oNand1->TriggerFirstUpdate();

    bitA->TriggerFirstUpdate();
    bitB->TriggerFirstUpdate();
    bitC->TriggerFirstUpdate();
    bitD->TriggerFirstUpdate();
}



//
// -- Complete the wire-up for the Control ROM Control Module
//    -------------------------------------------------------
void ResetModule_t::WireUp(void)
{
    // -- inputs into nand1; gate 1
    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, nand1, &IC_74xx00_t::ProcessUpdateA1);
    connect(nand1, &IC_74xx00_t::SignalY2Updated, nand1, &IC_74xx00_t::ProcessUpdateB1);

    // -- inputs into nand1; gate 2
    connect(nand1, &IC_74xx00_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateA2);
    connect(nand1, &IC_74xx00_t::SignalY3Updated, nand1, &IC_74xx00_t::ProcessUpdateB2);

    // -- inputs into nand1; gate 3
    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, nand1, &IC_74xx00_t::ProcessUpdateA3);
    connect(vib1, &IC_74xx123_t::SignalQ1Updated, nand1, &IC_74xx00_t::ProcessUpdateB3);

    // -- inputs into nand1; gate 4
    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, nand1, &IC_74xx00_t::ProcessUpdateA4);
    connect(vib1, &IC_74xx123_t::SignalQ2Updated, nand1, &IC_74xx00_t::ProcessUpdateB4);



    // -- inputs into nand2; gate 1
    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, nand2, &IC_74xx00_t::ProcessUpdateA1);
    connect(nand2, &IC_74xx00_t::SignalY2Updated, nand2, &IC_74xx00_t::ProcessUpdateB1);

    // -- inputs into nand2; gate 2
    connect(nand2, &IC_74xx00_t::SignalY1Updated, nand2, &IC_74xx00_t::ProcessUpdateA2);
    connect(nor1, &IC_74xx02_t::SignalY1Updated, nand2, &IC_74xx00_t::ProcessUpdateB2);

    // -- inputs into nand2; gate 3
    HW_Bus_1_t *copyHold = HW_Computer_t::GetCpyHldBus();
    connect(copyHold, &HW_Bus_1_t::SignalBit0Updated, nand2, &IC_74xx00_t::ProcessUpdateA3);
    connect(nand2, &IC_74xx00_t::SignalY4Updated, nand2, &IC_74xx00_t::ProcessUpdateB3);

    // -- inputs into nand2; gate 4
    connect(nand2, &IC_74xx00_t::SignalY3Updated, nand2, &IC_74xx00_t::ProcessUpdateA4);
    connect(counter, &IC_74xx193_t::SignalBoUpdated, nand2, &IC_74xx00_t::ProcessUpdateB4);



    // -- inputs into and1; gate 1
    // -- A1 below
    connect(nand1, &IC_74xx00_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateB1);

    // -- inputs into and1; gate 2
    connect(nand1, &IC_74xx00_t::SignalY2Updated, and1, &IC_74xx08_t::ProcessUpdateA2);
    // -- B2 below

    // -- inputs into and1; gate 3
    and1->ProcessA3Low();
    and1->ProcessB3Low();

    // -- inputs into and1; gate 4
    and1->ProcessA4Low();
    and1->ProcessB4Low();



    // -- inputs into or1; gate 1
    connect(and1, &IC_74xx08_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateA1);
    connect(and1, &IC_74xx08_t::SignalY2Updated, or1, &IC_74xx32_t::ProcessUpdateB1);

    // -- inputs into or1; gate 2
    connect(nand3, &IC_74xx00_t::SignalY3Updated, or1, &IC_74xx32_t::ProcessUpdateA2);
    connect(nand3, &IC_74xx00_t::SignalY3Updated, or1, &IC_74xx32_t::ProcessUpdateB2);

    // -- inputs into or1; gate 3
    or1->ProcessA3Low();
    or1->ProcessB3Low();

    // -- inputs into or1; gate 4
    or1->ProcessA4Low();
    or1->ProcessB4Low();



    // -- inputs into latch
    // -- latch 1
    latch->ProcessUpdateClr1(HIGH);
    connect(nand2, &IC_74xx00_t::SignalY3Updated, latch, &IC_74xx74_t::ProcessUpdateD1);
    // -- clock below
    latch->ProcessUpdatePre1(HIGH);


    // -- latch 2
    latch->ProcessUpdateClr2(LOW);
    latch->ProcessUpdateD2(LOW);
    latch->ProcessUpdateClockLatch2(LOW);
    latch->ProcessUpdateClockOutput2(LOW);
    latch->ProcessUpdatePre2(LOW);


    // -- inputs into the counter
    connect(bitA, &GUI_DipSwitch_t::SignalSwitchChanged, counter, &IC_74xx193_t::ProcessUpdateA);
    connect(bitB, &GUI_DipSwitch_t::SignalSwitchChanged, counter, &IC_74xx193_t::ProcessUpdateB);
    connect(bitC, &GUI_DipSwitch_t::SignalSwitchChanged, counter, &IC_74xx193_t::ProcessUpdateC);
    connect(bitD, &GUI_DipSwitch_t::SignalSwitchChanged, counter, &IC_74xx193_t::ProcessUpdateD);
    connect(copyHold, &HW_Bus_1_t::SignalBit0Updated, counter, &IC_74xx193_t::ProcessUpdateLoad);
    counter->ProcessUpdateUp(HIGH);
    counter->ProcessUpdateClr(LOW);



    // -- inputs into nand IC 3; gate 1
    connect(nand3, &IC_74xx00_t::SignalY3Updated, nand3, &IC_74xx00_t::ProcessUpdateA1);
    connect(nand3, &IC_74xx00_t::SignalY2Updated, nand3, &IC_74xx00_t::ProcessUpdateB1);

    // -- inputs into nand3; gate 2
    connect(nand3, &IC_74xx00_t::SignalY1Updated, nand3, &IC_74xx00_t::ProcessUpdateA2);
    connect(nor1, &IC_74xx02_t::SignalY2Updated, nand3, &IC_74xx00_t::ProcessUpdateB2);

    // -- inputs into nand3; gate 3
    connect(nand3, &IC_74xx00_t::SignalY4Updated, nand3, &IC_74xx00_t::ProcessUpdateA3);
    connect(nand3, &IC_74xx00_t::SignalY4Updated, nand3, &IC_74xx00_t::ProcessUpdateB3);

    // -- inputs into nand3; gate 4
    connect(nand3, &IC_74xx00_t::SignalY3Updated, nand3, &IC_74xx00_t::ProcessUpdateA4);
    connect(nor1, &IC_74xx02_t::SignalY3Updated, nand3, &IC_74xx00_t::ProcessUpdateB4);



    // -- inputs for nor1; gate 1
    connect(nand3, &IC_74xx00_t::SignalY2Updated, nor1, &IC_74xx02_t::ProcessUpdateA1);
    connect(nand3, &IC_74xx00_t::SignalY4Updated, nor1, &IC_74xx02_t::ProcessUpdateB1);

    // -- inputs for nor1; gate 2
    connect(nand2, &IC_74xx00_t::SignalY2Updated, nor1, &IC_74xx02_t::ProcessUpdateA2);
    connect(nand3, &IC_74xx00_t::SignalY4Updated, nor1, &IC_74xx02_t::ProcessUpdateB2);

    // -- inputs for nor1; gate 3
    connect(nand2, &IC_74xx00_t::SignalY2Updated, nor1, &IC_74xx02_t::ProcessUpdateA3);
    connect(nand3, &IC_74xx00_t::SignalY2Updated, nor1, &IC_74xx02_t::ProcessUpdateB3);

    // -- inputs for nor1; gate 4
    connect(copyHold, &HW_Bus_1_t::SignalBit0Updated, nor1, &IC_74xx02_t::ProcessUpdateA4);
    connect(copyHold, &HW_Bus_1_t::SignalBit0Updated, nor1, &IC_74xx02_t::ProcessUpdateB4);



    // -- inputs for oNand1; gate 1
    connect(nand3, &IC_74xx00_t::SignalY3Updated, oNand1, &IC_74xx03_t::ProcessUpdateA1);
    connect(nand3, &IC_74xx00_t::SignalY3Updated, oNand1, &IC_74xx03_t::ProcessUpdateB1);

    // -- inputs for oNand1; gate 2
    oNand1->ProcessA2Low();
    oNand1->ProcessB2Low();

    // -- inputs for oNand1; gate 3
    oNand1->ProcessA3Low();
    oNand1->ProcessB3Low();

    // -- inputs for oNand1; gate 4
    oNand1->ProcessA4Low();
    oNand1->ProcessB4Low();



    // -- inputs for the vibrator
    vib1->ProcessUpdateA1b(LOW);
    connect(copyHold, &HW_Bus_1_t::SignalBit0Updated, vib1, &IC_74xx123_t::ProcessUpdateB1);
    vib1->ProcessUpdateRD1b(HIGH);
//TODO    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, vib1, &IC_74xx123_t::ProcessUpdateRD1b);

    vib1->ProcessUpdateA2b(LOW);
    connect(latch, &IC_74xx74_t::SignalQ1bUpdated, vib1, &IC_74xx123_t::ProcessUpdateB2);
    vib1->ProcessUpdateRD2b(HIGH);
//TODO    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, vib1, &IC_74xx123_t::ProcessUpdateRD2b);



    // -- connect the LED
    connect(nand3, &IC_74xx00_t::SignalY3Updated, resetting, &GUI_Led_t::ProcessStateChange);


    // -- connect the outputs
    connect(or1, &IC_74xx32_t::SignalY2Updated, this, &ResetModule_t::SignalSystemClock);
    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, this, &ResetModule_t::SignalReset);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, this, &ResetModule_t::SignalResetting);
    connect(nand3, &IC_74xx00_t::SignalY1Updated, this, &ResetModule_t::SignalClocking);
    connect(nand3, &IC_74xx00_t::SignalY3Updated, this, &ResetModule_t::SignalRunning);
    oNand1->setObjectName("Reset:oNand1");
    connect(oNand1, &IC_74xx03_t::SignalY1Updated, HW_Computer_t::GetRHldBus(), &HW_Bus_1_t::ProcessUpdateBit0);



    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateLatch, this, &ResetModule_t::ProcessCpuClockLatch);
    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateLatch, this, &ResetModule_t::ProcessHighSpeedClockLatch);
    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateOutput, this, &ResetModule_t::ProcessCpuClockOutput);
    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateOutput, this, &ResetModule_t::ProcessHighSpeedClockOutput);
}



//
// -- Process a change in the high-speed clock
//    ----------------------------------------
void ResetModule_t::ProcessHighSpeedClockLatch(TriState_t state)
{
    and1->ProcessUpdateA1(state);
}

void ResetModule_t::ProcessHighSpeedClockOutput(TriState_t state)
{
}



//
// -- Process a change in the cpu clock
//    ---------------------------------
void ResetModule_t::ProcessCpuClockLatch(TriState_t state)
{
    and1->ProcessUpdateB2(state);
    latch->ProcessUpdateClockLatch1(state);
    counter->ProcessUpdateDown(state);
}

void ResetModule_t::ProcessCpuClockOutput(TriState_t state)
{
    latch->ProcessUpdateClockOutput1(state);
}



//
// -- Process the power on reset
//    --------------------------
void ResetModule_t::PowerOnReset(void)
{
    debug = true;
    DEBUG << "Initialization Complete";
    DEBUG << "==================================";
//    DEBUG << "Reset: Power on reset";
    reset->ProcessClick();
    HW_Computer_t::GetClock()->StartClock();

    reset->ProcessRelease();
//    DEBUG << "Reset: Power on Reset complete";
}



//
// -- A helper functions to set up the debugging connections
//    ------------------------------------------------------
void ResetModule_t::SetDebug(void)
{
    // -- debugging connections
//    connect(reset, &GUI_MomentarySwitch_t::SignalSwitchChanged, this, &ResetModule_t::DebugReset);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, this, &ResetModule_t::DebugResetting);
    connect(HW_Computer_t::GetCpyHldBus(), &HW_Bus_1_t::SignalBit0Updated, this, &ResetModule_t::DebugCPYHLD);
    connect(nand3, &IC_74xx00_t::SignalY1Updated, this, &ResetModule_t::DebugClocking);
    connect(latch, &IC_74xx74_t::SignalQ1bUpdated, this, &ResetModule_t::DebugRun);
    connect(nand3, &IC_74xx00_t::SignalY3Updated, this, &ResetModule_t::DebugRunning);
//    connect(HW_Computer_t::GetRHldBus(), &HW_Bus_1_t::SignalBit0Updated, this, &ResetModule_t::DebugRHLD);

    connect(vib1, &IC_74xx123_t::SignalQ1Updated, this, &ResetModule_t::DebugVibrator1);
    connect(vib1, &IC_74xx123_t::SignalQ2Updated, this, &ResetModule_t::DebugVibrator2);

    connect(counter, &IC_74xx193_t::SignalBoUpdated, this , &ResetModule_t::DebugCounterBorrow);
//    connect(HW_Computer_t::GetCpyHldBus(), &HW_Bus_1_t::SignalBit0Updated, this, &ResetModule_t::DebugCounterLoad);
    connect(nand2, &IC_74xx00_t::SignalY3Updated, this, &ResetModule_t::DebugLatchInput);
}



