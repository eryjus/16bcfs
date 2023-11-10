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
    void ProcessPressedRun(void) { emit SignalRunButtonChanged(LOW); }
    void ProcessReleasedRun(void) { emit SignalRunButtonChanged(HIGH); }
    void ProcessPressedInstr(void) { emit SignalInstrButtonChanged(LOW); }
    void ProcessReleasedInstr(void) { emit SignalInstrButtonChanged(HIGH); }
    void ProcessPressedCycle(void) { emit SignalCycleButtonChanged(LOW); }
    void ProcessReleasedCycle(void) { emit SignalCycleButtonChanged(HIGH); }


signals:
    void SignalRunButtonChanged(TriState_t state);
    void SignalInstrButtonChanged(TriState_t state);
    void SignalCycleButtonChanged(TriState_t state);
};
