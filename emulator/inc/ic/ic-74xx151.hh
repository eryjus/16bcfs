//===================================================================================================================
// ic-74xx151.hh -- This header file defines a 74xx151 Multiplexer
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-29  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx151 Mux
//    -----------------------------------
class IC_74xx151_t : public QObject {
    Q_OBJECT

public:
    enum {D3 = 1,
        D2 = 2,
        D1 = 3,
        D0 = 4,
        Y = 5,
        W = 6,
        STb = 7,
        C = 9,
        B = 10,
        A = 11,
        D7 = 12,
        D6 = 13,
        D5 = 14,
        D4 = 15,
    };


private:
    TriState_t pins[PIN_CNT(16)];


public:
    IC_74xx151_t(void);
    virtual ~IC_74xx151_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessUpdateA(TriState_t state) { pins[A] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateB(TriState_t state) { pins[B] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateC(TriState_t state) { pins[C] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD0(TriState_t state) { pins[D0] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD1(TriState_t state) { pins[D1] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD2(TriState_t state) { pins[D2] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD3(TriState_t state) { pins[D3] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD4(TriState_t state) { pins[D4] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD5(TriState_t state) { pins[D5] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD6(TriState_t state) { pins[D6] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD7(TriState_t state) { pins[D7] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateST(TriState_t state) { pins[STb] = state; ProcessUpdatesComplete(); }

    void ProcessUpdatesComplete(void);


signals:
    void SignalYUpdated(TriState_t state);
    void SignalWUpdated(TriState_t state);
};



