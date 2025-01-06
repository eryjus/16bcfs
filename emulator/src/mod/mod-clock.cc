//===================================================================================================================
//  mod-clock.cc -- This file implements the entirety of the clock module
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct a clock module.  Ideally, this class would
//  correlate 1:1 against the Clock Module KiCad design and its resulting PCB.  The interface point will only be
//  the inputs from and outputs to the computer backplane.  All other connections in real hardware are represented
//  here.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/mod-clock.moc.cc"



//
// -- Construct a clock module
//    ------------------------
ClockModule_t::ClockModule_t(void) : QGroupBox("Clock Module")
{
    clockCount = 0;

    setFixedWidth(190);
    setFixedHeight(150);

    AllocateComponents();
    BuildGui();
    WireUp();
    SetDebug();
    TriggerFirstUpdate();

    // -- start the oscillators as soon as they are placed
    osc->ProcessSetInterval(0);
    adj->ProcessSetInterval(500);

    osc->StartTimer();
    adj->StartTimer();
}



//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void ClockModule_t::AllocateComponents(void)
{
    osc = new HW_Oscillator_t;
    adj = new HW_Oscillator_t;
    speedPot = new HW_Pot_t;
    oscMomentary = new GUI_MomentarySwitch_t("Osc", LOW, HIGH);
    adjMomentary = new GUI_MomentarySwitch_t("Adj", LOW, HIGH);
    stepMomentary = new GUI_MomentarySwitch_t("Step", HIGH, LOW);
    singleStepDebounced = new HW_SpdtSwitch_t("Single Step", HW_SpdtSwitch_t::HIGH_WHEN_RELEASED);
    oscLed = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    adjLed = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    stepLed = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    actualClock = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::green);
    div0 = new GUI_DipSwitch_t;
    div1 = new GUI_DipSwitch_t;
    div2 = new GUI_DipSwitch_t;
    div3 = new GUI_DipSwitch_t;

    nand1 = new IC_74xx00_t;
    nand2 = new IC_74xx00_t;
    nor1 = new IC_74xx02_t;
    and1 = new IC_74xx08_t;
    and2 = new IC_74xx08_t;
    or1 = new IC_74xx32_t;
    latch1 = new IC_74xx74_t;
    divider1 = new IC_74xx193_t;


    div0->setSliderPosition(1);
    div1->setSliderPosition(0);
    div2->setSliderPosition(0);
    div3->setSliderPosition(0);
}


//
// -- Put the GUI components together
//    -------------------------------
void ClockModule_t::BuildGui(void)
{
    // -- the layout for this module will be a grid
    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setRowMinimumHeight(0, 30);
    layout->setRowMinimumHeight(1, 20);
    layout->setRowMinimumHeight(2, 40);
    layout->setRowMinimumHeight(3, 15);
    layout->setRowMinimumHeight(4, 25);
    layout->setRowMinimumHeight(5, 15);
    layout->setRowMinimumHeight(6, 30);
    layout->setRowMinimumHeight(7, 20);
    setAlignment(Qt::AlignHCenter);
    setLayout(layout);

    speedPot->setMinimumWidth(60);
    singleStepDebounced->setMinimumHeight(25);


    layout->addWidget(oscMomentary, 0, 0, Qt::AlignHCenter);
    layout->addWidget(oscLed, 0, 1, Qt::AlignHCenter);
    layout->addWidget(adjMomentary, 0, 2, Qt::AlignHCenter);
    layout->addWidget(adjLed, 0, 3, Qt::AlignHCenter);
    layout->addWidget(stepMomentary, 0, 4, Qt::AlignHCenter);
    layout->addWidget(stepLed, 0, 5, Qt::AlignHCenter);

    layout->addWidget(singleStepDebounced, 2, 0, 1, 4, Qt::AlignHCenter);
    layout->addWidget(actualClock, 2, 4, 1, 2, Qt::AlignHCenter);

    layout->addWidget(new QLabel("Speed"), 4, 0, 1, 2);
    layout->addWidget(speedPot, 4, 2, 1, 2, Qt::AlignHCenter);
    layout->addWidget(speedPot->GetLabel(), 4, 4, 1, 2);

    layout->addWidget(new QLabel("Divider"), 6, 0, 1, 2);
    layout->addWidget(div3, 6, 2, Qt::AlignHCenter);
    layout->addWidget(div2, 6, 3, Qt::AlignHCenter);
    layout->addWidget(div1, 6, 4, Qt::AlignHCenter);
    layout->addWidget(div0, 6, 5, Qt::AlignHCenter);
}


