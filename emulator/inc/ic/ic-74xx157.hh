//===================================================================================================================
// ic-74xx157.hh -- This header file defines a 74xx157 Quad 2:1 Multiplexer
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This class implements a 74xx157 Quad 2:1 Mux
//    --------------------------------------------
class IC_74xx157_t : public QObject {
    Q_OBJECT



public:
    enum {A_B = 1,
        A1 = 2,
        B1 = 3,
        Y1 = 4,
        A2 = 5,
        B2 = 6,
        Y2 = 7,
        Y3 = 9,
        B3 = 10,
        A3 = 11,
        Y4 = 12,
        B4 = 13,
        A4 = 14,
        Gb = 15,
    };



private:
    TriState_t pins[PIN_CNT(16)];



public:
    IC_74xx157_t(void);
    virtual ~IC_74xx157_t() {};



public:
    void TriggerFirstUpdate(void);


private:
    void ProcessUpdate(void);



public slots:
    void ProcessUpdateA1(TriState_t state) { pins[A1] = state; ProcessUpdate(); }
    void ProcessUpdateB1(TriState_t state) { pins[B1] = state; ProcessUpdate(); }
    void ProcessUpdateA2(TriState_t state) { pins[A2] = state; ProcessUpdate(); }
    void ProcessUpdateB2(TriState_t state) { pins[B2] = state; ProcessUpdate(); }
    void ProcessUpdateA3(TriState_t state) { pins[A3] = state; ProcessUpdate(); }
    void ProcessUpdateB3(TriState_t state) { pins[B3] = state; ProcessUpdate(); }
    void ProcessUpdateA4(TriState_t state) { pins[A4] = state; ProcessUpdate(); }
    void ProcessUpdateB4(TriState_t state) { pins[B4] = state; ProcessUpdate(); }
    void ProcessUpdateAB(TriState_t state) { pins[A_B] = state; ProcessUpdate(); }
    void ProcessUpdateGb(TriState_t state) { pins[Gb] = state; ProcessUpdate(); }



signals:
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
};



