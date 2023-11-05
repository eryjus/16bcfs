//===================================================================================================================
//  gui-clock4phase-group.hh -- This is a group box visualizing the 4-phase clock
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a 4-cycle clock visualization
//    -------------------------------------
class GUI_Clock4PhaseGroup_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_Led_t *led1;
    GUI_Led_t *led2;
    GUI_Led_t *led3;
    GUI_Led_t *led4;

    GUI_Led_t *phaseLed1;
    GUI_Led_t *phaseLed2;


public:
    GUI_Clock4PhaseGroup_t(HW_Clock4Phase_t *clock, QWidget *parent = nullptr);
    virtual ~GUI_Clock4PhaseGroup_t() {}
};
