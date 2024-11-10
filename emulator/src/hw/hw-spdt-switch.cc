//===================================================================================================================
//  hw-spdt-switch.cc -- This class implements a single pole double throw switch
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-spdt-switch.moc.cc"


//
// -- construct a momentary switch
//    ----------------------------
HW_SpdtSwitch_t::HW_SpdtSwitch_t(QString name, HighWhen_t w, QWidget *parent)
        : QPushButton(name, parent), highWhen(w)
{
    connect(this, &QPushButton::pressed, this, &HW_SpdtSwitch_t::ProcessPressed);
    connect(this, &QPushButton::released, this, &HW_SpdtSwitch_t::ProcessReleased);
}




