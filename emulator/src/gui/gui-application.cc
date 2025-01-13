//===================================================================================================================
//  gui-application.hh -- The main GUI application class
//
//      Copyright (c) 2023-2025 - Adam Clark
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
    QSettings *settings = HW_Computer_t::GetSettings();

    if (argc == 2) {
        HW_Computer_t::SetPgmRomFolder(QString(argv[1]));
    } else {
        HW_Computer_t::SetPgmRomFolder(settings->value(lastPgm).toString());
    }

    QString fldr = HW_Computer_t::GetPgmRomFolder();

    settings->setValue(lastPgm, fldr);
    settings->sync();

    app = this;
    HW_Computer_t::Get();
}
