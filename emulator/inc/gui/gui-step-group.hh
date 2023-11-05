//===================================================================================================================
//  gui-step-group.hh -- This group box visualizes a momentary switch for single stepping
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
// -- This is a momentary switch for single steps
//    -------------------------------------------
class GUI_StepGroup_t : public QGroupBox {
    Q_OBJECT


private:
    QPushButton *step;

public:
    GUI_StepGroup_t(HW_Clock4Phase_t *clock, QWidget *parent = nullptr);
    virtual ~GUI_StepGroup_t() {}


public slots:
    void PressedStep(void) { emit StepButtonChanged(HIGH); }
    void ReleasedStep(void) { emit StepButtonChanged(LOW); }


signals:
    void StepButtonChanged(TriState_t state);
};


