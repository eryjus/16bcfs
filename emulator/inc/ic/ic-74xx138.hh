//===================================================================================================================
// ic-74xx138.hh -- This header file defines a 74xx138 3:8 Demultiplexer
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
    void TriggerFirstUpdate(void) { ProcessUpdatesComplete(); }


public slots:
    void ProcessUpdateA(TriState_t state) { pins[A] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateB(TriState_t state) { pins[B] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateC(TriState_t state) { pins[C] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateG1(TriState_t state) { pins[G1] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateG2A(TriState_t state) { pins[G2Ab] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateG2B(TriState_t state) { pins[G2Bb] = state; ProcessUpdatesComplete(); }
    void ProcessUpdatesComplete(void);


signals:
    void SignalY0Updated(TriState_t state);
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
    void SignalY5Updated(TriState_t state);
    void SignalY6Updated(TriState_t state);
    void SignalY7Updated(TriState_t state);
};



