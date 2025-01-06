//===================================================================================================================
//  hw-computer.cc -- This is the foundation of the computer build -- like a backplane or breadboard
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-computer.moc.cc"



bool debug = false;


//
// -- Program ROM Folder location
//    ---------------------------
QString HW_Computer_t::pgmRomFolder;


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
PgmRomModule_t *HW_Computer_t::pgmRom = nullptr;
ResetModule_t *HW_Computer_t::reset = nullptr;


// -- Buses
HW_Bus_1_t *HW_Computer_t::rHld = nullptr;
HW_Bus_1_t *HW_Computer_t::cpyHld = nullptr;
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
InstructionRegisterModule_t *HW_Computer_t::instr = nullptr;



// -- Widgets
QWidget *HW_Computer_t::central = nullptr;



// -- Testing
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::brk = nullptr;



//
// -- Construct a Computer -- this is the backplane
//    ---------------------------------------------
void HW_Computer_t::Initialize(void)
{
    if (!settings) settings = new QSettings("eryjus", "16bcfs-emulator");

    AllocateComponents();
    BuildGui();
    WireUp();
    FinalWireUp();
    TriggerFirstUpdate();
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

    grid->addWidget((brk = new HW_MomentarySwitch_t("Break", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 13, 9);

    grid->addWidget(new GUI_BusLeds_t("Addr1", addr1), 12, 0, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr2", addr2), 12, 3, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Main", mainBus), 12, 6, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU A", aluA), 12, 9, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", aluB), 12, 12, 1, 3);

    grid->addWidget(pgmFlags, 0, 11);
    grid->addWidget(intFlags, 0, 12);

    // -- place the control logic mid-plane
    grid->addWidget(ctrlLogic, 0, 15, 8, 2);
    grid->addWidget(instr, 8, 15, 1, 2);
    grid->addWidget(fetch, 9, 15, 2, 2);
    grid->addWidget(clock, 11, 15, 3, 2);

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
    grid->addWidget(pgmRom, 13, 0, 1, 4);
    grid->addWidget(reset, 13, 13, 1, 2);


    central = new QWidget;
    central->setLayout(grid);
    singleton->setCentralWidget(central);

    singleton->statusBar()->showMessage("Hi!", 3000);

    QMenu *fileMenu = singleton->menuBar()->addMenu("File");
    QAction *quitAction = new QAction("Quit");
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip("Quit the emulator");
    connect(quitAction, &QAction::triggered, app, &GUI_Application_t::quit);
    fileMenu->addAction(quitAction);

    QMenu *editMenu = singleton->menuBar()->addMenu("Edit");
    QAction *settings = new QAction("Settings");
    settings->setStatusTip("Edit the emulator settings");
    connect(settings, &QAction::triggered, singleton, &HW_Computer_t::ProcessSettingsWindow);
    editMenu->addAction(settings);

    singleton->setWindowTitle(tr("16bcfs Emulator"));
    singleton->show();

    connect(brk, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessSignalBreak);
    connect(reset, &ResetModule_t::SignalReset, clock, &ClockModule_t::ProcessReset);
//    connect(reset, &ResetModule_t::SignalReset, ctrlLogic, &ControlLogic_MidPlane_t::ProcessReset);
    connect(reset, &ResetModule_t::SignalReset, pgmRom, &PgmRomModule_t::ProcessReset);
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
    rHld = new HW_Bus_1_t("Reset Hold", clock);
    cpyHld = new HW_Bus_1_t("Copy Hold", clock);
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


    //
    // -- The sequence of the allocations here are critical.  Since the signals and slots are processed in order
    //    of their "connection", we need the instruction register's connections to exist before the fetch register's
    //    connections so that the timing and sequencing are correct.  This is just a problem with this emulator's
    //    choice of framework.
    //    ----------------------------------------------------------------------------------------------------------
    instr = new InstructionRegisterModule_t;
    ctrlLogic = new ControlLogic_MidPlane_t;
    fetch = new FetchRegisterModule_t;

    pgmRom = new PgmRomModule_t(GetPgmRomFolder());

    reset = new ResetModule_t;
}



