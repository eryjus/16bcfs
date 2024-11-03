//===================================================================================================================
//  gui-mementary-switch.cc -- A button visualizing a pushbutton switch
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-momentary-switch.moc.cc"



//
// -- Construct a dip switch
//    ----------------------
GUI_MomentarySwitch_t::GUI_MomentarySwitch_t(const QString &name, TriState_t p, TriState_t r, QWidget *parent)
        : QPushButton(name, parent)
{
    whenPressed = p;
    whenReleased = r;
    current = whenReleased;

    setFixedHeight(25);

    connect(this, &GUI_MomentarySwitch_t::pressed, this, &GUI_MomentarySwitch_t::ProcessClick, CNN_TYPE);
    connect(this, &GUI_MomentarySwitch_t::released, this, &GUI_MomentarySwitch_t::ProcessRelease, CNN_TYPE);
}

