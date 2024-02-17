//===================================================================================================================
//  hw-computer.hh -- This is the foundation of the computer build -- like a backplane
//
//      Copyright (c) 2023-2024 - Adam Clark
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
class HW_Computer_t : public QMainWindow {
    Q_OBJECT

private:
    // -- singleton instance
    static HW_Computer_t *singleton;

    // -- here are the settings for the application
    static QSettings *settings;

    // -- The central widget
    static QWidget *central;

    // -- The computer modules
    static ClockModule_t *clock;

    // -- The General Purpose Register Modules
    static GpRegisterModule_t *pc;

    // -- Reset Hold control Bus (1-bit with pull-up)
    static HW_Bus_1_t *rHld;

    // -- ALU-A and ALU-B Input buses
    static HW_Bus_16_t *aluA;
    static HW_Bus_16_t *aluB;

    // -- Address Bus 1 and 2
    static HW_Bus_16_t *addr1;
    static HW_Bus_16_t *addr2;

    // -- The ALU
    static HW_Alu_t *alu;

    // -- The Main Bus
    static HW_Bus_16_t *mainBus;

    // -- The Insturuction Bus
    static HW_Bus_16_t *instrBus;

    // -- The control logic lookup bus
    static HW_Bus_16_t *ctrlBus;

    // -- The Address Copy Bus
    static HW_Bus_16_t *AddrCopyBus;

    // -- Flags for Pgm and Int contexts
    static AluFlagsModule_t *pgmFlags;
    static AluFlagsModule_t *intFlags;

    // -- the Control ROMS
    static CtrlRomCtrlModule_t *ctrlCtrl;
    static CtrlRomModule_t *ctrl0;


    static IC_25lc256_t *ctrl1;
    static IC_25lc256_t *ctrl2;
    static IC_25lc256_t *ctrl3;
    static IC_25lc256_t *ctrl4;
    static IC_25lc256_t *ctrl5;
    static IC_25lc256_t *ctrl6;
    static IC_25lc256_t *ctrl7;
    static IC_25lc256_t *ctrl8;
    static IC_25lc256_t *ctrl9;
    static IC_25lc256_t *ctrla;
    static IC_25lc256_t *ctrlb;

    // -- the Parallel Control RAMS
    static IC_AS6C62256_t *ctrl1Ram;
    static IC_AS6C62256_t *ctrl2Ram;
    static IC_AS6C62256_t *ctrl3Ram;
    static IC_AS6C62256_t *ctrl4Ram;
    static IC_AS6C62256_t *ctrl5Ram;
    static IC_AS6C62256_t *ctrl6Ram;
    static IC_AS6C62256_t *ctrl7Ram;
    static IC_AS6C62256_t *ctrl8Ram;
    static IC_AS6C62256_t *ctrl9Ram;
    static IC_AS6C62256_t *ctrlaRam;
    static IC_AS6C62256_t *ctrlbRam;


    // -- Temporary elements for testing the ALU
    static HW_BusDriver_t *aluADriver;
    static HW_BusDriver_t *aluBDriver;


    static HW_MomentarySwitch_t *brk;
    static HW_MomentarySwitch_t *rst;
    static HW_MomentarySwitch_t *clc;
    static HW_MomentarySwitch_t *stc;
    static HW_MomentarySwitch_t *clv;
    static HW_MomentarySwitch_t *stv;


private:
    explicit HW_Computer_t(void) { settings = new QSettings("eryjus", "16bcfs-emulator"); };
    virtual ~HW_Computer_t() {};

    HW_Computer_t(const HW_Computer_t &) = delete;
    HW_Computer_t &operator=(const HW_Computer_t &) = delete;


public:
    static HW_Computer_t *Get(void);

    static HW_Bus_1_t *GetRhldBus(void) { return rHld; }
    static HW_Bus_16_t *GetMainBus(void) { return mainBus; }
    static HW_Bus_16_t *GetAluABus(void) { return aluA; }
    static HW_Bus_16_t *GetAluBBus(void) { return aluB; }
    static HW_Bus_16_t *GetAddr1Bus(void) { return addr1; }
    static HW_Bus_16_t *GetAddr2Bus(void) { return addr2; }
    static HW_Bus_16_t *GetInstrBus(void) { return instrBus; }
    static HW_Bus_16_t *GetCtrlBus(void) { return ctrlBus; }
    static HW_Bus_16_t *GetAddrCopyBus(void) { return AddrCopyBus; }

    static ClockModule_t *GetClock(void) { return clock; }
    static HW_Alu_t *GetAlu(void) { return alu; }
    static AluFlagsModule_t *GetPgmFlags(void) { return pgmFlags; }
    static AluFlagsModule_t *GetIntFlags(void) { return intFlags; }

    static CtrlRomCtrlModule_t *GetCtrlCtrl(void) { return ctrlCtrl; }
    static CtrlRomModule_t *GetCtrl0(void) { return ctrl0; }


    static QSettings *GetSettings(void) { return settings; }
    static QWidget *BuildTestHarness(void);


public:
    static void InitGui(void);
    static void Initialize(void);
    static void PerformReset(void);


signals:
    void SignalOscillatorStateChanged(TriState_t state);


private slots:
    void ProcessToggleButton(int id, bool checked);
    void ProcessUpdateZLatch(int state);
    void ProcessUpdateCLatch(int state);
    void ProcessUpdateNVLLatch(int state);
    void ProcessSettingsWindow(void);
};


