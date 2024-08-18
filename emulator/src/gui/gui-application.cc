//===================================================================================================================
//  gui-application.hh -- The main GUI application class
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "gui-application.moc.cc"



//
// -- This is the main constructor for the application
//    ------------------------------------------------
GUI_Application_t::GUI_Application_t(int &argc, char **argv) : QApplication(argc, argv)
{
    app = this;
    HW_Computer_t::Get();

    QSettings *settings = HW_Computer_t::GetSettings();

    if (argc == 2) {
        pgmRomFolder = argv[1];
    } else {
        pgmRomFolder = settings->value(lastPgm).toString();
    }

    settings->setValue(lastPgm, pgmRomFolder);
    settings->sync();
}
