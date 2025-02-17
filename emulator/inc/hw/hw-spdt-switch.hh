//===================================================================================================================
//  hw-spdt-switch.hh -- This class implements a single pole double throw switch
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a momentary switch
//    --------------------------
class HW_SpdtSwitch_t : public QPushButton {
    Q_OBJECT


public:
    typedef enum {
        HIGH_WHEN_PRESSED = 0,
        HIGH_WHEN_RELEASED = 1,
    } HighWhen_t;


private:
    HighWhen_t highWhen;


public:
    explicit HW_SpdtSwitch_t(QString name, HighWhen_t w = HIGH_WHEN_PRESSED, QWidget *parent = nullptr);
    virtual ~HW_SpdtSwitch_t() {}


signals:
    void SignalState(TriState_t state);


private slots:
    void ProcessPressed(void) { emit SignalState(highWhen==HIGH_WHEN_PRESSED?HIGH:LOW); }
    void ProcessReleased(void) { emit SignalState(highWhen==HIGH_WHEN_PRESSED?LOW:HIGH); }
};
