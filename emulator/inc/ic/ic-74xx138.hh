//===================================================================================================================
// ic-74xx193.hh -- This header file defines a 74xx138 3:8 Demultiplexer
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-26  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx138 Demultiplexer
//    ---------------------------------------------
class IC_74xx138_t : public QObject {
    Q_OBJECT

public:
    enum {A = 1,
        B = 2,
        C = 3,
        G2Ab = 4,
        G2Bb = 5,
        G1 = 6,
        Y7 = 7,
        Y6 = 9,
        Y5 = 10,
        Y4 = 11,
        Y3 = 12,
        Y2 = 13,
        Y1 = 14,
        Y0 = 15,
    };


private:
    TriState_t pins[PIN_CNT(16)];


public:
    IC_74xx138_t(void);
    virtual ~IC_74xx138_t() {};


public:
    void TriggerFirstUpdates(void) { AllUpdated(); emit UpdatesComplete(); }


public slots:
    void UpdateA(TriState_t state) { pins[A] = state; }
    void UpdateB(TriState_t state) { pins[B] = state; }
    void UpdateC(TriState_t state) { pins[C] = state; }
    void UpdateG1(TriState_t state) { pins[G1] = state; }
    void UpdateG2A(TriState_t state) { pins[G2Ab] = state; }
    void UpdateG2B(TriState_t state) { pins[G2Bb] = state; }
    void UpdatesComplete(void);


signals:
    void Y0Updated(TriState_t state);
    void Y1Updated(TriState_t state);
    void Y2Updated(TriState_t state);
    void Y3Updated(TriState_t state);
    void Y4Updated(TriState_t state);
    void Y5Updated(TriState_t state);
    void Y6Updated(TriState_t state);
    void Y7Updated(TriState_t state);
    void AllUpdated(void);
};



