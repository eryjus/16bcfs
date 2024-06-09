//===================================================================================================================
//  hw-computer.cc -- This is the foundation of the computer build -- like a backplane or breadboard
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-computer.moc.cc"


//
// -- The settings for the application
//    --------------------------------
QSettings *HW_Computer_t::settings;


//
// -- Static class members -- modules
//    -------------------------------
HW_Computer_t *HW_Computer_t::singleton = nullptr;
ControlLogic_MidPlane_t *HW_Computer_t::ctrlLogic = nullptr;
ClockModule_t *HW_Computer_t::clock = nullptr;
AluFlagsModule_t *HW_Computer_t::pgmFlags = nullptr;
AluFlagsModule_t *HW_Computer_t::intFlags = nullptr;


// -- Buses
HW_Bus_1_t *HW_Computer_t::rHld = nullptr;
HW_Bus_16_t *HW_Computer_t::mainBus = nullptr;
HW_Bus_16_t *HW_Computer_t::aluA = nullptr;
HW_Bus_16_t *HW_Computer_t::aluB = nullptr;
HW_Bus_16_t *HW_Computer_t::addr1 = nullptr;
HW_Bus_16_t *HW_Computer_t::addr2 = nullptr;
HW_Bus_16_t *HW_Computer_t::instrBus = nullptr;
HW_Bus_16_t *HW_Computer_t::AddrCopyBus = nullptr;
HW_Bus_16_t *HW_Computer_t::fetchBus = nullptr;


// -- The ALU
HW_Alu_t *HW_Computer_t::alu = nullptr;


// -- Registers
GpRegisterModule_t *HW_Computer_t::r1 = nullptr;
GpRegisterModule_t *HW_Computer_t::r2 = nullptr;
GpRegisterModule_t *HW_Computer_t::r3 = nullptr;
GpRegisterModule_t *HW_Computer_t::r4 = nullptr;
GpRegisterModule_t *HW_Computer_t::r5 = nullptr;
GpRegisterModule_t *HW_Computer_t::r6 = nullptr;
GpRegisterModule_t *HW_Computer_t::r7 = nullptr;
GpRegisterModule_t *HW_Computer_t::r8 = nullptr;
GpRegisterModule_t *HW_Computer_t::r9 = nullptr;
GpRegisterModule_t *HW_Computer_t::r10 = nullptr;
GpRegisterModule_t *HW_Computer_t::r11 = nullptr;
GpRegisterModule_t *HW_Computer_t::r12 = nullptr;
GpRegisterModule_t *HW_Computer_t::pgmpc = nullptr;
GpRegisterModule_t *HW_Computer_t::pgmra = nullptr;
GpRegisterModule_t *HW_Computer_t::pgmsp = nullptr;
GpRegisterModule_t *HW_Computer_t::intpc = nullptr;
GpRegisterModule_t *HW_Computer_t::intra = nullptr;
GpRegisterModule_t *HW_Computer_t::intsp = nullptr;
FetchRegisterModule_t *HW_Computer_t::fetch = nullptr;



// -- Widgets
QWidget *HW_Computer_t::central = nullptr;



// -- Testing
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::brk = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::rst = nullptr;



//
// -- Construct a Computer -- this is the backplane
//    ---------------------------------------------
void HW_Computer_t::Initialize(void)
{
    settings = new QSettings("eryjus", "16bcfs-emulator");

    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdates();
}



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
// -- Some trivial implementations
//    ----------------------------
inline void HW_Computer_t::ProcessUpdateZLatch(int state) { pgmFlags->ProcessZLatch(state==Qt::Unchecked?LOW:HIGH); }
inline void HW_Computer_t::ProcessUpdateCLatch(int state) { pgmFlags->ProcessCLatch(state==Qt::Unchecked?LOW:HIGH); }
inline void HW_Computer_t::ProcessUpdateNVLLatch(int state) { pgmFlags->ProcessNVLLatch(state==Qt::Unchecked?LOW:HIGH); }



