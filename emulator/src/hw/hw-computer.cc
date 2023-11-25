//===================================================================================================================
//  hw-computer.cc -- This is the foundation of the computer build -- like a backplane or breadboard
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-computer.moc.cc"


//
// -- Static class members
//    --------------------
HW_Computer_t *HW_Computer_t::singleton = nullptr;
ClockModule_t *HW_Computer_t::clock = nullptr;
HW_Bus_t *HW_Computer_t::aluA = nullptr;
HW_Bus_t *HW_Computer_t::aluB = nullptr;
HW_Alu_t *HW_Computer_t::alu = nullptr;
HW_Bus_t *HW_Computer_t::mainBus = nullptr;
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;



//
// -- Get the singleton instance, initializing it as needed
//    -----------------------------------------------------
HW_Computer_t *HW_Computer_t::Get(void)
{
    if (unlikely(singleton == nullptr)) {
        singleton = new HW_Computer_t;
        Initialize();
    }

    return singleton;
}



//
// -- Initialize the main application window for all the widgets
//    ----------------------------------------------------------
void HW_Computer_t::InitGui(void)
{
    QGridLayout *grid;
    HW_MomentarySwitch_t *brk;
    HW_MomentarySwitch_t *rst;

    grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);

    grid->addWidget((brk = new HW_MomentarySwitch_t("Break", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED)), 16, 39);
    grid->addWidget((rst = new HW_MomentarySwitch_t("Reset", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED)), 16, 38);
    grid->addWidget(new GUI_BusLeds_t("ALU A", aluA), 17, 29, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", aluB), 17, 33, 1, 3);
    grid->addWidget(new GUI_BusTester_t("ALU A Input", aluADriver), 18, 29, 1, 4);
    grid->addWidget(new GUI_BusTester_t("ALU B Input", aluBDriver), 18, 33, 1, 4);
    grid->addWidget(new GUI_BusLeds_t("Main Bus", mainBus), 16, 31, 1, 3);


    clock = new ClockModule_t;
    grid->addWidget(clock, 17, 37, 2, 3);


    singleton->setLayout(grid);

    singleton->setWindowTitle(tr("16bcfs Emulator"));
    singleton->showMaximized();

    connect(brk, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessBreak);
    connect(rst, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessReset);
}


//
// -- Initialize the computer by placing the components and hooking up the wires (signals/slots)
//    ------------------------------------------------------------------------------------------
void HW_Computer_t::Initialize(void)
{
    //
    // -- Place the oscillator
    //    --------------------
    HW_Oscillator_t *oscillator = new HW_Oscillator_t;


    //
    // -- Create ALUA and ALUB
    //    --------------------
    aluA = new HW_Bus_t(oscillator);
    aluB = new HW_Bus_t(oscillator);
    aluADriver = new HW_BusDriver_t(aluA);
    aluBDriver = new HW_BusDriver_t(aluB);


    //
    // -- Create the Main Bus
    //    -------------------
    mainBus = new HW_Bus_t(oscillator);


    //
    // -- Create the ALU
    //    --------------
    alu = new HW_Alu_t(aluA, aluB, mainBus);


    connect(oscillator, &HW_Oscillator_t::SignalStateChanged, singleton, &HW_Computer_t::SignalOscillatorStateChanged);


    //
    // -- Finally, build the GUI screen
    //    -----------------------------
    InitGui();


    //
    // -- and start the timer oscillating
    //    -------------------------------
    oscillator->StartTimer();
}


