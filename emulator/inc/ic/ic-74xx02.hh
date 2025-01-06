//===================================================================================================================
// ic-74xx02.hh -- This header file defines a 74xx02 Quad Two-Input NOR Gate IC
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-26  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx02 NOR Gate IC
//    ------------------------------------------
class IC_74xx02_t : public QObject {
    Q_OBJECT

public:
    enum {Y1 = 1,
        A1 = 2,
        B1 = 3,
        Y2 = 4,
        A2 = 5,
        B2 = 6,
        A3 = 8,
        B3 = 9,
        Y3 = 10,
        A4 = 11,
        B4 = 12,
        Y4 = 13,
    };


private:
    TriState_t pins[14+1];


public:
    IC_74xx02_t(void);
    virtual ~IC_74xx02_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    // -- Gate #1 slots
    void ProcessUpdateA1(TriState_t state) { if (state != pins[A1]) { pins[A1] = state; ProcessUpdateGate1(); } }
    void ProcessUpdateB1(TriState_t state) { if (state != pins[B1]) { pins[B1] = state; ProcessUpdateGate1(); } }
    void ProcessA1Low(void)  { ProcessUpdateA1(LOW); }
    void ProcessA1High(void) { ProcessUpdateA1(HIGH); }
    void ProcessB1Low(void)  { ProcessUpdateB1(LOW); }
    void ProcessB1High(void) { ProcessUpdateB1(HIGH); }
    void ProcessUpdateGate1(void);

    // -- Gate #2 slots
    void ProcessUpdateA2(TriState_t state) { if (state != pins[A2]) { pins[A2] = state; ProcessUpdateGate2(); } }
    void ProcessUpdateB2(TriState_t state) { if (state != pins[B2]) { pins[B2] = state; ProcessUpdateGate2(); } }
    void ProcessA2Low(void)  { ProcessUpdateA2(LOW); }
    void ProcessA2High(void) { ProcessUpdateA2(HIGH); }
    void ProcessB2Low(void)  { ProcessUpdateB2(LOW); }
    void ProcessB2High(void) { ProcessUpdateB2(HIGH); }
    void ProcessUpdateGate2(void);

    // -- Gate #3 slots
    void ProcessUpdateA3(TriState_t state) { if (state != pins[A3]) { pins[A3] = state; ProcessUpdateGate3(); } }
    void ProcessUpdateB3(TriState_t state) { if (state != pins[B3]) { pins[B3] = state; ProcessUpdateGate3(); } }
    void ProcessA3Low(void)  { ProcessUpdateA3(LOW); }
    void ProcessA3High(void) { ProcessUpdateA3(HIGH); }
    void ProcessB3Low(void)  { ProcessUpdateB3(LOW); }
    void ProcessB3High(void) { ProcessUpdateB3(HIGH); }
    void ProcessUpdateGate3(void);

    // -- Gate #4 slots
    void ProcessUpdateA4(TriState_t state) { if (state != pins[A4]) { pins[A4] = state; ProcessUpdateGate4(); } }
    void ProcessUpdateB4(TriState_t state) { if (state != pins[B4]) { pins[B4] = state; ProcessUpdateGate4(); } }
    void ProcessA4Low(void)  { ProcessUpdateA4(LOW); }
    void ProcessA4High(void) { ProcessUpdateA4(HIGH); }
    void ProcessB4Low(void)  { ProcessUpdateB4(LOW); }
    void ProcessB4High(void) { ProcessUpdateB4(HIGH); }
    void ProcessUpdateGate4(void);


    // -- Lots of things changed, update everything
    void UpdatesComplete(void) {
        ProcessUpdateGate1();
        ProcessUpdateGate2();
        ProcessUpdateGate3();
        ProcessUpdateGate4();
    }


signals:
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
};



