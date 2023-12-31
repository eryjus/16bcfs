//===================================================================================================================
//  gui-led.hh -- A label visualizing an led
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-25  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is an oscillator visualization, changing states on a timer signal
//    ----------------------------------------------------------------------
class GUI_Led_t : public QLabel {
    Q_OBJECT

public:
    typedef enum {
        OnWhenLow = 0,
        OnWhenHigh = 1,
    } LedWiring_t;

public:
    explicit GUI_Led_t(LedWiring_t wire = OnWhenHigh, Qt::GlobalColor on = Qt::green, QWidget *parent = nullptr);
    explicit GUI_Led_t(QWidget *parent, LedWiring_t wire = OnWhenHigh, Qt::GlobalColor on = Qt::green);
    virtual ~GUI_Led_t() {}


public:
    virtual QSize sizeHint(void) const override { return QSize(5, 8); }


public slots:
    void ProcessStateChange(TriState_t state) { setPalette(!(state^onWhen)?onColor:offColor); }

private:
    QPalette onColor;
    static QPalette offColor;
    LedWiring_t onWhen;
};
