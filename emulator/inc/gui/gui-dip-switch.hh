//===================================================================================================================
//  gui-dip-switch.hh -- A slider visualizing a dip switch with 2 possible states
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-07  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- A dip switch is nothing more than a slide with 2 possible states
//    ----------------------------------------------------------------
class GUI_DipSwitch_t : public QSlider {
    Q_OBJECT


public:
    GUI_DipSwitch_t(QWidget *parent = nullptr);
    ~GUI_DipSwitch_t() {}


public slots:
    void ProcessClick(void) { setSliderPosition(value()==1?0:1); }
    void ProcessStateChange(void) { emit SignalSwitchChanged(value()==1?HIGH:LOW); }


signals:
    void SignalSwitchChanged(TriState_t state);
};


