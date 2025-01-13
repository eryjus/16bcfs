//===================================================================================================================
// ic-74xx04.hh -- This header file defines a 74xx04 Hex Inverter
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-26  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx04 Hex Inverter
//    -------------------------------------------
class IC_74xx04_t : public QObject {
    Q_OBJECT

public:
    enum {A1 = 1,
        Y1 = 2,
        A2 = 3,
        Y2 = 4,
        A3 = 5,
        Y3 = 6,
        Y4 = 8,
        A4 = 9,
        Y5 = 10,
        A5 = 11,
        Y6 = 12,
        A6 = 13,
    };


private:
    TriState_t pins[14+1];


public:
    IC_74xx04_t(void);
    virtual ~IC_74xx04_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    // -- Gate #1 slots
    void ProcessUpdateA1(TriState_t state) { if (state != pins[A1]) { pins[A1] = state; ProcessUpdateGate1(); } }
    void ProcessA1Low(void)  { ProcessUpdateA1(LOW); }
    void ProcessA1High(void) { ProcessUpdateA1(HIGH); }
    void ProcessUpdateGate1(void);

    // -- Gate #2 slots
    void ProcessUpdateA2(TriState_t state) { if (state != pins[A2]) { pins[A2] = state; ProcessUpdateGate2(); } }
    void ProcessA2Low(void)  { ProcessUpdateA2(LOW); }
    void ProcessA2High(void) { ProcessUpdateA2(HIGH); }
    void ProcessUpdateGate2(void);

    // -- Gate #3 slots
    void ProcessUpdateA3(TriState_t state) { if (state != pins[A3]) { pins[A3] = state; ProcessUpdateGate3(); } }
    void ProcessA3Low(void)  { ProcessUpdateA3(LOW); }
    void ProcessA3High(void) { ProcessUpdateA3(HIGH); }
    void ProcessUpdateGate3(void);

    // -- Gate #4 slots
    void ProcessUpdateA4(TriState_t state) { if (state != pins[A4]) { pins[A4] = state; ProcessUpdateGate4(); } }
    void ProcessA4Low(void)  { ProcessUpdateA4(LOW); }
    void ProcessA4High(void) { ProcessUpdateA4(HIGH); }
    void ProcessUpdateGate4(void);

    // -- Gate #5 slots
    void ProcessUpdateA5(TriState_t state) { if (state != pins[A5]) { pins[A5] = state; ProcessUpdateGate5(); } }
    void ProcessA5Low(void)  { ProcessUpdateA5(LOW); }
    void ProcessA5High(void) { ProcessUpdateA5(HIGH); }
    void ProcessUpdateGate5(void);

    // -- Gate #6 slots
    void ProcessUpdateA6(TriState_t state) { if (state != pins[A6]) { pins[A6] = state; ProcessUpdateGate6(); } }
    void ProcessA6Low(void)  { ProcessUpdateA6(LOW); }
    void ProcessA6High(void) { ProcessUpdateA6(HIGH); }
    void ProcessUpdateGate6(void);




    // -- Lots of things changed, update everything
    void UpdatesComplete(void) {
        ProcessUpdateGate1();
        ProcessUpdateGate2();
        ProcessUpdateGate3();
        ProcessUpdateGate4();
        ProcessUpdateGate5();
        ProcessUpdateGate6();
    }


signals:
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
    void SignalY5Updated(TriState_t state);
    void SignalY6Updated(TriState_t state);
};



