//===================================================================================================================
//  gui-step-group.cc -- This group box visualizes a momentary switch for single stepping
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-01  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-step-group.moc.cc"


//
// -- Construct a new Step Button Group
//    ---------------------------------
GUI_StepGroup_t::GUI_StepGroup_t(HW_Clock4Phase_t *clock, QWidget *parent) : QGroupBox("Step", parent)
{
    step = new QPushButton(tr("Step"));

    setObjectName(tr("Clock Step Group"));

    setAlignment(Qt::AlignHCenter);
    setFixedHeight(60);
    setMinimumHeight(60);
    setMaximumHeight(60);
    setMaximumWidth(50);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    step->setObjectName("Step Button");
    step->setMaximumWidth(40);
    step->setMaximumHeight(20);
    layout->addWidget(step);

    setLayout(layout);

    connect(step, &QPushButton::pressed, this, &GUI_StepGroup_t::PressedStep);
    connect(step, &QPushButton::released, this, &GUI_StepGroup_t::ReleasedStep);

    connect(this, &GUI_StepGroup_t::StepButtonChanged, clock->GetClkControlOr(), &IC_74xx32_t::UpdateB1);
    connect(this, &GUI_StepGroup_t::StepButtonChanged, clock->GetClkControlAnd(), &IC_74xx08_t::UpdateB4);

    emit StepButtonChanged(LOW);
}


