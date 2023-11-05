//===================================================================================================================
//  gui-clock-speed.cc -- This is a group box visualizing a control for the clock speed
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-01  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/gui-clock-speed.moc.cc"



//
// -- construct a new clock speed group box
//    -------------------------------------
GUI_ClockSpeed_t::GUI_ClockSpeed_t(HW_Oscillator_t *osc, QGroupBox *parent) : QGroupBox("Speed", parent), oscillator(osc)
{
    slider = new QSlider(Qt::Horizontal);
    label = new QLabel;

    slider->setMinimum(1);
    slider->setMaximum(10);
    slider->setFixedWidth(35);
    slider->setValue(5);

    setObjectName(tr("Clock Speed Group"));

    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    setFixedHeight(60);
    setMaximumWidth(50);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignHCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    slider->setObjectName("Step Speed");
    slider->setMaximumWidth(40);
    slider->setMaximumHeight(20);
    layout->addWidget(slider);

    label->setAlignment(Qt::AlignHCenter);
    label->setText("5");

    layout->addWidget(label);

    setLayout(layout);


    connect(slider, &QDial::valueChanged, this, &GUI_ClockSpeed_t::UpdateSpeed);
    connect(this, &GUI_ClockSpeed_t::SpeedChanged, osc, &HW_Oscillator_t::SetInterval);
}



//
// -- the value on the slider has changed; update the gui and subscribers
//    -----------------------------------------------------------------
void GUI_ClockSpeed_t::UpdateSpeed(int value)
{
    label->setText(QString::number(value));
    emit SpeedChanged((10 - value) * 100);
}

