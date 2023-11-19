//===================================================================================================================
//  gui-clock-led-group.cc -- This is a group box for visualizing the clock cycle
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-clock-led-group.moc.cc"



//
// -- This group box visualizes the computer's clock
//    ----------------------------------------------
GUI_ClockLedGroup_t::GUI_ClockLedGroup_t(HW_Clock_t *clk) : QGroupBox("Clock")
{
    clockLed = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    setObjectName(tr("Clock LED Group"));

    setAlignment(Qt::AlignHCenter);
    setFixedWidth(50);
    setFixedHeight(60);

    QHBoxLayout *led = new QHBoxLayout();
    led->setAlignment(Qt::AlignHCenter);
    led->setContentsMargins(0, 0, 0, 0);
    led->setSpacing(2);

    led->addWidget(clockLed, 2, Qt::AlignHCenter);

    setLayout(led);

    // -- connect the signal
    connect(clk, &HW_Clock_t::SignalClockCycle, clockLed, &GUI_Led_t::ProcessStateChange);
}