//
// -- Initialize the main application window for all the widgets
//    ----------------------------------------------------------
void HW_Computer_t::BuildGui(void)
{
    QGridLayout *grid;

    grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);

    grid->addWidget((brk = new HW_MomentarySwitch_t("Break", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 13, 14);
    grid->addWidget((rst = new HW_MomentarySwitch_t("Reset", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED)), 14, 14);

    grid->addWidget(new GUI_BusLeds_t("Addr1", addr1), 12, 0, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr2", addr2), 12, 3, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Main", mainBus), 12, 6, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU A", aluA), 12, 9, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", aluB), 12, 12, 1, 3);

    grid->addWidget(pgmFlags, 0, 11, 1, 1);
    grid->addWidget(intFlags, 0, 12, 1, 1);

    // -- place the control logic mid-plane
    grid->addWidget(ctrlLogic, 0, 15, 9, 2);
    grid->addWidget(fetch, 11, 15, 2, 2);

    grid->addWidget(clock, 13, 15, 2, 2);

    grid->addWidget(pgmpc, 0, 0, 2, 3);
    grid->addWidget(pgmra, 2, 0, 2, 3);
    grid->addWidget(pgmsp, 4, 0, 2, 3);
    grid->addWidget(intpc, 6, 0, 2, 3);
    grid->addWidget(intra, 8, 0, 2, 3);
    grid->addWidget(intsp, 10, 0, 2, 3);
    grid->addWidget(r1, 0, 3, 2, 3);
    grid->addWidget(r2, 2, 3, 2, 3);
    grid->addWidget(r3, 4, 3, 2, 3);
    grid->addWidget(r4, 6, 3, 2, 3);
    grid->addWidget(r5, 8, 3, 2, 3);
    grid->addWidget(r6, 10, 3, 2, 3);
    grid->addWidget(r7, 0, 6, 2, 3);
    grid->addWidget(r8, 2, 6, 2, 3);
    grid->addWidget(r9, 4, 6, 2, 3);
    grid->addWidget(r10, 6, 6, 2, 3);
    grid->addWidget(r11, 8, 6, 2, 3);
    grid->addWidget(r12, 10, 6, 2, 3);


    central = new QWidget;
    central->setLayout(grid);
    singleton->setCentralWidget(central);

    singleton->statusBar()->showMessage("Hi!", 3000);

    QMenu *fileMenu = singleton->menuBar()->addMenu("File");
    QAction *quitAction = new QAction("Quit");
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip("Quit the emulator");
    connect(quitAction, &QAction::triggered, app, &QApplication::quit);
    fileMenu->addAction(quitAction);

    QMenu *editMenu = singleton->menuBar()->addMenu("Edit");
    QAction *settings = new QAction("Settings");
    settings->setStatusTip("Edit the emulator settings");
    connect(settings, &QAction::triggered, singleton, &HW_Computer_t::ProcessSettingsWindow);
    editMenu->addAction(settings);

    singleton->setWindowTitle(tr("16bcfs Emulator"));
    singleton->show();

    connect(brk, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessBreak);
    connect(rst, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessReset);
    connect(rst, &HW_MomentarySwitch_t::SignalState, pgmpc, &GpRegisterModule_t::ProcessReset);
    connect(rst, &HW_MomentarySwitch_t::SignalState, ctrlLogic, &ControlLogic_MidPlane_t::ProcessReset);
}


//
// -- Initialize the computer by placing the components and hooking up the wires (signals/slots)
//    ------------------------------------------------------------------------------------------
void HW_Computer_t::AllocateComponents(void)
{
    //
    // -- Place the oscillator first -- needed for buses
    //    ----------------------------------------------
    clock = new ClockModule_t;


    //
    // -- Create the various buses -- top priority to place
    //    -------------------------------------------------
    rHld = new HW_Bus_1_t("Reset", clock);
    mainBus = new HW_Bus_16_t("Main", clock);
    aluA = new HW_Bus_16_t("ALU A", clock);
    aluB = new HW_Bus_16_t("ALU B", clock);
    addr1 = new HW_Bus_16_t("Addr 1", clock);
    addr2 = new HW_Bus_16_t("Addr 2", clock);
    instrBus = new HW_Bus_16_t("Instruction", clock);
    AddrCopyBus = new HW_Bus_16_t("ROM Copy", clock);
    fetchBus = new HW_Bus_16_t("Fetch", clock);


    //
    // -- Construct the Control Logic Mid-Plane
    //    -------------------------------------
    ctrlLogic = new ControlLogic_MidPlane_t;


    //
    // -- Create the testing drivers
    //    --------------------------
    aluADriver = new HW_BusDriver_t(aluA);
    aluBDriver = new HW_BusDriver_t(aluB);


    //
    // -- Create the ALU
    //    --------------
    alu = new HW_Alu_t(aluA, aluB, mainBus);


    //
    // -- Create the 2 flags results
    //    --------------------------
    pgmFlags = new AluFlagsModule_t("Pgm Flags");
    intFlags = new AluFlagsModule_t("Int Flags", HIGH);


    //
    // -- Create the Registers
    //    --------------------
    r1 = new GpRegisterModule_t("R1");
    r2 = new GpRegisterModule_t("R2");
    r3 = new GpRegisterModule_t("R3");
    r4 = new GpRegisterModule_t("R4");
    r5 = new GpRegisterModule_t("R5");
    r6 = new GpRegisterModule_t("R6");
    r7 = new GpRegisterModule_t("R7");
    r8 = new GpRegisterModule_t("R8");
    r9 = new GpRegisterModule_t("R9");
    r10 = new GpRegisterModule_t("R10");
    r11 = new GpRegisterModule_t("R11");
    r12 = new GpRegisterModule_t("R12");
    pgmpc = new GpRegisterModule_t("PGM PC");
    pgmra = new GpRegisterModule_t("PGM RA");
    pgmsp = new GpRegisterModule_t("PGM SP");
    intpc = new GpRegisterModule_t("INT PC");
    intra = new GpRegisterModule_t("INT RA");
    intsp = new GpRegisterModule_t("INT SP");
    fetch = new FetchRegisterModule_t;
}



