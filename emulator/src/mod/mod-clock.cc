//===================================================================================================================
//  mod-clock.cc -- This file implements the entirety of the clock module
//
//      Copyright (c) 2023-2024 - Adam Clark
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
    hi = QPixmap("img/clk-hi.png");
    lo = QPixmap("img/clk-lo.png");

    clockCount = 0;

    setFixedWidth(190);
    setFixedHeight(150);

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();

    // -- start the oscillator as soon as it is placed
    timer->ProcessSetInterval(500);
}



//
// -- allocate all the components needed for this module
//    --------------------------------------------------
void ClockModule_t::AllocateComponents(void)
{
    timer = new HW_Oscillator_t;
    oscillatorVisual = new QLabel;
    speedPot = new HW_Pot_t;
    runModeMomentary = new HW_MomentarySwitch_t("Run", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED);
    stepModeMomentary = new HW_MomentarySwitch_t("Single", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED);
    singleStepDebounced = new HW_SpdtSwitch_t("Step");
    runModeLed = new GUI_Led_t;
    stepModeLed = new GUI_Led_t;
    actualClock = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    nand1 = new IC_74xx00_t;
    and1 = new IC_74xx08_t;
    nor1 = new IC_74xx02_t;
    latch = new IC_74xx74_t;
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
    setAlignment(Qt::AlignHCenter);
    setLayout(layout);


    // -- top row, left side is the Clock LED
    QGroupBox *clockLed = new QGroupBox("Clock");
    clockLed->setAlignment(Qt::AlignHCenter);
    clockLed->setFixedWidth(50);
    clockLed->setFixedHeight(60);
    QHBoxLayout *ledLayout = new QHBoxLayout();
    ledLayout->setAlignment(Qt::AlignHCenter);
    ledLayout->setContentsMargins(0, 0, 0, 0);
    ledLayout->setSpacing(2);
    ledLayout->addWidget(actualClock, 2, Qt::AlignHCenter);
    clockLed->setLayout(ledLayout);
    layout->addWidget(clockLed, 0, 0);


    // -- top row, middle is the oscillator graphic
    QGroupBox *oscGraphic = new QGroupBox("Sq Wave");
    QVBoxLayout *oscVLayout = new QVBoxLayout;
    oscVLayout->setContentsMargins(0, 0, 0, 0);
    oscGraphic->setLayout(oscVLayout);
    oscGraphic->setAlignment(Qt::AlignHCenter);
    oscGraphic->setFixedHeight(60);
    oscGraphic->setMaximumWidth(50);
    oscillatorVisual->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    oscillatorVisual->setPixmap(hi);
    oscVLayout->addWidget(oscillatorVisual);
    oscGraphic->setLayout(oscVLayout);
    layout->addWidget(oscGraphic, 0, 1);


    // -- top row, right side is the speed control
    QGroupBox *speedGroup = new QGroupBox("Speed");
    speedGroup->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    speedGroup->setFixedHeight(60);
    speedGroup->setMaximumWidth(50);
    QVBoxLayout *speedLayout = new QVBoxLayout();
    speedLayout->setAlignment(Qt::AlignHCenter);
    speedLayout->setContentsMargins(0, 0, 0, 0);
    speedLayout->setSpacing(2);
    speedLayout->addWidget(speedPot);
    speedLayout->addWidget(speedPot->GetLabel());
    speedGroup->setLayout(speedLayout);
    layout->addWidget(speedGroup, 0, 2);


    // -- bottom row, left 2 columns is the mode control
    QGroupBox *modeGroup = new QGroupBox("Clock Mode");
    modeGroup->setAlignment(Qt::AlignHCenter);
    modeGroup->setFixedWidth(120);
    modeGroup->setFixedHeight(60);
    QHBoxLayout *modeOptions = new QHBoxLayout();
    modeOptions->setAlignment(Qt::AlignHCenter);
    modeOptions->setContentsMargins(0, 0, 0, 0);
    modeOptions->setSpacing(2);
    QVBoxLayout *clkModeRun = new QVBoxLayout;
    clkModeRun->setAlignment(Qt::AlignHCenter);
    clkModeRun->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *clkModeStep = new QVBoxLayout;
    clkModeStep->setAlignment(Qt::AlignHCenter);
    clkModeStep->setContentsMargins(0, 0, 0, 0);
    clkModeRun->addWidget(runModeLed, 2, Qt::AlignHCenter);
    clkModeStep->addWidget(stepModeLed, 2, Qt::AlignHCenter);
    runModeMomentary->setMaximumHeight(15);
    clkModeRun->addWidget(runModeMomentary);
    stepModeMomentary->setMaximumHeight(15);
    clkModeStep->addWidget(stepModeMomentary);
    modeOptions->addLayout(clkModeRun);
    modeOptions->addLayout(clkModeStep);
    modeGroup->setLayout(modeOptions);
    layout->addWidget(modeGroup, 1, 0, 1, 2);


    // -- bottom row, right column is the single step switch
    QGroupBox *stepGroup = new QGroupBox("Step");
    stepGroup->setAlignment(Qt::AlignHCenter);
    stepGroup->setFixedHeight(60);
    stepGroup->setMinimumHeight(60);
    stepGroup->setMaximumHeight(60);
    stepGroup->setMaximumWidth(50);
    QHBoxLayout *stepLayout = new QHBoxLayout();
    stepLayout->setAlignment(Qt::AlignHCenter);
    stepLayout->setContentsMargins(0, 0, 0, 0);
    stepLayout->setSpacing(2);
    singleStepDebounced->setMaximumWidth(40);
    singleStepDebounced->setMaximumHeight(20);
    stepLayout->addWidget(singleStepDebounced);
    stepGroup->setLayout(stepLayout);
    layout->addWidget(stepGroup, 1, 2);
}


