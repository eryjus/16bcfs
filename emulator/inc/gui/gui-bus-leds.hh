//===================================================================================================================
//  gui-bus-leds.hh -- This is a group box visualizing a bus contents
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-01  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is a bus LED visualization and optional tester
//    ---------------------------------------------------
class GUI_BusLeds_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_Led_t *ledF;
    GUI_Led_t *ledE;
    GUI_Led_t *ledD;
    GUI_Led_t *ledC;
    GUI_Led_t *ledB;
    GUI_Led_t *ledA;
    GUI_Led_t *led9;
    GUI_Led_t *led8;
    GUI_Led_t *led7;
    GUI_Led_t *led6;
    GUI_Led_t *led5;
    GUI_Led_t *led4;
    GUI_Led_t *led3;
    GUI_Led_t *led2;
    GUI_Led_t *led1;
    GUI_Led_t *led0;


public:
    GUI_BusLeds_t(QString name, HW_Bus_t *bus, QWidget *parent = nullptr);
    ~GUI_BusLeds_t() {}
};

