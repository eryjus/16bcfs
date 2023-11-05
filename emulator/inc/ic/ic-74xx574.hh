//===================================================================================================================
// ic-74xx193.hh -- This class implements an Octal 74xx574 D-Type Latch
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-30  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements an Octal 74xx574 D-Type Latch
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

public:
    IC_74xx574_t(void);
    virtual ~IC_74xx574_t() {};


public:
    void TriggerFirstUpdates(void);


public slots:
    void UpdateOE(TriState_t state);
    void UpdateD1(TriState_t state) { pins[D1] = state; }
    void UpdateD2(TriState_t state) { pins[D2] = state; }
    void UpdateD3(TriState_t state) { pins[D3] = state; }
    void UpdateD4(TriState_t state) { pins[D4] = state; }
    void UpdateD5(TriState_t state) { pins[D5] = state; }
    void UpdateD6(TriState_t state) { pins[D6] = state; }
    void UpdateD7(TriState_t state) { pins[D7] = state; }
    void UpdateD8(TriState_t state) { pins[D8] = state; }
    void UpdateClk(TriState_t state);


signals:
    void Q1Updated(TriState_t state);
    void Q2Updated(TriState_t state);
    void Q3Updated(TriState_t state);
    void Q4Updated(TriState_t state);
    void Q5Updated(TriState_t state);
    void Q6Updated(TriState_t state);
    void Q7Updated(TriState_t state);
    void Q8Updated(TriState_t state);
};



