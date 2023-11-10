//===================================================================================================================
//  gui-bus-tester.cc -- This is a generic bus tester
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-bus-tester.moc.cc"



GUI_BusTester_t::GUI_BusTester_t(HW_BusDriver_t *driver, QGroupBox *parent) : QGroupBox("Bus Tester", parent)
{
    dip0 = new GUI_DipSwitch_t;
    dip1 = new GUI_DipSwitch_t;
    dip2 = new GUI_DipSwitch_t;
    dip3 = new GUI_DipSwitch_t;
    dip4 = new GUI_DipSwitch_t;
    dip5 = new GUI_DipSwitch_t;
    dip6 = new GUI_DipSwitch_t;
    dip7 = new GUI_DipSwitch_t;
    dip8 = new GUI_DipSwitch_t;
    dip9 = new GUI_DipSwitch_t;
    dipA = new GUI_DipSwitch_t;
    dipB = new GUI_DipSwitch_t;
    dipC = new GUI_DipSwitch_t;
    dipD = new GUI_DipSwitch_t;
    dipE = new GUI_DipSwitch_t;
    dipF = new GUI_DipSwitch_t;

    oe = new QCheckBox;

    setObjectName(tr("Bus Tester"));

    setAlignment(Qt::AlignHCenter);
    setFixedWidth(260);
    setFixedHeight(60);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);

    QVBoxLayout *enab = new QVBoxLayout;
    enab->setContentsMargins(0, 0, 0, 0);
    enab->setSpacing(1);
    enab->addWidget(new QLabel("OE"));
    enab->addWidget(oe);

    QWidget *w = new QWidget;
    w->setLayout(enab);

    layout->addWidget(w);

    layout->addWidget(dipF);
    layout->addWidget(dipE);
    layout->addWidget(dipD);
    layout->addWidget(dipC);
    layout->addWidget(dipB);
    layout->addWidget(dipA);
    layout->addWidget(dip9);
    layout->addWidget(dip8);
    layout->addWidget(dip7);
    layout->addWidget(dip6);
    layout->addWidget(dip5);
    layout->addWidget(dip4);
    layout->addWidget(dip3);
    layout->addWidget(dip2);
    layout->addWidget(dip1);
    layout->addWidget(dip0);

    setLayout(layout);

    connect(dipF, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitF);
    connect(dipE, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitE);
    connect(dipD, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitD);
    connect(dipC, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitC);
    connect(dipB, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitB);
    connect(dipA, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitA);
    connect(dip9, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit9);
    connect(dip8, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit8);
    connect(dip7, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit7);
    connect(dip6, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit6);
    connect(dip5, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit5);
    connect(dip4, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit4);
    connect(dip3, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit3);
    connect(dip2, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit2);
    connect(dip1, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit1);
    connect(dip0, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit0);

    connect(oe, &QCheckBox::stateChanged, this, &GUI_BusTester_t::ProcessOEChange);
    connect(this, &GUI_BusTester_t::SignalOeUpdated, driver, &HW_BusDriver_t::ProcessUpdateOE1);
    connect(this, &GUI_BusTester_t::SignalOeUpdated, driver, &HW_BusDriver_t::ProcessUpdateOE2);
}
