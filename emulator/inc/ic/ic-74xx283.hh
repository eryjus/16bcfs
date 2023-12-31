//===================================================================================================================
// ic-74xx283.hh -- This class emulates a 74xx283 Full Adder
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-10  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx283 Full Adder
//    ------------------------------------------
class IC_74xx283_t : public QObject {
    Q_OBJECT

public:
    enum {S1 = 1,
        B1 = 2,
        A1 = 3,
        S0 = 4,
        A0 = 5,
        B0 = 6,
        CIN = 7,
        COUT = 9,
        S3 = 10,
        B3 = 11,
        A3 = 12,
        S2 = 13,
        A2 = 14,
        B2 = 15,
    };

private:
    unsigned a;
    unsigned b;
    unsigned sum;
    TriState_t pins[PIN_CNT(16)];
    bool output;

public:
    IC_74xx283_t(void);
    virtual ~IC_74xx283_t() {};


public:
    void TriggerFirstUpdate(void);


private:
    void UpdateSum(void);


public slots:
    void ProcessCInUpdate(TriState_t state) { pins[CIN] = state; UpdateSum(); }
    void ProcessA0Update(TriState_t state) { pins[A0] = state; UpdateSum(); }
    void ProcessB0Update(TriState_t state) { pins[B0] = state; UpdateSum(); }
    void ProcessA1Update(TriState_t state) { output = true; pins[A1] = state; UpdateSum(); output = false; }
    void ProcessB1Update(TriState_t state) { pins[B1] = state; UpdateSum(); }
    void ProcessA2Update(TriState_t state) { pins[A2] = state; UpdateSum(); }
    void ProcessB2Update(TriState_t state) { pins[B2] = state; UpdateSum(); }
    void ProcessA3Update(TriState_t state) { pins[A3] = state; UpdateSum(); }
    void ProcessB3Update(TriState_t state) { pins[B3] = state; UpdateSum(); }


signals:
    void SignalS0Updated(TriState_t state);
    void SignalS1Updated(TriState_t state);
    void SignalS2Updated(TriState_t state);
    void SignalS3Updated(TriState_t state);
    void SignalCOutUpdated(TriState_t state);
};



