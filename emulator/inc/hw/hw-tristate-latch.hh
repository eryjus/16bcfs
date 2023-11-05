//===================================================================================================================
//  hw-tristate-latch.hh -- This class implements a 3-state latch, where 1 of 3 states can be selected
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- This ia a tristate latch
//    ------------------------
class HW_TriStateLatch_t : public QObject {
    Q_OBJECT


private:
    IC_74xx00_t *tristateNand1;         // all 4 gates are used
    IC_74xx00_t *tristateNand2;         // 2 of 4 gates are used
    IC_74xx02_t *tristateNor;           // 3 of 4 gates are used


public:
    HW_TriStateLatch_t(QObject *parent = nullptr);
    virtual ~HW_TriStateLatch_t() {}


public:
    IC_74xx00_t *GetTristateNand1(void) const { return tristateNand1; }
    IC_74xx00_t *GetTristateNand2(void) const { return tristateNand2; }
    IC_74xx02_t *GetTristateNor(void) const { return tristateNor; }


public:
    void TriggerFirstUpdates(void);


public slots:
    void UpdateQ1(TriState_t state) { emit Q1Changed(state); }
    void UpdateQ2(TriState_t state) { emit Q2Changed(state); }
    void UpdateQ3(TriState_t state) { emit Q3Changed(state); }


signals:
    void Q1Changed(TriState_t state);
    void Q2Changed(TriState_t state);
    void Q3Changed(TriState_t state);
};


