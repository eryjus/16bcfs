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
HW_Bus_16_t *HW_Computer_t::ctrlBus = nullptr;
HW_Bus_16_t *HW_Computer_t::AddrCopyBus = nullptr;


// -- The ALU
HW_Alu_t *HW_Computer_t::alu = nullptr;


// -- Registers
GpRegisterModule_t *HW_Computer_t::pc = nullptr;


// -- Control ROMs
CtrlRomCtrlModule_t *HW_Computer_t::ctrlCtrl = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl0 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl1 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl2 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl3 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl4 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl5 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl6 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl7 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl8 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrl9 = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrla = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrlb = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrlc = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrld = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrle = nullptr;
CtrlRomModule_t *HW_Computer_t::ctrlf = nullptr;



// -- Widgets
QWidget *HW_Computer_t::central = nullptr;



// -- Testing
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::brk = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::rst = nullptr;


enum {
    TEST_MAIN_NONE,
    TEST_MAIN_ALU,
    TEST_MAIN_PC,
    TEST_SWAP_ALU,
    TEST_SWAP_PC,
    TEST_ALUA_NONE,
    TEST_ALUA_TEST,
    TEST_ALUA_PC,
    TEST_ALUB_NONE,
    TEST_ALUB_TEST,
    TEST_ALUB_PC,
    TEST_ADDR1_NONE,
    TEST_ADDR1_PC,
    TEST_ADDR2_NONE,
    TEST_ADDR2_PC,
    TEST_PC_NONE,
    TEST_PC_LOAD,
    TEST_PC_INC,
    TEST_PC_DEC,
};



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


void HW_Computer_t::ProcessUpdateZLatch(int state) { pgmFlags->ProcessZLatch(state==Qt::Unchecked?LOW:HIGH); }
void HW_Computer_t::ProcessUpdateCLatch(int state) { pgmFlags->ProcessCLatch(state==Qt::Unchecked?LOW:HIGH); }
void HW_Computer_t::ProcessUpdateNVLLatch(int state) { pgmFlags->ProcessNVLLatch(state==Qt::Unchecked?LOW:HIGH); }


//
// -- Handle when a radio button is toggled
//    -------------------------------------
void HW_Computer_t::ProcessToggleButton(int id, bool checked)
{
    switch (id) {
    case TEST_MAIN_NONE: break;
    case TEST_ALUA_NONE: break;
    case TEST_ALUA_PC: {
        pc->ProcessAssertAluA(checked?HIGH:LOW);
        break;
    }
    case TEST_ALUB_NONE: break;
    case TEST_ALUB_PC: {
        pc->ProcessAssertAluB(checked?HIGH:LOW);
        break;
    }
    case TEST_ADDR1_NONE: break;
    case TEST_ADDR1_PC: {
        pc->ProcessAssertAddr1(checked?HIGH:LOW);
        break;
    }
    case TEST_ADDR2_NONE: break;
    case TEST_ADDR2_PC: {
        pc->ProcessAssertAddr2(checked?HIGH:LOW);
        break;
    }
    case TEST_PC_NONE: break;
    case TEST_PC_LOAD: {
        pc->ProcessLoad(checked?HIGH:LOW);
        break;
    }
    case TEST_PC_INC: {
        pc->ProcessInc(checked?HIGH:LOW);
        break;
    }
    case TEST_PC_DEC: {
        pc->ProcessDec(checked?HIGH:LOW);
        break;
    }
    }
}


//
// -- Initialize the main application window for all the widgets
//    ----------------------------------------------------------
void HW_Computer_t::InitGui(void)
{
    QGridLayout *grid;

    grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);

    grid->addWidget((brk = new HW_MomentarySwitch_t("Break", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 12, 39);
    grid->addWidget((rst = new HW_MomentarySwitch_t("Reset", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED)), 12, 38);
    grid->addWidget(new GUI_BusLeds_t("Main Bus", mainBus), 9, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU A", aluA), 10, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", aluB), 11, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr1", addr1), 12, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr2", addr2), 13, 33, 1, 3);
