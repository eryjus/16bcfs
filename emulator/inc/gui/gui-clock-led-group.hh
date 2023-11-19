//===================================================================================================================
//  gui-clock-led-group.hh -- This is a group box for visualizing the clock cycle
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a group box where the clock mode is selected
//    ----------------------------------------------------
class GUI_ClockLedGroup_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_Led_t *clockLed;


public:
    explicit GUI_ClockLedGroup_t(HW_Clock_t *clk);
    virtual ~GUI_ClockLedGroup_t() {}
};
