//===================================================================================================================
//  hw-bus-1.hh -- This is a generic 1-bit bus
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Feb-12  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- Handle multiple connections into a bus
//    --------------------------------------
class HW_Bus_1_t : public QObject {
    Q_OBJECT


private:
    typedef QMap<QObject *, TriState_t> Asserts_t;
    typedef QMap<int, Asserts_t *> Map_t;
    Map_t *assertedBits;


public:
    HW_Bus_1_t(const QString &name, ClockModule_t *clk, QObject *parent = nullptr);
    virtual ~HW_Bus_1_t() {}


private:
    void MaintainBit(TriState_t state);


public slots:
    void ProcessUpdateBit0(TriState_t state) { qDebug() << sender()->objectName() << state; MaintainBit(state); }

    void ProcessSanityCheck(void);
    void TriggerFirstUpdate(void) { MaintainBit(Z); }


signals:
    void SignalBit0Updated(TriState_t state);
};


