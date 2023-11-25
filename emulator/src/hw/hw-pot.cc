//===================================================================================================================
//  hw-pot.cc -- This class implements a variable resistor or potentiometer
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-pot.moc.cc"


//
// -- construct a potentiometer or variable resistor
//    ----------------------------------------------
HW_Pot_t::HW_Pot_t(Qt::Orientation orient) : QSlider(orient)
{
    setMinimum(1);
    setMaximum(10);
    setFixedWidth(35);
    setValue(5);

    label = new QLabel("5");
    label->setAlignment(Qt::AlignHCenter);

    connect(this, &QSlider::valueChanged, this, &HW_Pot_t::ProcessUpdateValue);
}
