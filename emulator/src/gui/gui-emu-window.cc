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
// -- Initialize the main application window for all the widgets
//    ----------------------------------------------------------
void GUI_EmulationWindow_t::Initialize(void)
{
    computer = new HW_Computer_t();
    computer->Initialize();


    grid = new QGridLayout;
    grid->setContentsMargins(0, 0, 0, 0);

    GUI_ClockFunctionGroup_t *g;
    GUI_OscillatorGroup_t *o;

    grid->addWidget((o = new GUI_OscillatorGroup_t(computer->GetOscillator())), 17, 38);
    grid->addWidget(new GUI_Clock4PhaseGroup_t(computer->Get4PhaseClock()), 17, 37);
    grid->addWidget((g = new GUI_ClockFunctionGroup_t(computer->GetTriStateLatch(), computer->Get4PhaseClock())), 18, 37, 1, 2);
    grid->addWidget(new GUI_StepGroup_t(computer->Get4PhaseClock()), 18, 39);
    grid->addWidget(new GUI_ClockSpeed_t(computer->GetOscillator()), 17, 39);

    setLayout(grid);

    setWindowTitle(tr("16bcfs Emulator"));
    showMaximized();
}



