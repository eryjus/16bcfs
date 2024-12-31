//===================================================================================================================
// ic-74xx30.hh -- This header file defines a 74xx30 8-Input NAND Gate IC
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-27  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx30 NAND Gate IC
//    -------------------------------------------
class IC_74xx30_t : public QObject {
    Q_OBJECT

public:
    enum {A = 1,
        B = 2,
        C = 3,
        D = 4,
        E = 5,
        F = 6,
        Y = 8,
        G = 11,
        H = 12,
    };

private:
    TriState_t pins[PIN_CNT(14)];


public:
    IC_74xx30_t(void);
    virtual ~IC_74xx30_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    // -- Gate #1 slots
    void ProcessUpdateA(TriState_t state) { if (state != pins[A]) { pins[A] = state; ProcessUpdateGate(); } }
    void ProcessUpdateB(TriState_t state) { if (state != pins[B]) { pins[B] = state; ProcessUpdateGate(); } }
    void ProcessUpdateC(TriState_t state) { if (state != pins[C]) { pins[C] = state; ProcessUpdateGate(); } }
    void ProcessUpdateD(TriState_t state) { if (state != pins[D]) { pins[D] = state; ProcessUpdateGate(); } }
    void ProcessUpdateE(TriState_t state) { if (state != pins[E]) { pins[E] = state; ProcessUpdateGate(); } }
    void ProcessUpdateF(TriState_t state) { if (state != pins[F]) { pins[F] = state; ProcessUpdateGate(); } }
    void ProcessUpdateG(TriState_t state) { if (state != pins[G]) { pins[G] = state; ProcessUpdateGate(); } }
    void ProcessUpdateH(TriState_t state) { if (state != pins[H]) { pins[H] = state; ProcessUpdateGate(); } }
    void ProcessALow(void)  { ProcessUpdateA(LOW); }
    void ProcessAHigh(void) { ProcessUpdateA(HIGH); }
    void ProcessBLow(void)  { ProcessUpdateB(LOW); }
    void ProcessBHigh(void) { ProcessUpdateB(HIGH); }
    void ProcessCLow(void)  { ProcessUpdateC(LOW); }
    void ProcessCHigh(void) { ProcessUpdateC(HIGH); }
    void ProcessDLow(void)  { ProcessUpdateD(LOW); }
    void ProcessDHigh(void) { ProcessUpdateD(HIGH); }
    void ProcessELow(void)  { ProcessUpdateE(LOW); }
    void ProcessEHigh(void) { ProcessUpdateE(HIGH); }
    void ProcessFLow(void)  { ProcessUpdateF(LOW); }
    void ProcessFHigh(void) { ProcessUpdateF(HIGH); }
    void ProcessGLow(void)  { ProcessUpdateG(LOW); }
    void ProcessGHigh(void) { ProcessUpdateG(HIGH); }
    void ProcessHLow(void)  { ProcessUpdateH(LOW); }
    void ProcessHHigh(void) { ProcessUpdateH(HIGH); }
    void ProcessUpdateGate(void);


    // -- Lots of things changed, update everything
    void UpdatesComplete(void) { ProcessUpdateGate(); }

signals:
    void SignalYUpdated(TriState_t state);
};



