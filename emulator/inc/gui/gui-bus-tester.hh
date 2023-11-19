//===================================================================================================================
//  gui-bus-tester.hh -- This is a generic bus tester
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- Test multiple connections into a bus
//    ------------------------------------
class GUI_BusTester_t : public QGroupBox {
    Q_OBJECT


private:
    GUI_DipSwitch_t *dipF;
    GUI_DipSwitch_t *dipE;
    GUI_DipSwitch_t *dipD;
    GUI_DipSwitch_t *dipC;
    GUI_DipSwitch_t *dipB;
    GUI_DipSwitch_t *dipA;
    GUI_DipSwitch_t *dip9;
    GUI_DipSwitch_t *dip8;
    GUI_DipSwitch_t *dip7;
    GUI_DipSwitch_t *dip6;
    GUI_DipSwitch_t *dip5;
    GUI_DipSwitch_t *dip4;
    GUI_DipSwitch_t *dip3;
    GUI_DipSwitch_t *dip2;
    GUI_DipSwitch_t *dip1;
    GUI_DipSwitch_t *dip0;

    QCheckBox *oe;


public:
    GUI_BusTester_t(QString name, HW_BusDriver_t *driver, QGroupBox *parent = nullptr);
    virtual ~GUI_BusTester_t() {}


public slots:
    void ProcessOEChange(int val) { if (val == Qt::Unchecked) emit SignalOeUpdated(HIGH); else emit SignalOeUpdated(LOW); }


signals:
    void SignalOeUpdated(TriState_t state);
};