//
// -- Wire-Up all the signals
//    -----------------------
void HW_Computer_t::WireUp(void)
{
    // -- connect up the clock
    connect(clock, &ClockModule_t::SignalCpuClockLatch, pgmFlags, &AluFlagsModule_t::ProcessClockLatch);
    connect(clock, &ClockModule_t::SignalCpuClockOutput, pgmFlags, &AluFlagsModule_t::ProcessClockOutput);

    connect(clock, &ClockModule_t::SignalCpuClockOutput, singleton, &HW_Computer_t::SignalOscillatorStateChanged);

    HW_Bus_1_t *cpyHld = HW_Computer_t::GetCpyHldBus();
    connect(cpyHld, &HW_Bus_1_t::SignalBit0Updated, ctrlLogic, &ControlLogic_MidPlane_t::ProcessSanityCheck);
    connect(cpyHld, &HW_Bus_1_t::SignalBit0Updated, clock, &ClockModule_t::ProcessCpyHld);

    connect(clock, &ClockModule_t::SignalHighSpeedClockLatch, ctrlLogic, &ControlLogic_MidPlane_t::ProcessRawSystemClock);
}


//
// -- Trigger first updates to put everything in sync
//    -----------------------------------------------
void HW_Computer_t::TriggerFirstUpdate(void)
{
    brk->TriggerFirstUpdate();
    rHld->TriggerFirstUpdate();
    cpyHld->TriggerFirstUpdate();
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

    reset->TriggerFirstUpdate();

    reset->PowerOnReset();
}


//
// -- Now that all the planes are built, complete the final wire up between them
//    --------------------------------------------------------------------------
void HW_Computer_t::FinalWireUp(void)
{
    // -- Wire up the PC Register
    connect(HW_Computer_t::GetRHldBus(), &HW_Bus_1_t::SignalBit0Updated, pgmpc, &GpRegisterModule_t::ProcessReset);
    connect(clock, &ClockModule_t::SignalCpuClockLatch, pgmpc, &GpRegisterModule_t::ProcessClockLatch);
    connect(clock, &ClockModule_t::SignalCpuClockOutput, pgmpc, &GpRegisterModule_t::ProcessClockOutput);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalPgmPCLoad, pgmpc, &GpRegisterModule_t::ProcessLoad);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalPgmPCInc, pgmpc, &GpRegisterModule_t::ProcessInc);
    pgmpc->ProcessDec(LOW);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapPgmPC, pgmpc, &GpRegisterModule_t::ProcessAssertSwap);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalMainBusAssertPgmPC, pgmpc, &GpRegisterModule_t::ProcessAssertMain);
    pgmpc->ProcessAssertAluA(LOW);
    pgmpc->ProcessAssertAluB(LOW);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalAddrBus1AssertPgmPC, pgmpc, &GpRegisterModule_t::ProcessAssertAddr1);
    pgmpc->ProcessAssertAddr2(LOW);


    // -- Wire up the R1 Register
    connect(HW_Computer_t::GetRHldBus(), &HW_Bus_1_t::SignalBit0Updated, r1, &GpRegisterModule_t::ProcessReset);
    connect(clock, &ClockModule_t::SignalCpuClockLatch, r1, &GpRegisterModule_t::ProcessClockLatch);
    connect(clock, &ClockModule_t::SignalCpuClockOutput, r1, &GpRegisterModule_t::ProcessClockOutput);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalR1Load, r1, &GpRegisterModule_t::ProcessLoad);
    r1->ProcessInc(LOW);
    r1->ProcessDec(LOW);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR1, r1, &GpRegisterModule_t::ProcessAssertSwap);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalMainBusAssertR1, r1, &GpRegisterModule_t::ProcessAssertMain);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalALUBusAAssertR1, r1, &GpRegisterModule_t::ProcessAssertAluA);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalALUBusBAssertR1, r1, &GpRegisterModule_t::ProcessAssertAluB);
    r1->ProcessAssertAddr1(LOW);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR1, r1, &GpRegisterModule_t::ProcessAssertAddr2);


    // -- Wire up the Fetch Register
    connect(HW_Computer_t::GetRHldBus(), &HW_Bus_1_t::SignalBit0Updated, fetch, &FetchRegisterModule_t::ProcessReset);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalInstructionSuppress, fetch, &FetchRegisterModule_t::ProcessInstructionSuppress);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalMainBusAssertFetch, fetch, &FetchRegisterModule_t::ProcessAssertMain);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalALUBusBAssertFetch, fetch, &FetchRegisterModule_t::ProcessAssertAluB);
    connect(ctrlLogic, &ControlLogic_MidPlane_t::SignalAddrBus2AssertFetch, fetch, &FetchRegisterModule_t::ProcessAssertAddr2);


    connect(reset, &ResetModule_t::SignalReset, ctrlLogic, &ControlLogic_MidPlane_t::ProcessReset);
}



