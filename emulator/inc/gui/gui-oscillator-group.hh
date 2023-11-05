//===================================================================================================================
//  gui-oscillator-group.hh -- This is a group box visualizing an oscillator
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
// -- This is an oscillator visualization
//    -----------------------------------
class GUI_OscillatorGroup_t : public QGroupBox {
    Q_OBJECT


private:
    QPixmap hi;
    QPixmap lo;
    QLabel *oscVisual;


public:
    GUI_OscillatorGroup_t(HW_Oscillator_t *hardware, QWidget *parent = nullptr);
    virtual ~GUI_OscillatorGroup_t() {}


public slots:
    void UpdateImage(TriState_t state)  { oscVisual->setPixmap(state?hi:lo); }
};

