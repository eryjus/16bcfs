//===================================================================================================================
//  gui-emu-window.cc -- The main Emulation Window -- the graphical application
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "gui-emu-window.moc.cc"


//
// -- static class members
//    --------------------
GUI_EmulationWindow_t *GUI_EmulationWindow_t::singleton = nullptr;
HW_Computer_t *GUI_EmulationWindow_t::computer = nullptr;


//
// -- construct a singleton instance of a GUI_EmulationWindow_t
//    ---------------------------------------------------------
GUI_EmulationWindow_t::GUI_EmulationWindow_t(void) : QWidget(nullptr)
{
    singleton = this;
    Initialize();
}


//
// -- Initialize the main application window for all the widgets
//    ----------------------------------------------------------
void GUI_EmulationWindow_t::Initialize(void)
{
    QGridLayout *grid;

    computer = HW_Computer_t::Get();
    computer->Initialize();

    grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);

    grid->addWidget(new GUI_OscillatorGroup_t(computer->GetOscillator()), 17, 38);
    grid->addWidget(new GUI_ClockLedGroup_t(computer->GetClock()), 17, 37);
    grid->addWidget(new GUI_ClockModeGroup_t(computer->GetClock()), 18, 37, 1, 2);
    grid->addWidget(new GUI_StepGroup_t(computer->GetClock()), 18, 39);
    grid->addWidget(new GUI_ClockSpeed_t(computer->GetOscillator()), 17, 39);
    grid->addWidget(new GUI_BusLeds_t("ALU A", computer->GetAluA()), 17, 29, 1, 3);
    grid->addWidget(new GUI_BusLeds_t("ALU B", computer->GetAluB()), 17, 33, 1, 3);
    grid->addWidget(new GUI_BusTester_t("ALU A Input", computer->GetAluADriver()), 18, 29, 1, 4);
    grid->addWidget(new GUI_BusTester_t("ALU B Input", computer->GetAluBDriver()), 18, 33, 1, 4);

    grid->addWidget(new GUI_BusLeds_t("Main Bus", computer->GetMainBus()), 16, 31, 1, 3);


    singleton->setLayout(grid);

    singleton->setWindowTitle(tr("16bcfs Emulator"));
    singleton->showMaximized();
}



