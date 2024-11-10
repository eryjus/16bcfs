//===================================================================================================================
//  main.cc -- this is the main entry point for the emulator
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"



//
// -- I cannot see a way around this, app needs to be a global variable
//    -----------------------------------------------------------------
GUI_Application_t *app = nullptr;


//
// -- get the current clock count
//    ---------------------------
unsigned long Count(void)
{
    return HW_Computer_t::GetClock()->GetClockCount();
}


//
// -- The main entry point for the application
//    ----------------------------------------
int main(int argc, char *argv[])
{
    debug = true;
    new GUI_Application_t(argc, argv);

    // -- settings will use these if not provided explicitly; they will be relied upon
    app->setOrganizationName("eryjus");
    app->setApplicationName("16bcfs-emulator");
    HW_Computer_t::Get()->PerformReset();

    return app->exec();
}


