//===================================================================================================================
//  gui-clock-function-group.cc -- This is a group box for selecting which clock function
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-clock-function-group.moc.cc"


//
// -- Construct the Group box to hold the current clock mode buttons and current state
//    --------------------------------------------------------------------------------
GUI_ClockFunctionGroup_t::GUI_ClockFunctionGroup_t(HW_TriStateLatch_t *latch, HW_Clock4Phase_t *clock, QWidget *parent)
        : QGroupBox("Clock Mode", parent)
{
    tristateLedRun = new GUI_Led_t;
    tristateLedInstr = new GUI_Led_t;
    tristateLedCycle = new GUI_Led_t;

    tristateRunButton = new QPushButton("Run");
    tristateInstrButton = new QPushButton("Instr");
    tristateCycleButton = new QPushButton("Cycle");

    setObjectName(tr("Clock Function Group"));

    setAlignment(Qt::AlignHCenter);
    setFixedWidth(120);
    setFixedHeight(60);

    QHBoxLayout *functionOptions = new QHBoxLayout();
    functionOptions->setAlignment(Qt::AlignHCenter);
    functionOptions->setContentsMargins(0, 0, 0, 0);
    functionOptions->setSpacing(2);

    QVBoxLayout *clkFunctionRun = new QVBoxLayout;
    clkFunctionRun->setAlignment(Qt::AlignHCenter);
    clkFunctionRun->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *clkFunctionInstr = new QVBoxLayout;
    clkFunctionInstr->setAlignment(Qt::AlignHCenter);
    clkFunctionInstr->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *clkFunctionCycle = new QVBoxLayout;
    clkFunctionCycle->setAlignment(Qt::AlignHCenter);
    clkFunctionCycle->setContentsMargins(0, 0, 0, 0);

    tristateLedRun->setObjectName("TriState LED1");
    clkFunctionRun->addWidget(tristateLedRun, 2, Qt::AlignHCenter);

    tristateLedInstr->setObjectName("TriState LED2");
    clkFunctionInstr->addWidget(tristateLedInstr, 2, Qt::AlignHCenter);

    tristateLedCycle->setObjectName("TriState LED3");
    clkFunctionCycle->addWidget(tristateLedCycle, 2, Qt::AlignHCenter);

    tristateRunButton->setObjectName("Run Button");
    tristateRunButton->setMaximumHeight(15);
    clkFunctionRun->addWidget(tristateRunButton);

    tristateInstrButton->setObjectName("Instr Button");
    tristateInstrButton->setMaximumHeight(15);
    clkFunctionInstr->addWidget(tristateInstrButton);

    tristateCycleButton->setObjectName("Cycle Button");
    tristateCycleButton->setMaximumHeight(15);
    clkFunctionCycle->addWidget(tristateCycleButton);

    functionOptions->addLayout(clkFunctionRun);
    functionOptions->addLayout(clkFunctionInstr);
    functionOptions->addLayout(clkFunctionCycle);

    setLayout(functionOptions);

    connect(tristateRunButton, &QPushButton::pressed, this, &GUI_ClockFunctionGroup_t::ProcessPressedRun);
    connect(tristateRunButton, &QPushButton::released, this, &GUI_ClockFunctionGroup_t::ProcessReleasedRun);
    connect(tristateInstrButton, &QPushButton::pressed, this, &GUI_ClockFunctionGroup_t::ProcessPressedInstr);
    connect(tristateInstrButton, &QPushButton::released, this, &GUI_ClockFunctionGroup_t::ProcessReleasedInstr);
    connect(tristateCycleButton, &QPushButton::pressed, this, &GUI_ClockFunctionGroup_t::ProcessPressedCycle);
    connect(tristateCycleButton, &QPushButton::released, this, &GUI_ClockFunctionGroup_t::ProcessReleasedCycle);

    connect(this, &GUI_ClockFunctionGroup_t::SignalRunButtonChanged, latch->GetTristateNand1(), &IC_74xx00_t::ProcessUpdateA1);
    connect(this, &GUI_ClockFunctionGroup_t::SignalInstrButtonChanged, latch->GetTristateNand1(), &IC_74xx00_t::ProcessUpdateA3);
    connect(this, &GUI_ClockFunctionGroup_t::SignalCycleButtonChanged, latch->GetTristateNand2(), &IC_74xx00_t::ProcessUpdateA1);

    connect(latch, &HW_TriStateLatch_t::SignalQ1Changed, tristateLedRun, &GUI_Led_t::ProcessStateChange);
    connect(latch, &HW_TriStateLatch_t::SignalQ2Changed, tristateLedInstr, &GUI_Led_t::ProcessStateChange);
    connect(latch, &HW_TriStateLatch_t::SignalQ3Changed, tristateLedCycle, &GUI_Led_t::ProcessStateChange);

    // -- connect the latches to the control inputs
    connect(latch, &HW_TriStateLatch_t::SignalQ1Changed, clock->GetClkControlLatch(), &IC_74xx574_t::ProcessUpdateD1);
    connect(latch, &HW_TriStateLatch_t::SignalQ2Changed, clock->GetClkControlLatch(), &IC_74xx574_t::ProcessUpdateD2);
    connect(latch, &HW_TriStateLatch_t::SignalQ3Changed, clock->GetClkControlLatch(), &IC_74xx574_t::ProcessUpdateD3);


    tristateRunButton->pressed();
    tristateRunButton->released();
}


