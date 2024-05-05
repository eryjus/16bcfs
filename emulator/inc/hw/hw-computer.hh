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
    static GpRegisterModule_t *r1;
    static GpRegisterModule_t *r2;
    static GpRegisterModule_t *r3;
    static GpRegisterModule_t *r4;
    static GpRegisterModule_t *r5;
    static GpRegisterModule_t *r6;
    static GpRegisterModule_t *r7;
    static GpRegisterModule_t *r8;
    static GpRegisterModule_t *r9;
    static GpRegisterModule_t *r10;
    static GpRegisterModule_t *r11;
    static GpRegisterModule_t *r12;
    static GpRegisterModule_t *pgmpc;
    static GpRegisterModule_t *pgmra;
    static GpRegisterModule_t *pgmsp;
    static GpRegisterModule_t *intpc;
    static GpRegisterModule_t *intra;
    static GpRegisterModule_t *intsp;

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
    static CtrlRomModule_t *ctrl1;
    static CtrlRomModule_t *ctrl2;
    static CtrlRomModule_t *ctrl3;
    static CtrlRomModule_t *ctrl4;
    static CtrlRomModule_t *ctrl5;
    static CtrlRomModule_t *ctrl6;
    static CtrlRomModule_t *ctrl7;
    static CtrlRomModule_t *ctrl8;
    static CtrlRomModule_t *ctrl9;
    static CtrlRomModule_t *ctrla;
    static CtrlRomModule_t *ctrlb;
    static CtrlRomModule_t *ctrlc;
    static CtrlRomModule_t *ctrld;
    static CtrlRomModule_t *ctrle;
    static CtrlRomModule_t *ctrlf;


    // -- Temporary elements for testing the ALU
    static HW_BusDriver_t *aluADriver;
    static HW_BusDriver_t *aluBDriver;


    static HW_MomentarySwitch_t *brk;
    static HW_MomentarySwitch_t *rst;


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
    static CtrlRomModule_t *GetCtrl1(void) { return ctrl1; }
    static CtrlRomModule_t *GetCtrl2(void) { return ctrl2; }
    static CtrlRomModule_t *GetCtrl3(void) { return ctrl3; }
    static CtrlRomModule_t *GetCtrl4(void) { return ctrl4; }
    static CtrlRomModule_t *GetCtrl5(void) { return ctrl5; }
    static CtrlRomModule_t *GetCtrl6(void) { return ctrl6; }
    static CtrlRomModule_t *GetCtrl7(void) { return ctrl7; }
    static CtrlRomModule_t *GetCtrl8(void) { return ctrl8; }
    static CtrlRomModule_t *GetCtrl9(void) { return ctrl9; }
    static CtrlRomModule_t *GetCtrla(void) { return ctrla; }
    static CtrlRomModule_t *GetCtrlb(void) { return ctrlb; }
    static CtrlRomModule_t *GetCtrlc(void) { return ctrlc; }
    static CtrlRomModule_t *GetCtrld(void) { return ctrld; }
    static CtrlRomModule_t *GetCtrle(void) { return ctrle; }
    static CtrlRomModule_t *GetCtrlf(void) { return ctrlf; }


    static QSettings *GetSettings(void) { return settings; }


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


