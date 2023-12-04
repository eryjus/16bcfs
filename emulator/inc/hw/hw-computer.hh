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

    // -- The General Purpose Register Modules
    static GpRegisterModule_t *pc;

    // -- singleton instance
    static HW_Computer_t *singleton;

    // -- ALU-A and ALU-B Input buses
    static HW_Bus_t *aluA;
    static HW_Bus_t *aluB;

    // -- Address Bus 1 and 2
    static HW_Bus_t *addr1;
    static HW_Bus_t *addr2;

    // -- The ALU
    static HW_Alu_t *alu;

    // -- The Main Bus
    static HW_Bus_t *mainBus;


    // -- Temporary elements for testing the ALU
    static HW_BusDriver_t *aluADriver;
    static HW_BusDriver_t *aluBDriver;


    static HW_MomentarySwitch_t *brk;
    static HW_MomentarySwitch_t *rst;


private:
    explicit HW_Computer_t(void) {};
    virtual ~HW_Computer_t() {};

    HW_Computer_t(const HW_Computer_t &) = delete;
    HW_Computer_t &operator=(const HW_Computer_t &) = delete;


public:
    static HW_Computer_t *Get(void);

    static HW_Bus_t *GetMainBus(void) { return mainBus; }
    static HW_Bus_t *GetAluABus(void) { return aluA; }
    static HW_Bus_t *GetAluBBus(void) { return aluB; }
    static HW_Bus_t *GetAddr1Bus(void) { return addr1; }
    static HW_Bus_t *GetAddr2Bus(void) { return addr2; }

    static ClockModule_t *GetClock(void) { return clock; }

    static QWidget *BuildTestHarness(void);


public:
    static void InitGui(void);
    static void Initialize(void);


signals:
    void SignalOscillatorStateChanged(TriState_t state);


private slots:
    void ProcessToggleButton(int id, bool checked);
};


