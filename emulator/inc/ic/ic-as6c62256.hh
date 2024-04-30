//===================================================================================================================
// ic-as5lc62256.hh -- This header file defines an AS6C62256 Parallel SRAM IC
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-17  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This is the class which will emulate the SRAM
//    ---------------------------------------------
class IC_AS6C62256_t : public QObject {
    Q_OBJECT



public:
    enum {
        A14 = 1,
        A12 = 2,
        A7  = 3,
        A6  = 4,
        A5  = 5,
        A4  = 6,
        A3  = 7,
        A2  = 8,
        A1  = 9,
        A0  = 10,
        DQ0 = 11,
        DQ1 = 12,
        DQ2 = 13,
        DQ3 = 15,
        DQ4 = 16,
        DQ5 = 17,
        DQ6 = 18,
        DQ7 = 19,
        CEb = 20,
        A10 = 21,
        OEb = 22,
        A11 = 23,
        A9  = 24,
        A8  = 25,
        A13 = 26,
        WEb = 27,
    };


private:
    IC_25lc256_t *reference;
    bool outputting;
    uint8_t outputValue;


private:
    TriState_t pins[PIN_CNT(28)];
    uint8_t contents[32*1024];  // this is the RAM contents



public:
    IC_AS6C62256_t(IC_25lc256_t *sanity = nullptr);
    virtual ~IC_AS6C62256_t() {}



public:
    void TriggerFirstUpdates(void);



private:
    void ProcessInput(void);
    void ProcessOutput(void);
    void UpdateAll(void);
    void OutputZ(void);



public slots:
    void ProcessUpdateA0(TriState_t state)  { pins[A0] = state;  UpdateAll(); }
    void ProcessUpdateA1(TriState_t state)  { pins[A1] = state;  UpdateAll(); }
    void ProcessUpdateA2(TriState_t state)  { pins[A2] = state;  UpdateAll(); }
    void ProcessUpdateA3(TriState_t state)  { pins[A3] = state;  UpdateAll(); }
    void ProcessUpdateA4(TriState_t state)  { pins[A4] = state;  UpdateAll(); }
    void ProcessUpdateA5(TriState_t state)  { pins[A5] = state;  UpdateAll(); }
    void ProcessUpdateA6(TriState_t state)  { pins[A6] = state;  UpdateAll(); }
    void ProcessUpdateA7(TriState_t state)  { pins[A7] = state;  UpdateAll(); }
    void ProcessUpdateA8(TriState_t state)  { pins[A8] = state;  UpdateAll(); }
    void ProcessUpdateA9(TriState_t state)  { pins[A9] = state;  UpdateAll(); }
    void ProcessUpdateA10(TriState_t state) { pins[A10] = state; UpdateAll(); }
    void ProcessUpdateA11(TriState_t state) { pins[A11] = state; UpdateAll(); }
    void ProcessUpdateA12(TriState_t state) { pins[A12] = state; UpdateAll(); }
    void ProcessUpdateA13(TriState_t state) { pins[A13] = state; UpdateAll(); }
    void ProcessUpdateA14(TriState_t state) { pins[A14] = state; UpdateAll(); }

    void ProcessUpdateCE(TriState_t state);
    void ProcessUpdateOE(TriState_t state);
    void ProcessUpdateWE(TriState_t state);

    void ProcessUpdateDq0(TriState_t state)  { pins[DQ0] = state; UpdateAll(); }
    void ProcessUpdateDq1(TriState_t state)  { pins[DQ1] = state; UpdateAll(); }
    void ProcessUpdateDq2(TriState_t state)  { pins[DQ2] = state; UpdateAll(); }
    void ProcessUpdateDq3(TriState_t state)  { pins[DQ3] = state; UpdateAll(); }
    void ProcessUpdateDq4(TriState_t state)  { pins[DQ4] = state; UpdateAll(); }
    void ProcessUpdateDq5(TriState_t state)  { pins[DQ5] = state; UpdateAll(); }
    void ProcessUpdateDq6(TriState_t state)  { pins[DQ6] = state; UpdateAll(); }
    void ProcessUpdateDq7(TriState_t state)  { pins[DQ7] = state; UpdateAll(); }


    void ProcessSanityCheck(void);
    void CopyEeprom(void);


signals:
    void SignalDq0Updated(TriState_t state);
    void SignalDq1Updated(TriState_t state);
    void SignalDq2Updated(TriState_t state);
    void SignalDq3Updated(TriState_t state);
    void SignalDq4Updated(TriState_t state);
    void SignalDq5Updated(TriState_t state);
    void SignalDq6Updated(TriState_t state);
    void SignalDq7Updated(TriState_t state);
};

