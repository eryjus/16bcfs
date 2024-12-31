//===================================================================================================================
//  gui-dip-switch.cc -- A slider visualizing a dip switch with 2 possible states
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-07  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-dip-switch.moc.cc"



//
// -- Construct a dip switch
//    ----------------------
GUI_DipSwitch_t::GUI_DipSwitch_t(QWidget *parent) : QSlider(Qt::Vertical, parent)
{
    setMinimum(0);
    setMaximum(1);
    setFixedHeight(25);

    connect(this, &GUI_DipSwitch_t::sliderPressed, this, &GUI_DipSwitch_t::ProcessClick);
    connect(this, &GUI_DipSwitch_t::valueChanged, this, &GUI_DipSwitch_t::ProcessStateChange);
}

