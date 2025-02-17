//===================================================================================================================
//  gui-led.hh -- A label visualizing an led
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-25  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "gui-led.moc.cc"


//
// -- we will only use one off color
//    ------------------------------
QPalette GUI_Led_t::offColor(QPalette::Window, Qt::gray);


//
// -- Construct a new LED
//    -------------------
GUI_Led_t::GUI_Led_t(LedWiring_t wire, Qt::GlobalColor on, QWidget *parent) : QLabel(parent), onWhen(wire)
{
    onColor = QPalette(QPalette::Window, on);
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Window);
    setPalette(offColor);
    setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    setMaximumHeight(14);
}


//
// -- Construct a new LED
//    -------------------
GUI_Led_t::GUI_Led_t(QWidget *parent, LedWiring_t wire, Qt::GlobalColor on) : QLabel(parent), onWhen(wire)
{
    onColor = QPalette(QPalette::Window, on);
    setAutoFillBackground(true);
    setBackgroundRole(QPalette::Window);
    setPalette(offColor);
}

