//===================================================================================================================
//  hw-bus-8.hh -- This is a generic 8-bit bus
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- Handle multiple connections into a bus
//    --------------------------------------
class HW_Bus_8_t : public QObject {
    Q_OBJECT


public:
    enum { BIT_0 = 0,
        BIT_1 = 1,
        BIT_2 = 2,
        BIT_3 = 3,
        BIT_4 = 4,
        BIT_5 = 5,
        BIT_6 = 6,
        BIT_7 = 7,
    };


private:
    typedef QMap<QObject *, TriState_t> Asserts_t;
    typedef QMap<int, Asserts_t *> Map_t;
    Map_t *assertedBits;


public:
    HW_Bus_8_t(const QString &name, ClockModule_t *clk, QObject *parent = nullptr);
    virtual ~HW_Bus_8_t() {}


private:
    void MaintainBit(int bit, TriState_t state);


public slots:
    void ProcessUpdateBit0(TriState_t state) { MaintainBit(BIT_0, state); }
    void ProcessUpdateBit1(TriState_t state) { MaintainBit(BIT_1, state); }
    void ProcessUpdateBit2(TriState_t state) { MaintainBit(BIT_2, state); }
    void ProcessUpdateBit3(TriState_t state) { MaintainBit(BIT_3, state); }
    void ProcessUpdateBit4(TriState_t state) { MaintainBit(BIT_4, state); }
    void ProcessUpdateBit5(TriState_t state) { MaintainBit(BIT_5, state); }
    void ProcessUpdateBit6(TriState_t state) { MaintainBit(BIT_6, state); }
    void ProcessUpdateBit7(TriState_t state) { MaintainBit(BIT_7, state); }

    void ProcessSanityCheck(void);


signals:
    void SignalBit0Updated(TriState_t state);
    void SignalBit1Updated(TriState_t state);
    void SignalBit2Updated(TriState_t state);
    void SignalBit3Updated(TriState_t state);
    void SignalBit4Updated(TriState_t state);
    void SignalBit5Updated(TriState_t state);
    void SignalBit6Updated(TriState_t state);
    void SignalBit7Updated(TriState_t state);
};


