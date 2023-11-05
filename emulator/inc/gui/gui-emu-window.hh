//===================================================================================================================
//  gui-emu-window.hh -- The main Emulation Window -- the graphical application
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is the main emulation window, with all its widgets
//    -------------------------------------------------------
class GUI_EmulationWindow_t : public QWidget {
    Q_OBJECT

private:
    QGridLayout *grid;
    HW_Computer_t *computer;


public:
    explicit GUI_EmulationWindow_t(QWidget *parent = nullptr) : QWidget(parent) {};
    virtual ~GUI_EmulationWindow_t() {}


public:
    HW_Computer_t *GetComputer(void) const { return computer; }


public:
    void Initialize(void);
};

