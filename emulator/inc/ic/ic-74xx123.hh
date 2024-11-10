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
    void TriggerFirstUpdate(void) {
        emit SignalQ1bUpdated(pins[Q1b]);
        emit SignalQ1Updated(pins[Q1]);
        emit SignalQ2bUpdated(pins[Q2b]);
        emit SignalQ2Updated(pins[Q2]);
    }



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


// -- Debugging slots
//    void DebugA1(TriState_t state) { DEBUG << objectName() << "#A1 " << state; }
//    void DebugB1(TriState_t state) { DEBUG << objectName() << "B1 " << state; }
//    void DebugRD1(TriState_t state) { DEBUG << objectName() << "RD1 " << state; }
//    void DebugQ1(TriState_t state) { DEBUG << objectName() << "Q1 " << state; }
//    void DebugQ1b(TriState_t state) { DEBUG << objectName() << "#Q1 " << state; }
    void DebugRC1(void) { DEBUG << "vib1:1 End of pulse 1"; }
//    void DebugA2(TriState_t state) { DEBUG << objectName() << "#A2 " << state; }
//    void DebugB2(TriState_t state) { DEBUG << objectName() << "B2 " << state; }
//    void DebugRD2(TriState_t state) { DEBUG << objectName() << "RD2 " << state; }
//    void DebugQ2(TriState_t state) { DEBUG << objectName() << "Q2 " << state; }
//    void DebugQ2b(TriState_t state) { DEBUG << objectName() << "#Q2 " << state; }
    void DebugRC2(void) { DEBUG << "vib1:2 End of pulse 2"; }


signals:
    void SignalQ1Updated(TriState_t state);
    void SignalQ1bUpdated(TriState_t state);
    void SignalQ2Updated(TriState_t state);
    void SignalQ2bUpdated(TriState_t state);
};