//    grid->addWidget(new GUI_BusTester_t("ALU A Input", aluADriver), 18, 29, 1, 4);
//    grid->addWidget(new GUI_BusTester_t("ALU B Input", aluBDriver), 18, 33, 1, 4);


    grid->addWidget((ctrl0 = new CtrlRomModule_t("Ctrl0", "ctrl0.bin")), 2, 38, 1, 1);
    grid->addWidget((ctrl1 = new CtrlRomModule_t("Ctrl1", "ctrl1.bin")), 3, 38, 1, 1);
    grid->addWidget((ctrl2 = new CtrlRomModule_t("Ctrl2", "ctrl2.bin")), 4, 38, 1, 1);
    grid->addWidget((ctrl3 = new CtrlRomModule_t("Ctrl3", "ctrl3.bin")), 5, 38, 1, 1);
    grid->addWidget((ctrl4 = new CtrlRomModule_t("Ctrl4", "ctrl4.bin")), 6, 38, 1, 1);
    grid->addWidget((ctrl5 = new CtrlRomModule_t("Ctrl5", "ctrl5.bin")), 7, 38, 1, 1);
    grid->addWidget((ctrl6 = new CtrlRomModule_t("Ctrl6", "ctrl6.bin")), 8, 38, 1, 1);
    grid->addWidget((ctrl7 = new CtrlRomModule_t("Ctrl7", "ctrl7.bin")), 9, 38, 1, 1);
    grid->addWidget((ctrl8 = new CtrlRomModule_t("Ctrl8", "ctrl8.bin")), 2, 39, 1, 1);
    grid->addWidget((ctrl9 = new CtrlRomModule_t("Ctrl9", "ctrl9.bin")), 3, 39, 1, 1);
    grid->addWidget((ctrla = new CtrlRomModule_t("Ctrla", "ctrla.bin")), 4, 39, 1, 1);
    grid->addWidget((ctrlb = new CtrlRomModule_t("Ctrlb", "ctrlb.bin")), 5, 39, 1, 1);
    grid->addWidget((ctrlc = new CtrlRomModule_t("Ctrlc", "ctrlc.bin")), 6, 39, 1, 1);
    grid->addWidget((ctrld = new CtrlRomModule_t("Ctrld", "ctrld.bin")), 7, 39, 1, 1);
    grid->addWidget((ctrle = new CtrlRomModule_t("Ctrle", "ctrle.bin")), 8, 39, 1, 1);
    grid->addWidget((ctrlf = new CtrlRomModule_t("Ctrlf", "ctrlf.bin")), 9, 39, 1, 1);
    grid->addWidget(ctrlCtrl, 0, 38, 2, 2);


    grid->addWidget(clock, 13, 38, 2, 2);
    grid->addWidget(pc, 0, 4, 2, 4);
    grid->addWidget(pgmFlags, 0, 29, 1, 2);
    grid->addWidget(intFlags, 0, 31, 1, 2);

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
    singleton->showMaximized();

    connect(brk, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessBreak);
    connect(rst, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessReset);
    connect(rst, &HW_MomentarySwitch_t::SignalState, pc, &GpRegisterModule_t::ProcessReset);
    connect(rst, &HW_MomentarySwitch_t::SignalState, ctrlCtrl, &CtrlRomCtrlModule_t::ProcessResetUpdate);
}


//
// -- Initialize the computer by placing the components and hooking up the wires (signals/slots)
//    ------------------------------------------------------------------------------------------
void HW_Computer_t::Initialize(void)
{
    //
    // -- Place the oscillator
    //    --------------------
    clock = new ClockModule_t;


    //
    // -- Create the various buses
    //    ------------------------
    rHld = new HW_Bus_1_t(clock);
    mainBus = new HW_Bus_16_t(clock);
    aluA = new HW_Bus_16_t(clock);
    aluB = new HW_Bus_16_t(clock);
    addr1 = new HW_Bus_16_t(clock);
    addr2 = new HW_Bus_16_t(clock);
    instrBus = new HW_Bus_16_t(clock);
    ctrlBus = new HW_Bus_16_t(clock);
    AddrCopyBus = new HW_Bus_16_t(clock);


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
    pc = new GpRegisterModule_t("PC");


    //
    // -- Create the Control ROM Control Module
    //    -------------------------------------
    ctrlCtrl = new CtrlRomCtrlModule_t;


    //
    // -- Finally, build the GUI screen
    //    -----------------------------
    InitGui();


    // -- connect up the clock
    connect(clock, &ClockModule_t::SignalClockState, pc, &GpRegisterModule_t::ProcessClk);
    connect(clock, &ClockModule_t::SignalClockState, pgmFlags, &AluFlagsModule_t::ProcessClk);

    connect(clock, &ClockModule_t::SignalClockState, singleton, &HW_Computer_t::SignalOscillatorStateChanged);

    HW_Bus_1_t *rHld = HW_Computer_t::GetRhldBus();
    connect(rHld, &HW_Bus_1_t::SignalBit0Updated, ctrl0, &CtrlRomModule_t::ProcessSanityCheck);


    ctrl0->TriggerFirstUpdate();
    ctrl1->TriggerFirstUpdate();
    ctrl2->TriggerFirstUpdate();
    ctrl3->TriggerFirstUpdate();
    ctrl4->TriggerFirstUpdate();
    ctrl5->TriggerFirstUpdate();
    ctrl6->TriggerFirstUpdate();
    ctrl7->TriggerFirstUpdate();
    ctrl8->TriggerFirstUpdate();
    ctrl9->TriggerFirstUpdate();
    ctrla->TriggerFirstUpdate();
    ctrlb->TriggerFirstUpdate();
    ctrlc->TriggerFirstUpdate();
    ctrld->TriggerFirstUpdate();
    ctrle->TriggerFirstUpdate();
    ctrlf->TriggerFirstUpdate();

//    stv->TriggerFirstUpdates();
//    clv->TriggerFirstUpdates();
//    stc->TriggerFirstUpdates();
//    clc->TriggerFirstUpdates();
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
    pc->TriggerFirstUpdate();
    ctrlCtrl->TriggerFirstUpdate();
    ctrl0->TriggerFirstUpdate();
    ctrl1->TriggerFirstUpdate();
    ctrl2->TriggerFirstUpdate();
    ctrl3->TriggerFirstUpdate();
    ctrl4->TriggerFirstUpdate();
    ctrl5->TriggerFirstUpdate();
    ctrl6->TriggerFirstUpdate();
    ctrl7->TriggerFirstUpdate();
    ctrl8->TriggerFirstUpdate();
    ctrl9->TriggerFirstUpdate();
    ctrla->TriggerFirstUpdate();
    ctrlb->TriggerFirstUpdate();
    ctrlc->TriggerFirstUpdate();
    ctrld->TriggerFirstUpdate();
    ctrle->TriggerFirstUpdate();
    ctrlf->TriggerFirstUpdate();

    rst->Release();
}


