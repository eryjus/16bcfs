//===================================================================================================================
// ic-74xx123.hh -- This header file defines a 74xx123 multivirbator
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-03  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx123 Multivibrator
//    ---------------------------------------------
class IC_74xx123_t : public QObject {
    Q_OBJECT

public:
    enum {A1b = 1,
        B1 = 2,
        RD1b = 3,
        Q1b = 4,
        Q2 = 5,
        CEXT2 = 6,      // -- not connected
        RCEXT2 = 7,     // -- timeout pin
        A2b = 9,
        B2 = 10,
        RD2b = 11,
        Q2b = 12,
        Q1 = 13,
        CEXT1 = 14,     // -- not connected
        RCEXT1 = 15,    // -- timeout pin
    };


private:
    TriState_t pins[PIN_CNT(16)];

    TriState_t a1;
    TriState_t b1;
    TriState_t rd1;
    TriState_t a2;
    TriState_t b2;
    TriState_t rd2;

    bool cycling1;
    bool cycling2;


public:
    IC_74xx123_t(void);
    virtual ~IC_74xx123_t() {};



public:
    void TriggerFirstUpdate(void) { ProcessUpdates1Complete(); ProcessUpdates2Complete(); }



public slots:
    void ProcessUpdateA1b(TriState_t state) { pins[A1b] = state; ProcessUpdates1Complete(); }
    void ProcessUpdateB1(TriState_t state) { pins[B1] = state; ProcessUpdates1Complete(); }
    void ProcessUpdateRD1b(TriState_t state) { pins[RD1b] = state; ProcessUpdates1Complete(); }
    void ProcessUpdateRCEXT1(void);
    void ProcessUpdateA2b(TriState_t state) { pins[A2b] = state; ProcessUpdates2Complete(); }
    void ProcessUpdateB2(TriState_t state) { pins[B2] = state; ProcessUpdates2Complete(); }
    void ProcessUpdateRD2b(TriState_t state) { pins[RD2b] = state; ProcessUpdates2Complete(); }
    void ProcessUpdateRCEXT2(void);
    void ProcessUpdates1Complete(void);
    void ProcessUpdates2Complete(void);



signals:
    void SignalQ1Updated(TriState_t state);
    void SignalQ1bUpdated(TriState_t state);
    void SignalQ2Updated(TriState_t state);
    void SignalQ2bUpdated(TriState_t state);
};



