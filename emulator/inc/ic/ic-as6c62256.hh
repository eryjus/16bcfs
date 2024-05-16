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
    enum {
        ENAB_ALL    = 0b000,
        ENAB_CE_WE  = 0b001,
        ENAB_CE_OE  = 0b010,
        ENAB_CE     = 0b011,
        ENAB_WE_OE  = 0b100,
        ENAB_WE     = 0b101,
        ENAB_OE     = 0b110,
        ENAB_NONE   = 0b111,
    };

private:
    IC_25lc256_t *reference;
    bool outputting;
    uint8_t outputValue;


private:
    TriState_t pins[PIN_CNT(28)];
    uint8_t contents[32*1024];  // this is the RAM contents


    // -- These hold the incoming value in case an update is needed later
    TriState_t hold0;
    TriState_t hold1;
    TriState_t hold2;
    TriState_t hold3;
    TriState_t hold4;
    TriState_t hold5;
    TriState_t hold6;
    TriState_t hold7;

    TriState_t lastCE;
    TriState_t lastWE;
    TriState_t lastOE;

    bool updating;



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

    bool StatusChange(void) { return (pins[CEb] != lastCE) || (pins[WEb] != lastWE) || (pins[OEb] != lastOE); }



public slots:
    void ProcessUpdateA0(TriState_t state)  { pins[ A0] = state; UpdateAll(); }
    void ProcessUpdateA1(TriState_t state)  { pins[ A1] = state; UpdateAll(); }
    void ProcessUpdateA2(TriState_t state)  { pins[ A2] = state; UpdateAll(); }
    void ProcessUpdateA3(TriState_t state)  { pins[ A3] = state; UpdateAll(); }
    void ProcessUpdateA4(TriState_t state)  { pins[ A4] = state; UpdateAll(); }
    void ProcessUpdateA5(TriState_t state)  { pins[ A5] = state; UpdateAll(); }
    void ProcessUpdateA6(TriState_t state)  { pins[ A6] = state; UpdateAll(); }
    void ProcessUpdateA7(TriState_t state)  { pins[ A7] = state; UpdateAll(); }
    void ProcessUpdateA8(TriState_t state)  { pins[ A8] = state; UpdateAll(); }
    void ProcessUpdateA9(TriState_t state)  { pins[ A9] = state; UpdateAll(); }
    void ProcessUpdateA10(TriState_t state) { pins[A10] = state; UpdateAll(); }
    void ProcessUpdateA11(TriState_t state) { pins[A11] = state; UpdateAll(); }
    void ProcessUpdateA12(TriState_t state) { pins[A12] = state; UpdateAll(); }
    void ProcessUpdateA13(TriState_t state) { pins[A13] = state; UpdateAll(); }
    void ProcessUpdateA14(TriState_t state) { pins[A14] = state; UpdateAll(); }

    void ProcessUpdateCE(TriState_t state) { pins[CEb] = state; UpdateAll(); }
    void ProcessUpdateOE(TriState_t state) { pins[OEb] = state; UpdateAll(); }
    void ProcessUpdateWE(TriState_t state) { pins[WEb] = state; UpdateAll(); }

    void ProcessUpdateDq0(TriState_t state)  { hold0 = state; UpdateAll(); }
    void ProcessUpdateDq1(TriState_t state)  { hold1 = state; UpdateAll(); }
    void ProcessUpdateDq2(TriState_t state)  { hold2 = state; UpdateAll(); }
    void ProcessUpdateDq3(TriState_t state)  { hold3 = state; UpdateAll(); }
    void ProcessUpdateDq4(TriState_t state)  { hold4 = state; UpdateAll(); }
    void ProcessUpdateDq5(TriState_t state)  { hold5 = state; UpdateAll(); }
    void ProcessUpdateDq6(TriState_t state)  { hold6 = state; UpdateAll(); }
    void ProcessUpdateDq7(TriState_t state)  { hold7 = state; UpdateAll(); }


    void ProcessSanityCheck(QString name);
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

