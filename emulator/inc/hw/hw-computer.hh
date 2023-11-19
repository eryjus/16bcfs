//===================================================================================================================
//  hw-computer.hh -- This is the foundation of the computer build -- like a backplane
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#pragma once


//
// -- This is the foundation of the computer, as in the backplane or breadboard that everything plugs into
//    ----------------------------------------------------------------------------------------------------
class HW_Computer_t : public QObject {
    Q_OBJECT

private:
    // -- singleton instance
    static HW_Computer_t *singleton;

    // -- Start with an oscillator
    static HW_Oscillator_t *oscillator;
    static HW_Clock_t *clock;

    // -- ALU-A and ALU-B Input buses
    static HW_Bus_t *aluA;
    static HW_Bus_t *aluB;

    // -- The ALU
    static HW_Alu_t *alu;

    // -- The Main Bus
    static HW_Bus_t *mainBus;


    // -- Temporary elements for testing the ALU
    static HW_BusDriver_t *aluADriver;
    static HW_BusDriver_t *aluBDriver;


private:
    explicit HW_Computer_t(void) {};
    virtual ~HW_Computer_t() {};

    HW_Computer_t(const HW_Computer_t &) = delete;
    HW_Computer_t &operator=(const HW_Computer_t &) = delete;


public:
    static HW_Computer_t *Get(void) { if (singleton == nullptr) singleton = new HW_Computer_t; return singleton; }


public:
    static void Initialize(void);

    static HW_Oscillator_t *GetOscillator(void) { return oscillator; }
    static HW_Clock_t *GetClock(void) { return clock; }
    static HW_Bus_t *GetAluA(void) { return aluA; }
    static HW_Bus_t *GetAluB(void) { return aluB; }
    static HW_BusDriver_t *GetAluADriver(void) { return aluADriver; }
    static HW_BusDriver_t *GetAluBDriver(void) { return aluBDriver; }
    static HW_Alu_t *GetAlu(void) { return alu; }
    static HW_Bus_t *GetMainBus(void) { return mainBus; }


signals:
    void SignalOscillatorStateChanged(TriState_t state);
};


