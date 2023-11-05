//===================================================================================================================
//  gui-clock-speed.hh -- This is a group box visualizing a control for the clock speed
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-01  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a clock speed slider
//    --------------------------
class GUI_ClockSpeed_t : public QGroupBox {
    Q_OBJECT


private:
    QSlider *slider;
    QLabel *label;
    HW_Oscillator_t *oscillator;


public:
    GUI_ClockSpeed_t(HW_Oscillator_t *osc, QGroupBox *parent = nullptr);
    virtual ~GUI_ClockSpeed_t() {}


public slots:
    void UpdateSpeed(int value);


signals:
    void SpeedChanged(int value);

};

