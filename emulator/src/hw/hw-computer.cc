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
// -- Static class members -- modules
//    -------------------------------
HW_Computer_t *HW_Computer_t::singleton = nullptr;
ClockModule_t *HW_Computer_t::clock = nullptr;
AluFlagsModule_t *HW_Computer_t::pgmFlags = nullptr;
AluFlagsModule_t *HW_Computer_t::intFlags = nullptr;


// -- Buses
HW_Bus_t *HW_Computer_t::mainBus = nullptr;
HW_Bus_t *HW_Computer_t::aluA = nullptr;
HW_Bus_t *HW_Computer_t::aluB = nullptr;
HW_Bus_t *HW_Computer_t::addr1 = nullptr;
HW_Bus_t *HW_Computer_t::addr2 = nullptr;
HW_Alu_t *HW_Computer_t::alu = nullptr;


// -- Registers
GpRegisterModule_t *HW_Computer_t::pc = nullptr;


// -- Testing
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::brk = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::rst = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::clc = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::stc = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::clv = nullptr;
HW_MomentarySwitch_t *HW_Computer_t::stv = nullptr;


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


//
// -- Code in this function is used to handle the test harness for the current iteration
//    ----------------------------------------------------------------------------------
QWidget *HW_Computer_t::BuildTestHarness(void)
{
    QGroupBox *test = new QGroupBox("Test Harness");

    QGridLayout *grid = new QGridLayout;

    QButtonGroup *mainGroup = new QButtonGroup;
    QRadioButton *mainNone = new QRadioButton("Main None");
    QRadioButton *mainAlu = new QRadioButton("Main ALU");
    QRadioButton *mainPc = new QRadioButton("Main PC");
    QRadioButton *swapAlu = new QRadioButton("Swap ALU");
    QRadioButton *swapPc = new QRadioButton("Swap PC");
    mainGroup->addButton(mainNone, TEST_MAIN_NONE);
    mainGroup->addButton(mainAlu, TEST_MAIN_ALU);
    mainGroup->addButton(mainPc, TEST_MAIN_PC);
    mainGroup->addButton(swapAlu, TEST_SWAP_ALU);
    mainGroup->addButton(swapPc, TEST_SWAP_PC);

    QButtonGroup *aluAGroup = new QButtonGroup;
    QRadioButton *aluANone = new QRadioButton("ALUA None");
    QRadioButton *aluAPc = new QRadioButton("ALUA PC");
    aluAGroup->addButton(aluANone, TEST_ALUA_NONE);
    aluAGroup->addButton(aluAPc, TEST_ALUA_PC);
    grid->addWidget(aluANone, 2, 0);
    grid->addWidget(aluAPc, 2, 2);

    QButtonGroup *aluBGroup = new QButtonGroup;
    QRadioButton *aluBNone = new QRadioButton("ALUB None");
    QRadioButton *aluBPc = new QRadioButton("ALUB PC");
    aluBGroup->addButton(aluBNone, TEST_ALUB_NONE);
    aluBGroup->addButton(aluBPc, TEST_ALUB_PC);
    grid->addWidget(aluBNone, 3, 0);
    grid->addWidget(aluBPc, 3, 2);

    QButtonGroup *addr1Group = new QButtonGroup;
    QRadioButton *addr1None = new QRadioButton("Addr1 None");
    QRadioButton *addr1Pc = new QRadioButton("Addr1 PC");
    addr1Group->addButton(addr1None, TEST_ADDR1_NONE);
    addr1Group->addButton(addr1Pc, TEST_ADDR1_PC);
    grid->addWidget(addr1None, 4, 0);
    grid->addWidget(addr1Pc, 4, 2);

    QButtonGroup *addr2Group = new QButtonGroup;
    QRadioButton *addr2None = new QRadioButton("Addr2 None");
    QRadioButton *addr2Pc = new QRadioButton("Addr2 PC");
    addr2Group->addButton(addr2None, TEST_ADDR2_NONE);
    addr2Group->addButton(addr2Pc, TEST_ADDR2_PC);
    grid->addWidget(addr2None, 5, 0);
    grid->addWidget(addr2Pc, 5, 2);

    QButtonGroup *pcModeGroup = new QButtonGroup;
    QRadioButton *pcModeNone = new QRadioButton("PC None");
    QRadioButton *pcModeLoad = new QRadioButton("PC Load");
    QRadioButton *pcModeInc = new QRadioButton("PC Inc");
    QRadioButton *pcModeDec = new QRadioButton("PC Dec");
    pcModeGroup->addButton(pcModeNone, TEST_PC_NONE);
    pcModeGroup->addButton(pcModeLoad, TEST_PC_LOAD);
    pcModeGroup->addButton(pcModeInc, TEST_PC_INC);
    pcModeGroup->addButton(pcModeDec, TEST_PC_DEC);
    grid->addWidget(pcModeNone, 6, 0);
    grid->addWidget(pcModeLoad, 6, 1);
    grid->addWidget(pcModeInc, 6, 2);
    grid->addWidget(pcModeDec, 6, 3);

    QCheckBox *z = new QCheckBox("Latch Z");
    grid->addWidget(z, 7, 0);
    QCheckBox *c = new QCheckBox("Latch C");
    grid->addWidget(c, 7, 1);
    QCheckBox *nvl = new QCheckBox("Latch NVL");
    grid->addWidget(nvl, 7, 2);

    connect(mainGroup, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);
    connect(aluAGroup, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);
    connect(aluBGroup, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);
    connect(addr1Group, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);
    connect(addr2Group, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);
    connect(pcModeGroup, &QButtonGroup::idToggled, HW_Computer_t::Get(), &HW_Computer_t::ProcessToggleButton);

    connect(z, &QCheckBox::stateChanged, HW_Computer_t::Get(), &HW_Computer_t::ProcessUpdateZLatch);
    connect(c, &QCheckBox::stateChanged, HW_Computer_t::Get(), &HW_Computer_t::ProcessUpdateCLatch);
    connect(nvl, &QCheckBox::stateChanged, HW_Computer_t::Get(), &HW_Computer_t::ProcessUpdateNVLLatch);


    mainNone->setChecked(false);
    aluANone->setChecked(false);
    aluBNone->setChecked(false);
    addr1None->setChecked(false);
    addr2None->setChecked(false);
    pcModeNone->setChecked(false);

    mainNone->setChecked(true);
    aluANone->setChecked(true);
    aluBNone->setChecked(true);
    addr1None->setChecked(true);
    addr2None->setChecked(true);
    pcModeNone->setChecked(true);

    test->setLayout(grid);

    return test;
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

    grid->addWidget((brk = new HW_MomentarySwitch_t("Break", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 16, 39);
    grid->addWidget((rst = new HW_MomentarySwitch_t("Reset", HW_MomentarySwitch_t::HIGH_WHEN_RELEASED)), 16, 38);
    grid->addWidget((clc = new HW_MomentarySwitch_t("CLC", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 15, 38);
    grid->addWidget((stc = new HW_MomentarySwitch_t("STC", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 15, 39);
    grid->addWidget((clv = new HW_MomentarySwitch_t("CLV", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 14, 38);
    grid->addWidget((stv = new HW_MomentarySwitch_t("STV", HW_MomentarySwitch_t::HIGH_WHEN_PRESSED)), 14, 39);
    grid->addWidget(new GUI_BusLeds_t("Main Bus", mainBus), 13, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU A", aluA), 14, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", aluB), 15, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr1", addr1), 16, 33, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("Addr2", addr2), 17, 33, 1, 3);
    grid->addWidget(new GUI_BusTester_t("ALU A Input", aluADriver), 18, 29, 1, 4);
    grid->addWidget(new GUI_BusTester_t("ALU B Input", aluBDriver), 18, 33, 1, 4);


    grid->addWidget(clock, 17, 37, 2, 3);
    grid->addWidget(pc, 0, 4, 2, 4);
    grid->addWidget(pgmFlags, 0, 29, 1, 2);
    grid->addWidget(intFlags, 0, 31, 1, 2);


    singleton->setLayout(grid);

    singleton->setWindowTitle(tr("16bcfs Emulator"));
    singleton->showMaximized();

    grid->addWidget(BuildTestHarness(), 0, 33, 6, 6);

    connect(brk, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessBreak);
    connect(rst, &HW_MomentarySwitch_t::SignalState, clock, &ClockModule_t::ProcessReset);
    connect(rst, &HW_MomentarySwitch_t::SignalState, pc, &GpRegisterModule_t::ProcessReset);

    connect(clc, &HW_MomentarySwitch_t::SignalState, pgmFlags, &AluFlagsModule_t::ProcessClearCarry);
    connect(stc, &HW_MomentarySwitch_t::SignalState, pgmFlags, &AluFlagsModule_t::ProcessSetCarry);

    connect(clv, &HW_MomentarySwitch_t::SignalState, pgmFlags, &AluFlagsModule_t::ProcessClearOverflow);
    connect(stv, &HW_MomentarySwitch_t::SignalState, pgmFlags, &AluFlagsModule_t::ProcessSetOverflow);
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
    mainBus = new HW_Bus_t(clock);
    aluA = new HW_Bus_t(clock);
    aluB = new HW_Bus_t(clock);
    addr1 = new HW_Bus_t(clock);
    addr2 = new HW_Bus_t(clock);


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
    // -- Finally, build the GUI screen
    //    -----------------------------
    InitGui();


    // -- connect up the clock
    connect(clock, &ClockModule_t::SignalClockState, pc, &GpRegisterModule_t::ProcessClk);
    connect(clock, &ClockModule_t::SignalClockState, pgmFlags, &AluFlagsModule_t::ProcessClk);

    connect(clock, &ClockModule_t::SignalClockState, singleton, &HW_Computer_t::SignalOscillatorStateChanged);


    stv->TriggerFirstUpdates();
    clv->TriggerFirstUpdates();
    stc->TriggerFirstUpdates();
    clc->TriggerFirstUpdates();
    brk->TriggerFirstUpdates();
    rst->TriggerFirstUpdates();         // must be last to reset everything
}


