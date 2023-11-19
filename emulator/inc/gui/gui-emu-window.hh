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
    static GUI_EmulationWindow_t *singleton;
    static HW_Computer_t *computer;


private:
    explicit GUI_EmulationWindow_t(void);
    virtual ~GUI_EmulationWindow_t() {}

    GUI_EmulationWindow_t(const GUI_EmulationWindow_t &) = delete;
    GUI_EmulationWindow_t &operator=(const GUI_EmulationWindow_t &) = delete;


public:
    static GUI_EmulationWindow_t *Get(void) { if (!singleton) new GUI_EmulationWindow_t; return singleton; }


public:
    static HW_Computer_t *GetComputer(void) { return computer; }


public:
    static void Initialize(void);
};

