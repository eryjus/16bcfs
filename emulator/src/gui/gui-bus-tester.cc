//===================================================================================================================
//  gui-bus-tester.cc -- This is a generic bus tester
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-bus-tester.moc.cc"



//
// -- Construct the bus tester, which will manually push values onto a bus
//    --------------------------------------------------------------------
GUI_BusTester_t::GUI_BusTester_t(QString name, HW_BusDriver_t *driver, QGroupBox *parent) : QGroupBox(name, parent)
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

    setObjectName(name);

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

    connect(dipF, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitF, CNN_TYPE);
    connect(dipE, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitE, CNN_TYPE);
    connect(dipD, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitD, CNN_TYPE);
    connect(dipC, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitC, CNN_TYPE);
    connect(dipB, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitB, CNN_TYPE);
    connect(dipA, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBitA, CNN_TYPE);
    connect(dip9, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit9, CNN_TYPE);
    connect(dip8, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit8, CNN_TYPE);
    connect(dip7, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit7, CNN_TYPE);
    connect(dip6, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit6, CNN_TYPE);
    connect(dip5, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit5, CNN_TYPE);
    connect(dip4, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit4, CNN_TYPE);
    connect(dip3, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit3, CNN_TYPE);
    connect(dip2, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit2, CNN_TYPE);
    connect(dip1, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit1, CNN_TYPE);
    connect(dip0, &GUI_DipSwitch_t::SignalSwitchChanged, driver, &HW_BusDriver_t::ProcessUpdateBit0, CNN_TYPE);

    connect(oe, &QCheckBox::stateChanged, this, &GUI_BusTester_t::ProcessOEChange, CNN_TYPE);
    connect(this, &GUI_BusTester_t::SignalOeUpdated, driver, &HW_BusDriver_t::ProcessUpdateOE1, CNN_TYPE);
    connect(this, &GUI_BusTester_t::SignalOeUpdated, driver, &HW_BusDriver_t::ProcessUpdateOE2, CNN_TYPE);
}
