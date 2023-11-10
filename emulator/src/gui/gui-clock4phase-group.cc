//===================================================================================================================
//  gui-clock4phase-group.cc -- This is a group box visualizing the 4-phase clock
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-clock4phase-group.moc.cc"


//
// -- Construct the group box that contains the LEDs showing the clock state
//    ----------------------------------------------------------------------
GUI_Clock4PhaseGroup_t::GUI_Clock4PhaseGroup_t(HW_Clock4Phase_t *clock, QWidget *parent)
        : QGroupBox(tr("4Ph Clk"), parent)
{
    led1 = new GUI_Led_t(GUI_Led_t::OnWhenLow);
    led2 = new GUI_Led_t(GUI_Led_t::OnWhenLow);
    led3 = new GUI_Led_t(GUI_Led_t::OnWhenLow);
    led4 = new GUI_Led_t(GUI_Led_t::OnWhenLow);

    phaseLed1 = new GUI_Led_t;
    phaseLed2 = new GUI_Led_t;

    setObjectName("Clock LED Pretty Box");
    setAlignment(Qt::AlignHCenter);
    setFixedHeight(60);
    setMaximumWidth(50);

    QHBoxLayout *clkLedHLayout1 = new QHBoxLayout();
    clkLedHLayout1->setAlignment(Qt::AlignHCenter);
    clkLedHLayout1->setContentsMargins(0, 0, 0, 0);
    clkLedHLayout1->setSpacing(2);

    led1->setObjectName("LED1");
    clkLedHLayout1->addWidget(led1);

    led2->setObjectName("LED2");
    clkLedHLayout1->addWidget(led2);

    led3->setObjectName("LED3");
    clkLedHLayout1->addWidget(led3);

    led4->setObjectName("LED4");
    clkLedHLayout1->addWidget(led4);


    QHBoxLayout *clkLedHLayout2 = new QHBoxLayout();
    clkLedHLayout2->setAlignment(Qt::AlignHCenter);
    clkLedHLayout2->setContentsMargins(0, 0, 0, 0);
    clkLedHLayout2->setSpacing(2);

    phaseLed1 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    phaseLed1->setObjectName("Phased Clock Major");
    clkLedHLayout2->addWidget(phaseLed1);

    phaseLed2 = new GUI_Led_t(GUI_Led_t::OnWhenHigh, Qt::blue);
    phaseLed2->setObjectName("Phased Clock Minor");
    clkLedHLayout2->addWidget(phaseLed2);

    QVBoxLayout *clkLedVLayout = new QVBoxLayout;
    clkLedVLayout->setAlignment(Qt::AlignHCenter);
    clkLedVLayout->setContentsMargins(0, 0, 0, 0);
    clkLedVLayout->addLayout(clkLedHLayout1);
    clkLedVLayout->addLayout(clkLedHLayout2);

    setLayout(clkLedVLayout);

    connect(clock, &HW_Clock4Phase_t::SignalCycle1Changed, led1, &GUI_Led_t::ProcessStateChange);
    connect(clock, &HW_Clock4Phase_t::SignalCycle2Changed, led2, &GUI_Led_t::ProcessStateChange);
    connect(clock, &HW_Clock4Phase_t::SignalCycle3Changed, led3, &GUI_Led_t::ProcessStateChange);
    connect(clock, &HW_Clock4Phase_t::SignalCycle4Changed, led4, &GUI_Led_t::ProcessStateChange);

    connect(clock, &HW_Clock4Phase_t::SignalPhase1Changed, phaseLed1, &GUI_Led_t::ProcessStateChange);
    connect(clock, &HW_Clock4Phase_t::SignalPhase2Changed, phaseLed2, &GUI_Led_t::ProcessStateChange);
}


