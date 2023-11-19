//===================================================================================================================
//  gui-clock-mode-group.hh -- This is a group box for selecting which clock mode
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
class GUI_ClockModeGroup_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_Led_t *modeLedRun;
    GUI_Led_t *modeLedStep;

    QPushButton *runButton;
    QPushButton *stepButton;


public:
    GUI_ClockModeGroup_t(HW_Clock_t *clock, QWidget *parent = nullptr);
    virtual ~GUI_ClockModeGroup_t() {}

public slots:
    void ProcessPressedRun(void) { emit SignalRunButtonChanged(LOW); }
    void ProcessReleasedRun(void) { emit SignalRunButtonChanged(HIGH); }
    void ProcessPressedStep(void) { emit SignalStepButtonChanged(LOW); }
    void ProcessReleasedStep(void) { emit SignalStepButtonChanged(HIGH); }


signals:
    void SignalRunButtonChanged(TriState_t state);
    void SignalStepButtonChanged(TriState_t state);
};
