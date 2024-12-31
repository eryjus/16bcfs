//===================================================================================================================
// ic-74xx165.hh -- This header file defines a 74xx165 Parallel load shift out register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  Internally, this register will treat the most significant bit as Qh being shited out of the register.
//  New bits will enter as the least significant bit.
//
//  NOTE: this has nothing to do with the external implenentation of these signals.  The inputs may as well
//  just as easily be pushing the least significant bit first.  There is no correlation.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This class implements a 74xx165 Shift Register
//    ----------------------------------------------
class IC_74xx165_t : public QObject {
    Q_OBJECT



public:
    enum {SH_LD = 1,
        CLK = 2,
        E = 3,
        F = 4,
        G = 5,
        H = 6,
        QHb = 7,
        QH = 9,
        SER = 10,
        A = 11,
        B = 12,
        C = 13,
        D = 14,
        CLK_INH = 15,
    };



private:
    TriState_t pins[PIN_CNT(16)];
    uint8_t val;
    TriState_t clockState;



public:
    IC_74xx165_t(void);
    virtual ~IC_74xx165_t() {};



public:
    void TriggerFirstUpdate(void) { emit SignalQHUpdated(pins[QH]); emit SignalQHbUpdated(pins[QHb]); }



private:
    void ProcessClock(void);
    void ProcessNewPin(void);



public slots:
    void ProcessUpdateA(TriState_t state) { pins[A] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateB(TriState_t state) { pins[B] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateC(TriState_t state) { pins[C] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateD(TriState_t state) { pins[D] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateE(TriState_t state) { pins[E] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateF(TriState_t state) { pins[F] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateG(TriState_t state) { pins[G] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateH(TriState_t state) { pins[H] = state; if (pins[SH_LD] == LOW) ProcessNewPin(); }
    void ProcessUpdateSer(TriState_t state) { pins[SER] = state; }
    void ProcessUpdateClk(TriState_t state) { pins[CLK] = state; ProcessClock(); }
    void ProcessUpdateClkInh(TriState_t state) { pins[CLK_INH] = state; ProcessClock(); }
    void ProcessUpdateShLd(TriState_t state) { pins[SH_LD] = state; ProcessNewPin(); };



signals:
    void SignalQHUpdated(TriState_t state);
    void SignalQHbUpdated(TriState_t state);
};



