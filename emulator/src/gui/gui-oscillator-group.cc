//===================================================================================================================
//  gui-oscillator-group.hh -- This is a group box visualizing an oscillator
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-oscillator-group.moc.cc"


//
// -- Construct an oscillator visual
//    ------------------------------
GUI_OscillatorGroup_t::GUI_OscillatorGroup_t(HW_Oscillator_t *hardware, QWidget *parent)
        : QGroupBox("Sq Wave", parent)
{
    hi = QPixmap("img/clk-hi.png");
    lo = QPixmap("img/clk-lo.png");

    // -- build the group box which will hold the oscillator visuals
    setObjectName("Oscillator Visual");

    QVBoxLayout *oscVLayout = new QVBoxLayout;
    oscVLayout->setObjectName("Oscillator Vert Layout");
    oscVLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(oscVLayout);

    setObjectName("Oscillator Group Box");
    setAlignment(Qt::AlignHCenter);
    setFixedHeight(60);
    setMaximumWidth(50);

    oscVisual = new QLabel;
    oscVisual->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    oscVisual->setPixmap(lo);

    oscVLayout->addWidget(oscVisual);
    setLayout(oscVLayout);

    // -- wire the oscillator state in the computer to its visual representation on the screen
    connect(hardware, &HW_Oscillator_t::SignalStateChanged, this, &GUI_OscillatorGroup_t::ProcessUpdateImage);
}