//
// -- Trigger all the proper initial updates
//    --------------------------------------
void ClockModule_t::TriggerFirstUpdate(void)
{
    speedPot->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
    nand2->TriggerFirstUpdate();
    nor1->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    or1->TriggerFirstUpdate();
    latch1->TriggerFirstUpdate();
    divider1->TriggerFirstUpdate();
    div0->TriggerFirstUpdate();
    div1->TriggerFirstUpdate();
    div2->TriggerFirstUpdate();
    div3->TriggerFirstUpdate();

    oscMomentary->ProcessRelease();
    adjMomentary->ProcessRelease();
    stepMomentary->ProcessRelease();

    singleStepDebounced->released();
}


//
// -- complete the wire-up for this module
//    ------------------------------------
void ClockModule_t::WireUp(void)
{
    //
    // -- The inputs into nand1
    //    ---------------------
    connect(oscMomentary, &GUI_MomentarySwitch_t::SignalSwitchChanged, nand1, &IC_74xx00_t::ProcessUpdateA1);
    connect(nand1, &IC_74xx00_t::SignalY2Updated, nand1, &IC_74xx00_t::ProcessUpdateB1);

    connect(nand1, &IC_74xx00_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateA2);
    connect(nor1, &IC_74xx02_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateB2);


    // -- the other 2 gates are unused
    nand1->ProcessA3Low();
    nand1->ProcessB3Low();
    nand1->ProcessA4Low();
    nand1->ProcessB4Low();



    //
    // -- The inputs into nand2
    //    ---------------------
    connect(and1, &IC_74xx08_t::SignalY1Updated, nand2, &IC_74xx00_t::ProcessUpdateA1);     // set
    connect(nand2, &IC_74xx00_t::SignalY2Updated, nand2, &IC_74xx00_t::ProcessUpdateB1);

    connect(nand2, &IC_74xx00_t::SignalY1Updated, nand2, &IC_74xx00_t::ProcessUpdateA2);
    connect(nor1, &IC_74xx02_t::SignalY2Updated, nand2, &IC_74xx00_t::ProcessUpdateB2);     // reset


    connect(nand2, &IC_74xx00_t::SignalY4Updated, nand2, &IC_74xx00_t::ProcessUpdateA3);
    connect(nor1, &IC_74xx02_t::SignalY3Updated, nand2, &IC_74xx00_t::ProcessUpdateB3);     // reset

    connect(nor1, &IC_74xx02_t::SignalY4Updated, nand2, &IC_74xx00_t::ProcessUpdateA4);     // set
    connect(nand2, &IC_74xx00_t::SignalY3Updated, nand2, &IC_74xx00_t::ProcessUpdateB4);



    //
    // -- Inputs for the nor gates
    //    ------------------------
    connect(nand2, &IC_74xx00_t::SignalY1Updated, nor1, &IC_74xx02_t::ProcessUpdateA1);
    connect(nand2, &IC_74xx00_t::SignalY4Updated, nor1, &IC_74xx02_t::ProcessUpdateB1);

    connect(nand1, &IC_74xx00_t::SignalY1Updated, nor1, &IC_74xx02_t::ProcessUpdateA2);
    connect(nand2, &IC_74xx00_t::SignalY4Updated, nor1, &IC_74xx02_t::ProcessUpdateB2);

    connect(nand1, &IC_74xx00_t::SignalY1Updated, nor1, &IC_74xx02_t::ProcessUpdateA3);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, nor1, &IC_74xx02_t::ProcessUpdateB3);


    connect(stepMomentary, &GUI_MomentarySwitch_t::SignalSwitchChanged, nor1, &IC_74xx02_t::ProcessUpdateA4);
    // B4 is handled in `ProcessSignalBreak`



    //
    // -- Inputs into the and gates
    //    -------------------------
    connect(adjMomentary, &GUI_MomentarySwitch_t::SignalSwitchChanged, and1, &IC_74xx08_t::ProcessUpdateA1);
    // B1 is handled in `ProcessReset`


    connect(divider1, &IC_74xx193_t::SignalBoUpdated, and1, &IC_74xx08_t::ProcessUpdateA2);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateB2);

    connect(adj, &HW_Oscillator_t::SignalStateChanged, and1, &IC_74xx08_t::ProcessUpdateA3);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, and1, &IC_74xx08_t::ProcessUpdateB3);

    connect(singleStepDebounced, &HW_SpdtSwitch_t::SignalState, and1, &IC_74xx08_t::ProcessUpdateA4);
    connect(nand2, &IC_74xx00_t::SignalY4Updated, and1, &IC_74xx08_t::ProcessUpdateB4);



    //
    // -- and 2 gates
    //    -----------
    connect(nand2, &IC_74xx00_t::SignalY3Updated, and2, &IC_74xx08_t::ProcessUpdateA1);
    connect(latch1, &IC_74xx74_t::SignalQ1Updated, and2, &IC_74xx08_t::ProcessUpdateB1);


    and2->ProcessA2High();
    and2->ProcessA3High();


    // -- the other 2 gates are drivers for LEDs
    connect(nand2, &IC_74xx00_t::SignalY4Updated, and2, &IC_74xx08_t::ProcessUpdateA3);
    connect(nand2, &IC_74xx00_t::SignalY4Updated, and2, &IC_74xx08_t::ProcessUpdateB3);

    connect(or1, &IC_74xx32_t::SignalY2Updated, and2, &IC_74xx08_t::ProcessUpdateA4);
    connect(or1, &IC_74xx32_t::SignalY2Updated, and2, &IC_74xx08_t::ProcessUpdateB4);



    //
    // -- Inputs into the Or gate
    //    -----------------------
    connect(and1, &IC_74xx08_t::SignalY2Updated, or1, &IC_74xx32_t::ProcessUpdateA1);
    connect(and1, &IC_74xx08_t::SignalY3Updated, or1, &IC_74xx32_t::ProcessUpdateB1);

    connect(and2, &IC_74xx08_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateA2);
    connect(and1, &IC_74xx08_t::SignalY4Updated, or1, &IC_74xx32_t::ProcessUpdateB2);


    // These 2 gates drive LEDs
    connect(nand1, &IC_74xx00_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateA3);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateB3);

    connect(nand2, &IC_74xx00_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateA4);
    connect(nand2, &IC_74xx00_t::SignalY1Updated, or1, &IC_74xx32_t::ProcessUpdateB4);



    //
    // -- Inputs for the latch
    //    --------------------
    latch1->ProcessUpdatePre1(HIGH);
    connect(latch1, &IC_74xx74_t::SignalQ1bUpdated, latch1, &IC_74xx74_t::ProcessUpdateD1);
    connect(or1, &IC_74xx32_t::SignalY1Updated, latch1, &IC_74xx74_t::ProcessUpdateClockLatch1);
    connect(or1, &IC_74xx32_t::SignalY1Updated, latch1, &IC_74xx74_t::ProcessUpdateClockOutput1);
    // Set1 is handled in `ProcessCpyHld`


    latch1->ProcessUpdateClr2(HIGH);
    latch1->ProcessUpdateD2(HIGH);
    latch1->ProcessUpdateClockLatch2(HIGH);
    latch1->ProcessUpdateClockOutput2(HIGH);
    latch1->ProcessUpdatePre2(HIGH);



    //
    // -- Inputs into the counter as the clock divider
    //    --------------------------------------------
    connect(div0, &GUI_DipSwitch_t::SignalSwitchChanged, divider1, &IC_74xx193_t::ProcessUpdateA);
    connect(div1, &GUI_DipSwitch_t::SignalSwitchChanged, divider1, &IC_74xx193_t::ProcessUpdateB);
    connect(div2, &GUI_DipSwitch_t::SignalSwitchChanged, divider1, &IC_74xx193_t::ProcessUpdateC);
    connect(div3, &GUI_DipSwitch_t::SignalSwitchChanged, divider1, &IC_74xx193_t::ProcessUpdateD);
    connect(divider1, &IC_74xx193_t::SignalBoUpdated, divider1, &IC_74xx193_t::ProcessUpdateLoad);
    divider1->ProcessUpdateUp(HIGH);
    connect(osc, &HW_Oscillator_t::SignalStateChanged, divider1, &IC_74xx193_t::ProcessUpdateDown);
    divider1->ProcessUpdateClr(LOW);



    //
    // -- Connect the HS_CLK crystal to its output
    //    ----------------------------------------
    connect(osc, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::ProcessHighSpeedClock);



    //
    // -- connect the CPU Clock to its output
    //    -----------------------------------
    connect(or1, &IC_74xx32_t::SignalY2Updated, this, &ClockModule_t::ProcessCpuClock);



    //
    // -- connect up the LEDs
    //    -------------------
    connect(or1, &IC_74xx32_t::SignalY3Updated, oscLed, &GUI_Led_t::ProcessStateChange);
    connect(or1, &IC_74xx32_t::SignalY4Updated, adjLed, &GUI_Led_t::ProcessStateChange);
    connect(and2, &IC_74xx08_t::SignalY3Updated, stepLed, &GUI_Led_t::ProcessStateChange);
    connect(and2, &IC_74xx08_t::SignalY4Updated, actualClock, &GUI_Led_t::ProcessStateChange);



    //
    // -- connect the pot to adjust the adjustible clock
    //    ----------------------------------------------
    connect(speedPot, &HW_Pot_t::SignalValueChanged, adj, &HW_Oscillator_t::ProcessRawInterval);
}



