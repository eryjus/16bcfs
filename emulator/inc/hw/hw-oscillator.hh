//===================================================================================================================
//  hw-oscillator.hh -- This is a generic oscillator
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-28  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- This is on oscillator, built on the QTimer class
//    ------------------------------------------------
class HW_Oscillator_t : public QTimer {
    Q_OBJECT


private:
    TriState_t state;

public:
    HW_Oscillator_t(int interval = 500);
    virtual ~HW_Oscillator_t() { stop(); }

public:
    TriState_t GetState(void) const { return state; }
    void StartTimer(void) { start(); }

public slots:
    void ProcessTimerStateChange(void) {
        state = (state==HIGH?LOW:HIGH);
        emit SignalStateChanged(state);
        emit SignalSanityCheck();
    }
    void ProcessSetInterval(int interval) { QTimer::setInterval(interval); }
    void ProcessRawInterval(int interval) { QTimer::setInterval((10-interval)*100); }

signals:
    void SignalStateChanged(TriState_t state);
    void SignalSanityCheck(void);

};

