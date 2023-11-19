//===================================================================================================================
//  gui-clock-mode-group.cc -- This is a group box for selecting which clock mode
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-17  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-clock-mode-group.moc.cc"



//
// -- conctruct the GUI controls for the run/step clock modes
//    -------------------------------------------------------
GUI_ClockModeGroup_t::GUI_ClockModeGroup_t(HW_Clock_t *clock, QWidget *parent)
        : QGroupBox("Clock Mode", parent)
{
    modeLedRun = new GUI_Led_t;
    modeLedStep = new GUI_Led_t;

    QPushButton *runButton = new QPushButton("Run");
    QPushButton *stepButton = new QPushButton("1-Step");

    setObjectName(tr("Clock Mode Group"));

    setAlignment(Qt::AlignHCenter);
    setFixedWidth(120);
    setFixedHeight(60);

    QHBoxLayout *modeOptions = new QHBoxLayout();
    modeOptions->setAlignment(Qt::AlignHCenter);
    modeOptions->setContentsMargins(0, 0, 0, 0);
    modeOptions->setSpacing(2);

    QVBoxLayout *clkModeRun = new QVBoxLayout;
    clkModeRun->setAlignment(Qt::AlignHCenter);
    clkModeRun->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *clkModeStep = new QVBoxLayout;
    clkModeStep->setAlignment(Qt::AlignHCenter);
    clkModeStep->setContentsMargins(0, 0, 0, 0);

    modeLedRun->setObjectName("Mode LED Run");
    clkModeRun->addWidget(modeLedRun, 2, Qt::AlignHCenter);

    modeLedStep->setObjectName("Mode LED Step");
    clkModeStep->addWidget(modeLedStep, 2, Qt::AlignHCenter);

    runButton->setObjectName("Run Button");
    runButton->setMaximumHeight(15);
    clkModeRun->addWidget(runButton);

    stepButton->setObjectName("Step Button");
    stepButton->setMaximumHeight(15);
    clkModeStep->addWidget(stepButton);

    modeOptions->addLayout(clkModeRun);
    modeOptions->addLayout(clkModeStep);

    setLayout(modeOptions);


    // -- make the connections to the SR latch
    connect(this, &GUI_ClockModeGroup_t::SignalRunButtonChanged, clock, &HW_Clock_t::ProcessSetRunMode);
    connect(this, &GUI_ClockModeGroup_t::SignalStepButtonChanged, clock, &HW_Clock_t::ProcessSetStepMode);

    connect(runButton, &QPushButton::pressed, this, &GUI_ClockModeGroup_t::ProcessPressedRun);
    connect(runButton, &QPushButton::released, this, &GUI_ClockModeGroup_t::ProcessReleasedRun);
    connect(stepButton, &QPushButton::pressed, this, &GUI_ClockModeGroup_t::ProcessPressedStep);
    connect(stepButton, &QPushButton::released, this, &GUI_ClockModeGroup_t::ProcessReleasedStep);

    connect(clock, &HW_Clock_t::SignalRunMode, modeLedRun, &GUI_Led_t::ProcessStateChange);
    connect(clock, &HW_Clock_t::SignalStepMode, modeLedStep, &GUI_Led_t::ProcessStateChange);


    // -- have to cycle through this twice to make sure everything is updated properly
    clock->TriggerFirstUpdates();
    ProcessReleasedStep();
    ProcessReleasedRun();
}




