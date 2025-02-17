//===================================================================================================================
//  hw-momentary-switch.cc -- This class implements a momentary switch
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-momentary-switch.moc.cc"


//
// -- construct a momentary switch
//    ----------------------------
HW_MomentarySwitch_t::HW_MomentarySwitch_t(QString name, HighWhen_t w, QWidget *parent)
        : QPushButton(name, parent), highWhen(w)
{
    connect(this, &QPushButton::pressed, this, &HW_MomentarySwitch_t::ProcessPressed);
    connect(this, &QPushButton::released, this, &HW_MomentarySwitch_t::ProcessReleased);

    TriggerFirstUpdate();
}


//
// -- Trigger the first updates to put the object in sync
//    ---------------------------------------------------
void HW_MomentarySwitch_t::TriggerFirstUpdate(void)
{
    Release();
//    emit pressed();
//    emit released();
}