//
// -- Trigger all the proper initial updates
//    --------------------------------------
void ClockModule_t::TriggerFirstUpdate(void)
{
    speedPot->TriggerFirstUpdate();
    nand1->TriggerFirstUpdate();
    and1->TriggerFirstUpdate();
    nor1->TriggerFirstUpdate();
    latch->TriggerFirstUpdate();

    ProcessBreak(HIGH);

    runModeMomentary->pressed();
    runModeMomentary->released();
    stepModeMomentary->pressed();
    stepModeMomentary->released();

    ProcessReset(LOW);
    ProcessReset(HIGH);
}


//
// -- complete the wire-up for this module
//    ------------------------------------
void ClockModule_t::WireUp(void)
{
    // -- increment the clock counter first thing
    connect(timer, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::IncrementClockCount);


    // -- connect up the oscillator inputs
    connect(speedPot, &HW_Pot_t::SignalValueChanged, timer, &HW_Oscillator_t::ProcessRawInterval);


    // -- connect up the inputs to the NAND gates
    connect(and1, &IC_74xx08_t::SignalY4Updated, nand1, &IC_74xx00_t::ProcessUpdateA1);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, nand1, &IC_74xx00_t::ProcessUpdateB1);
    nand1->ProcessA2Low();
    nand1->ProcessB2Low();
    nand1->ProcessA3Low();
    nand1->ProcessB3Low();
    connect(and1, &IC_74xx08_t::SignalY3Updated, nand1, &IC_74xx00_t::ProcessUpdateA4);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, nand1, &IC_74xx00_t::ProcessUpdateB4);


    // -- connect up the inputs to the D-type latch
    connect(timer, &HW_Oscillator_t::SignalStateChanged, latch, &IC_74xx74_t::ProcessUpdateClockLatch1);
    connect(timer, &HW_Oscillator_t::SignalStateChanged, latch, &IC_74xx74_t::ProcessUpdateClockOutput1);
    connect(nand1, &IC_74xx00_t::SignalY4Updated, latch, &IC_74xx74_t::ProcessUpdateD1);
    latch->ProcessUpdateClr1(HIGH);
    latch->ProcessUpdatePre1(HIGH);
    latch->ProcessUpdateClockLatch2(LOW);
    latch->ProcessUpdateClockOutput2(LOW);
    latch->ProcessUpdateClr2(HIGH);
    latch->ProcessUpdateD2(LOW);
    latch->ProcessUpdatePre2(HIGH);


    // -- connect up the inputs to the AND gates
    connect(timer, &HW_Oscillator_t::SignalStateChanged, and1, &IC_74xx08_t::ProcessUpdateA1);
    connect(latch, &IC_74xx74_t::SignalQ1Updated, and1, &IC_74xx08_t::ProcessUpdateB1);
    connect(latch, &IC_74xx74_t::SignalQ1bUpdated, and1, &IC_74xx08_t::ProcessUpdateA2);
    connect(singleStepDebounced, &HW_SpdtSwitch_t::SignalState, and1, &IC_74xx08_t::ProcessUpdateB2);
    // ProcessUpdateA3 connected to Reset Slot
    connect(runModeMomentary, &HW_MomentarySwitch_t::SignalState, and1, &IC_74xx08_t::ProcessUpdateB3);
    connect(nor1, &IC_74xx02_t::SignalY4Updated, and1, &IC_74xx08_t::ProcessUpdateA4);
    connect(stepModeMomentary, &HW_MomentarySwitch_t::SignalState, and1, &IC_74xx08_t::ProcessUpdateB4);


    // -- connect up the inputs to the NOR gates
    connect(and1, &IC_74xx08_t::SignalY1Updated, nor1, &IC_74xx02_t::ProcessUpdateA1);
    connect(and1, &IC_74xx08_t::SignalY2Updated, nor1, &IC_74xx02_t::ProcessUpdateB1);

    nor1->ProcessA2Low();
    nor1->ProcessB2Low();
    nor1->ProcessA3Low();
    nor1->ProcessB3Low();


    // -- connect up the oscillator output (LED?)
    connect(timer, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::ProcessUpdateImage);


    // -- connect the clock mode LEDs
    connect(nand1, &IC_74xx00_t::SignalY4Updated, runModeLed, &GUI_Led_t::ProcessStateChange);
    connect(nand1, &IC_74xx00_t::SignalY1Updated, stepModeLed, &GUI_Led_t::ProcessStateChange);



    // -- connect the final clock to its LED
    connect(nor1, &IC_74xx02_t::SignalY1Updated, actualClock, &GUI_Led_t::ProcessStateChange);


    // -- Finally, trigger all external signals
    connect(nor1, &IC_74xx02_t::SignalY1Updated, this, &ClockModule_t::SignalClockStateLatch);
    connect(nor1, &IC_74xx02_t::SignalY1Updated, this, &ClockModule_t::SignalClockStateOutput);

    // -- The last connection to make MUST be the sanity check
    connect(timer, &HW_Oscillator_t::SignalStateChanged, this, &ClockModule_t::SignalSanityCheck);
}


//
// -- These represent inputs from off the module and where those signals are wired
//    ----------------------------------------------------------------------------
void ClockModule_t::ProcessReset(TriState_t state) { and1->ProcessUpdateA3(state); }
void ClockModule_t::ProcessBreak(TriState_t state) {
    nor1->ProcessUpdateA4(state);
    nor1->ProcessUpdateB4(state);
}



void ClockModule_t::StartClock(void) { timer->StartTimer(); }