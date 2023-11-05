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
    void TriggerFirstUpdates(void);


public slots:
    void UpdateB(TriState_t state) { pins[B] = state; }
    void UpdateDown(TriState_t state) { lastDown = pins[DOWN]; pins[DOWN] = state; UpdatesComplete(); }
    void UpdateUp(TriState_t state) { lastUp = pins[UP]; pins[UP] = state; UpdatesComplete(); }
    void UpdateD(TriState_t state) { pins[D] = state; }
    void UpdateC(TriState_t state) { pins[C] = state; }
    void UpdateLoad(TriState_t state) { pins[LOADb] = state; }
    void UpdateClr(TriState_t state) { pins[CLR] = state; }
    void UpdateA(TriState_t state) { pins[A] = state; }
    void UpdatesComplete(void);

signals:
    void QaUpdated(TriState_t state);
    void QbUpdated(TriState_t state);
    void QcUpdated(TriState_t state);
    void QdUpdated(TriState_t state);
    void BoUpdated(TriState_t state);
    void CoUpdated(TriState_t state);
    void AllUpdated(void);
};



