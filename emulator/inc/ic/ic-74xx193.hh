//===================================================================================================================
// ic-74xx193.hh -- This header file defines a 74xx193 Up/Down Counter
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-25  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx193 Up/Down Counter
//    -----------------------------------------------
class IC_74xx193_t : public QObject {
    Q_OBJECT

public:
    enum {B = 1,
        QB = 2,
        QA = 3,
        DOWN = 4,
        UP = 5,
        QC = 6,
        QD = 7,
        D = 9,
        C = 10,
        LOADb = 11,
        COb = 12,
        BOb = 13,
        CLR = 14,
        A = 15,
    };

private:
    int cnt;
    TriState_t pins[PIN_CNT(16)];
    TriState_t lastUp;
    TriState_t lastDown;

public:
    IC_74xx193_t(void);
    virtual ~IC_74xx193_t() {};


public:
    void TriggerFirstUpdate(void);


public slots:
    void ProcessUpdateB(TriState_t state) { pins[B] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateDown(TriState_t state) { lastDown = pins[DOWN]; pins[DOWN] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateUp(TriState_t state) { lastUp = pins[UP]; pins[UP] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateD(TriState_t state) { pins[D] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateC(TriState_t state) { pins[C] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateLoad(TriState_t state) { pins[LOADb] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateClr(TriState_t state) { pins[CLR] = state; ProcessUpdatesComplete(); }
    void ProcessUpdateA(TriState_t state) { pins[A] = state; ProcessUpdatesComplete(); }
    void ProcessUpdatesComplete(void);

signals:
    void SignalQaUpdated(TriState_t state);
    void SignalQbUpdated(TriState_t state);
    void SignalQcUpdated(TriState_t state);
    void SignalQdUpdated(TriState_t state);
    void SignalBoUpdated(TriState_t state);
    void SignalCoUpdated(TriState_t state);
};



