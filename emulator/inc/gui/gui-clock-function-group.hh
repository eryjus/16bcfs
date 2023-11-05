//===================================================================================================================
//  gui-clock-function-group.hh -- This is a group box for selecting which clock function
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
// -- This is a group box where the clock function is selected
//    --------------------------------------------------------
class GUI_ClockFunctionGroup_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_Led_t *tristateLedRun;
    GUI_Led_t *tristateLedInstr;
    GUI_Led_t *tristateLedCycle;

    QPushButton *tristateRunButton;
    QPushButton *tristateInstrButton;
    QPushButton *tristateCycleButton;


public:
    GUI_ClockFunctionGroup_t(HW_TriStateLatch_t *latch, HW_Clock4Phase_t *clock, QWidget *parent = nullptr);
    virtual ~GUI_ClockFunctionGroup_t() {}

public slots:
    void PressedRun(void) { emit RunButtonChanged(LOW); }
    void ReleasedRun(void) { emit RunButtonChanged(HIGH); }
    void PressedInstr(void) { emit InstrButtonChanged(LOW); }
    void ReleasedInstr(void) { emit InstrButtonChanged(HIGH); }
    void PressedCycle(void) { emit CycleButtonChanged(LOW); }
    void ReleasedCycle(void) { emit CycleButtonChanged(HIGH); }


signals:
    void RunButtonChanged(TriState_t state);
    void InstrButtonChanged(TriState_t state);
    void CycleButtonChanged(TriState_t state);
};