//
// -- Start the oscillators
//    ---------------------
void ClockModule_t::StartClock(void)
{
    osc->StartTimer();
    adj->StartTimer();
}



//
// -- When debugging, make all the proper connections
//    -----------------------------------------------
void ClockModule_t::SetDebug(void)
{
    // connect(osc, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::DebugOsc);
    // connect(divider1, &IC_74xx193_t::SignalBoUpdated, this, &ClockModule_t::DebugBo);
//    connect(adj, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::DebugAdj);

//    connect(and1, &IC_74xx08_t::SignalY1Updated, this, &ClockModule_t::DebugAdjSel);
//    connect(nand2, &IC_74xx00_t::SignalY1Updated, this, &ClockModule_t::DebugAdjSelOutput);
//    connect(latch1, &IC_74xx74_t::SignalQ1bUpdated, this, &ClockModule_t::DebugLatchDataIn);
    // connect(latch1, &IC_74xx74_t::SignalQ1Updated, this, &ClockModule_t::DebugLatchDataOut);
    //  connect(or1, &IC_74xx32_t::SignalY1Updated, this, &ClockModule_t::DebugLatchClock);
    // connect(and1, &IC_74xx08_t::SignalY3Updated, this, &ClockModule_t::DebugAdjClockTick);
    //connect(or1, &IC_74xx32_t::SignalY2Updated, this, &ClockModule_t::DebugCpuClock);
    // connect(div0, &GUI_DipSwitch_t::SignalSwitchChanged, this, &ClockModule_t::DebugDiv0);
    // connect(div1, &GUI_DipSwitch_t::SignalSwitchChanged, this, &ClockModule_t::DebugDiv1);
    // connect(div2, &GUI_DipSwitch_t::SignalSwitchChanged, this, &ClockModule_t::DebugDiv2);
    // connect(div3, &GUI_DipSwitch_t::SignalSwitchChanged, this, &ClockModule_t::DebugDiv3);
}



