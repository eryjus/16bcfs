//===================================================================================================================
// ic-74xx164.hh -- This header file defines a 74xx164 shift in parallel output register
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Feb-05  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This class implements a 74xx164 Shift in parallel out Register
//    --------------------------------------------------------------
class IC_74xx164_t : public QObject {
    Q_OBJECT



public:
    enum {A = 1,
        B = 2,
        QA = 3,
        QB = 4,
        QC = 5,
        QD = 6,
        CLK = 8,
        CLRb = 9,
        QE = 10,
        QF = 11,
        QG = 12,
        QH = 13,
    };



private:
    TriState_t pins[PIN_CNT(14)];



public:
    IC_74xx164_t(void);
    virtual ~IC_74xx164_t() {};



public:
    void TriggerFirstUpdate(void);



public slots:
    void ProcessUpdateA(TriState_t state) { pins[A] = state; }
    void ProcessUpdateB(TriState_t state) { pins[B] = state; }
    void ProcessUpdateClr(TriState_t state);
    void ProcessUpdateClk(TriState_t state);



signals:
    void SignalQAUpdated(TriState_t state);
    void SignalQBUpdated(TriState_t state);
    void SignalQCUpdated(TriState_t state);
    void SignalQDUpdated(TriState_t state);
    void SignalQEUpdated(TriState_t state);
    void SignalQFUpdated(TriState_t state);
    void SignalQGUpdated(TriState_t state);
    void SignalQHUpdated(TriState_t state);
};



