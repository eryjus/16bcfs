//===================================================================================================================
//  gui-pushbutton.hh -- A button for a pushbutton switch
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- A push button is what you would think it would be
//    -------------------------------------------------
class GUI_MomentarySwitch_t : public QPushButton {
    Q_OBJECT


public:
    TriState_t whenPressed;
    TriState_t whenReleased;
    TriState_t current;



public:
    GUI_MomentarySwitch_t(const QString &name, TriState_t p = HIGH, TriState_t r = LOW, QWidget *parent = nullptr);
    ~GUI_MomentarySwitch_t() {}


public slots:
    void ProcessClick(void) { current = whenPressed; emit SignalSwitchChanged(whenPressed); }
    void ProcessRelease(void) { current = whenReleased; emit SignalSwitchChanged(whenReleased); }


signals:
    void SignalSwitchChanged(TriState_t state);
};


