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
class HW_Computer_t : public QWidget {
    Q_OBJECT

private:
    // -- The computer modules
    static ClockModule_t *clock;

    // -- singleton instance
    static HW_Computer_t *singleton;

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
    static HW_Computer_t *Get(void);


public:
    static void InitGui(void);
    static void Initialize(void);


signals:
    void SignalOscillatorStateChanged(TriState_t state);
};


