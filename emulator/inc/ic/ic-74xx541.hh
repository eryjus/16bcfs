//===================================================================================================================
// ic-74xx541.hh -- This class implements a 74xx541 Line Driver
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-30  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This class implements a 74xx541 Line Driver
//    -------------------------------------------
class IC_74xx541_t : public QObject {
    Q_OBJECT


public:
    enum {OE1b = 1,
        D0 = 2,
        D1 = 3,
        D2 = 4,
        D3 = 5,
        D4 = 6,
        D5 = 7,
        D6 = 8,
        D7 = 9,
        Y7 = 11,
        Y6 = 12,
        Y5 = 13,
        Y4 = 14,
        Y3 = 15,
        Y2 = 16,
        Y1 = 17,
        Y0 = 18,
        OE2b = 19,
    };

private:
    TriState_t pins[PIN_CNT(20)];

public:
    IC_74xx541_t(void);
    virtual ~IC_74xx541_t() {};


public:
    void TriggerFirstUpdates(void) { UpdateOutputs(); }
    void UpdateOutputs(void);


public slots:
    void ProcessUpdateOE1(TriState_t state) { pins[OE1b] = state; UpdateOutputs(); }
    void ProcessUpdateOE2(TriState_t state) { pins[OE2b] = state; UpdateOutputs(); }
    void ProcessUpdateD0(TriState_t state) { pins[D0] = state; UpdateOutputs(); }
    void ProcessUpdateD1(TriState_t state) { pins[D1] = state; UpdateOutputs(); }
    void ProcessUpdateD2(TriState_t state) { pins[D2] = state; UpdateOutputs(); }
    void ProcessUpdateD3(TriState_t state) { pins[D3] = state; UpdateOutputs(); }
    void ProcessUpdateD4(TriState_t state) { pins[D4] = state; UpdateOutputs(); }
    void ProcessUpdateD5(TriState_t state) { pins[D5] = state; UpdateOutputs(); }
    void ProcessUpdateD6(TriState_t state) { pins[D6] = state; UpdateOutputs(); }
    void ProcessUpdateD7(TriState_t state) { pins[D7] = state; UpdateOutputs(); }


signals:
    void SignalY0Updated(TriState_t state);
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
    void SignalY5Updated(TriState_t state);
    void SignalY6Updated(TriState_t state);
    void SignalY7Updated(TriState_t state);
};



