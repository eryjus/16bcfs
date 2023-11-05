//===================================================================================================================
//  main.cc -- this is the main entry point for the emulator
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"


//
// -- The main entry point for the application
//    ----------------------------------------
int main(int argc, char *argv[])
{
    GUI_Application_t *app = new GUI_Application_t(argc, argv);
    GUI_EmulationWindow_t *emu = new GUI_EmulationWindow_t;

    emu->Initialize();
    emu->GetComputer()->GetOscillator()->StartTimer();

    return app->exec();
}


