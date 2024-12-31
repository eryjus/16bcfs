//===================================================================================================================
// ic-74xx574.hh -- This class implements a 74xx574 Octal D-Type Latch
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-30  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements an 74xx574 Octal D-Type Latch
//    ---------------------------------------------------
class IC_74xx574_t : public QObject {
    Q_OBJECT


public:
    enum {OEb = 1,
        D1 = 2,
        D2 = 3,
        D3 = 4,
        D4 = 5,
        D5 = 6,
        D6 = 7,
        D7 = 8,
        D8 = 9,
        CLK = 11,
        Q8 = 12,
        Q7 = 13,
        Q6 = 14,
        Q5 = 15,
        Q4 = 16,
        Q3 = 17,
        Q2 = 18,
        Q1 = 19,
    };

private:
    TriState_t pins[PIN_CNT(20)];

    TriState_t d1;
    TriState_t d2;
    TriState_t d3;
    TriState_t d4;
    TriState_t d5;
    TriState_t d6;
    TriState_t d7;
    TriState_t d8;


public:
    IC_74xx574_t(void);
    virtual ~IC_74xx574_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessUpdateOE(TriState_t state);
    void ProcessUpdateD1(TriState_t state) { pins[D1] = state; }
    void ProcessUpdateD2(TriState_t state) { pins[D2] = state; }
    void ProcessUpdateD3(TriState_t state) { pins[D3] = state; }
    void ProcessUpdateD4(TriState_t state) { pins[D4] = state; }
    void ProcessUpdateD5(TriState_t state) { pins[D5] = state; }
    void ProcessUpdateD6(TriState_t state) { pins[D6] = state; }
    void ProcessUpdateD7(TriState_t state) { pins[D7] = state; }
    void ProcessUpdateD8(TriState_t state) { pins[D8] = state; }
    void ProcessUpdateClockLatch(TriState_t state);
    void ProcessUpdateClockOutput(TriState_t state);


signals:
    void SignalQ1Updated(TriState_t state);
    void SignalQ2Updated(TriState_t state);
    void SignalQ3Updated(TriState_t state);
    void SignalQ4Updated(TriState_t state);
    void SignalQ5Updated(TriState_t state);
    void SignalQ6Updated(TriState_t state);
    void SignalQ7Updated(TriState_t state);
    void SignalQ8Updated(TriState_t state);
};



