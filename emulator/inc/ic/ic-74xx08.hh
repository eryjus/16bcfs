//===================================================================================================================
// ic-74xx08.hh -- This header file defines a 74xx08 Quad Two-Input AND Gate IC
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-23  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx08 AND Gate IC
//    ------------------------------------------
class IC_74xx08_t : public QObject {
    Q_OBJECT

public:
    enum {A1 = 1,
        B1 = 2,
        Y1 = 3,
        A2 = 4,
        B2 = 5,
        Y2 = 6,
        Y3 = 8,
        B3 = 9,
        A3 = 10,
        Y4 = 11,
        B4 = 12,
        A4 = 13,
    };

private:
    TriState_t pins[14+1];


public:
    IC_74xx08_t(void);
    virtual ~IC_74xx08_t() {};

public:
    void TriggerFirstUpdates(void);


public slots:
    // -- Gate #1 slots
    void UpdateA1(TriState_t state) { if (state != pins[A1]) { pins[A1] = state; UpdateGate1(); } }
    void UpdateB1(TriState_t state) { if (state != pins[B1]) { pins[B1] = state; UpdateGate1(); } }
    void A1Low(void)  { UpdateA1(LOW); }
    void A1High(void) { UpdateA1(HIGH); }
    void B1Low(void)  { UpdateB1(LOW); }
    void B1High(void) { UpdateB1(HIGH); }
    void UpdateGate1(void);

    // -- Gate #2 slots
    void UpdateA2(TriState_t state) { if (state != pins[A2]) { pins[A2] = state; UpdateGate2(); } }
    void UpdateB2(TriState_t state) { if (state != pins[B2]) { pins[B2] = state; UpdateGate2(); } }
    void A2Low(void)  { UpdateA2(LOW); }
    void A2High(void) { UpdateA2(HIGH); }
    void B2Low(void)  { UpdateB2(LOW); }
    void B2High(void) { UpdateB2(HIGH); }
    void UpdateGate2(void);

    // -- Gate #3 slots
    void UpdateA3(TriState_t state) { if (state != pins[A3]) { pins[A3] = state; UpdateGate3(); } }
    void UpdateB3(TriState_t state) { if (state != pins[B3]) { pins[B3] = state; UpdateGate3(); } }
    void A3Low(void)  { UpdateA3(LOW); }
    void A3High(void) { UpdateA3(HIGH); }
    void B3Low(void)  { UpdateB3(LOW); }
    void B3High(void) { UpdateB3(HIGH); }
    void UpdateGate3(void);

    // -- Gate #4 slots
    void UpdateA4(TriState_t state) { if (state != pins[A4]) { pins[A4] = state; UpdateGate4(); } }
    void UpdateB4(TriState_t state) { if (state != pins[B4]) { pins[B4] = state; UpdateGate4(); } }
    void A4Low(void)  { UpdateA4(LOW); }
    void A4High(void) { UpdateA4(HIGH); }
    void B4Low(void)  { UpdateB4(LOW); }
    void B4High(void) { UpdateB4(HIGH); }
    void UpdateGate4(void);


    // -- Lots of things changed, update everything
    void UpdatesComplete(void) {
        UpdateGate1();
        UpdateGate2();
        UpdateGate3();
        UpdateGate4();
    }


signals:
    void Y1Updated(TriState_t state);
    void Y2Updated(TriState_t state);
    void Y3Updated(TriState_t state);
    void Y4Updated(TriState_t state);
};