//
// -- Wire-Up all the signals
//    -----------------------
void HW_Computer_t::WireUp(void)
{
    // -- connect up the clock
    connect(clock, &ClockModule_t::SignalClockState, pgmFlags, &AluFlagsModule_t::ProcessClk);

    connect(clock, &ClockModule_t::SignalClockState, singleton, &HW_Computer_t::SignalOscillatorStateChanged);

    HW_Bus_1_t *rHld = HW_Computer_t::GetRhldBus();
    connect(rHld, &HW_Bus_1_t::SignalBit0Updated, ctrlLogic, &ControlLogic_MidPlane_t::ProcessSanityCheck);


    //
    // -- Clock for the register module
    //    -----------------------------
    connect(clock, &ClockModule_t::SignalClockState, pgmpc, &GpRegisterModule_t::ProcessClk);



    //
    // -- Wire up the relevant pieces of the Control Logic Mid-Plane
    //    ----------------------------------------------------------
    connect(clock, &ClockModule_t::SignalClockState, ctrlLogic, &ControlLogic_MidPlane_t::ProcessCpuClock);

    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalAddrBus1AssertPgmPC, pgmpc, &GpRegisterModule_t::ProcessAssertAddr1);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalPgmPCInc, pgmpc, &GpRegisterModule_t::ProcessInc);
}


//
// -- Trigger first updates to put everything in sync
//    -----------------------------------------------
void HW_Computer_t::TriggerFirstUpdates(void)
{
    brk->TriggerFirstUpdates();
    rst->TriggerFirstUpdates();         // must be last to reset everything
}



//
// -- execute the settings dialog
//    ---------------------------
void HW_Computer_t::ProcessSettingsWindow(void)
{
    GUI_SettingsDialog_t *settings = new GUI_SettingsDialog_t(this);
    settings->exec();
    delete settings;
}



//
// -- Perform the steps needed to execute a proper reset
//    --------------------------------------------------
void HW_Computer_t::PerformReset(void)
{
    rst->Press();

    pgmFlags->TriggerFirstUpdate();
    intFlags->TriggerFirstUpdate();
    alu->TriggerFirstUpdate();

    r1->TriggerFirstUpdate();
    r2->TriggerFirstUpdate();
    r3->TriggerFirstUpdate();
    r4->TriggerFirstUpdate();
    r5->TriggerFirstUpdate();
    r6->TriggerFirstUpdate();
    r7->TriggerFirstUpdate();
    r8->TriggerFirstUpdate();
    r9->TriggerFirstUpdate();
    r10->TriggerFirstUpdate();
    r11->TriggerFirstUpdate();
    r12->TriggerFirstUpdate();
    pgmpc->TriggerFirstUpdate();
    pgmra->TriggerFirstUpdate();
    pgmsp->TriggerFirstUpdate();
    intpc->TriggerFirstUpdate();
    intra->TriggerFirstUpdate();
    intsp->TriggerFirstUpdate();
    fetch->TriggerFirstUpdate();
    ctrlLogic->TriggerFirstUpdate();

    rst->Release();
}


