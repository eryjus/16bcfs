//===================================================================================================================
// ic-74xx74.hh -- This class implements a 74xx74 Dual D-Type Latch
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-20  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements an 74xx74 Dual D-Type Latch
//    -------------------------------------------------
class IC_74xx74_t : public QObject {
    Q_OBJECT


public:
    enum {
        CLR1b = 1,
        D1 = 2,
        CLK1 = 3,
        PRE1b = 4,
        Q1 = 5,
        Q1b = 6,
        Q2b = 8,
        Q2 = 9,
        PRE2b = 10,
        CLK2 = 11,
        D2 = 12,
        CLR2b = 13,
    };


private:
    TriState_t pins[PIN_CNT(14)];

public:
    IC_74xx74_t(void);
    virtual ~IC_74xx74_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessUpdateClr1(TriState_t state);
    void ProcessUpdateD1(TriState_t state) { pins[D1] = state; }
    void ProcessUpdateClk1(TriState_t state);
    void ProcessUpdatePre1(TriState_t state);
    void ProcessUpdateClr2(TriState_t state);
    void ProcessUpdateD2(TriState_t state) { pins[D2] = state; }
    void ProcessUpdateClk2(TriState_t state);
    void ProcessUpdatePre2(TriState_t state);


signals:
    void SignalQ1Updated(TriState_t state);
    void SignalQ1bUpdated(TriState_t state);
    void SignalQ2Updated(TriState_t state);
    void SignalQ2bUpdated(TriState_t state);
};

