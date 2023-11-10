//===================================================================================================================
//  gui-bus-leds.cc -- This is a group box visualizing a bus contents
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-01  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/gui-bus-leds.moc.cc"



//
// -- construct the bus LED display, hooking the LEDs to the proper bus output signals
//    --------------------------------------------------------------------------------
GUI_BusLeds_t::GUI_BusLeds_t(HW_Bus_t *bus, QWidget *parent) : QGroupBox("Fictitious Bus", parent)
{
    led0 = new GUI_Led_t;
    led1 = new GUI_Led_t;
    led2 = new GUI_Led_t;
    led3 = new GUI_Led_t;
    led4 = new GUI_Led_t;
    led5 = new GUI_Led_t;
    led6 = new GUI_Led_t;
    led7 = new GUI_Led_t;
    led8 = new GUI_Led_t;
    led9 = new GUI_Led_t;
    ledA = new GUI_Led_t;
    ledB = new GUI_Led_t;
    ledC = new GUI_Led_t;
    ledD = new GUI_Led_t;
    ledE = new GUI_Led_t;
    ledF = new GUI_Led_t;
    setObjectName(tr("Fictitious Bus"));

    setAlignment(Qt::AlignHCenter);
    setFixedWidth(190);
    setFixedHeight(60);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);

    layout->addWidget(ledF);
    layout->addWidget(ledE);
    layout->addWidget(ledD);
    layout->addWidget(ledC);
    layout->addSpacerItem(new QSpacerItem(4, 10));
    layout->addWidget(ledB);
    layout->addWidget(ledA);
    layout->addWidget(led9);
    layout->addWidget(led8);
    layout->addSpacerItem(new QSpacerItem(4, 10));
    layout->addWidget(led7);
    layout->addWidget(led6);
    layout->addWidget(led5);
    layout->addWidget(led4);
    layout->addSpacerItem(new QSpacerItem(4, 10));
    layout->addWidget(led3);
    layout->addWidget(led2);
    layout->addWidget(led1);
    layout->addWidget(led0);

    layout->setAlignment(ledF, Qt::AlignHCenter);
    layout->setAlignment(ledE, Qt::AlignHCenter);
    layout->setAlignment(ledD, Qt::AlignHCenter);
    layout->setAlignment(ledC, Qt::AlignHCenter);
    layout->setAlignment(ledB, Qt::AlignHCenter);
    layout->setAlignment(ledA, Qt::AlignHCenter);
    layout->setAlignment(led9, Qt::AlignHCenter);
    layout->setAlignment(led8, Qt::AlignHCenter);
    layout->setAlignment(led7, Qt::AlignHCenter);
    layout->setAlignment(led6, Qt::AlignHCenter);
    layout->setAlignment(led5, Qt::AlignHCenter);
    layout->setAlignment(led4, Qt::AlignHCenter);
    layout->setAlignment(led3, Qt::AlignHCenter);
    layout->setAlignment(led2, Qt::AlignHCenter);
    layout->setAlignment(led1, Qt::AlignHCenter);
    layout->setAlignment(led0, Qt::AlignHCenter);

    setLayout(layout);


    // -- make the connections
    connect(bus, &HW_Bus_t::SignalBit0Updated, led0, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit1Updated, led1, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit2Updated, led2, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit3Updated, led3, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit4Updated, led4, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit5Updated, led5, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit6Updated, led6, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit7Updated, led7, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit8Updated, led8, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBit9Updated, led9, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitAUpdated, ledA, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitBUpdated, ledB, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitCUpdated, ledC, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitDUpdated, ledD, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitEUpdated, ledE, &GUI_Led_t::ProcessStateChange);
    connect(bus, &HW_Bus_t::SignalBitFUpdated, ledF, &GUI_Led_t::ProcessStateChange);
}



