//===================================================================================================================
//  hw-pot.hh -- This class implements a variable resistor or potentiometer
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a variable resistor or potentiometer
//    --------------------------------------------
class HW_Pot_t : public QSlider {
    Q_OBJECT


private:
    QLabel *label;


public:
    explicit HW_Pot_t(Qt::Orientation orient = Qt::Horizontal);
    virtual ~HW_Pot_t() {}


public:
    QLabel *GetLabel(void) const { return label; }
    void TriggerFirstUpdate(void) { emit SignalValueChanged(value()); }


signals:
    void SignalValueChanged(int v);


private slots:
    void ProcessUpdateValue(int v) { label->setText(QString::number(v)); emit SignalValueChanged(v); }
};
