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


#pragma once


class GUI_Application_t : public QApplication {
    Q_OBJECT;


private:
    QString pgmRomFolder;


public:
    explicit GUI_Application_t(int &argc, char **argv);
    virtual ~GUI_Application_t() {}


public:
    const QString &GetPgmRomFolder(void) const { return pgmRomFolder; }
};

