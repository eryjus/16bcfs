//===================================================================================================================
//  hw-computer.hh -- This is the foundation of the computer build -- like a backplane
//
//      Copyright (c) 2023-2025 - Adam Clark
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
    static QString pgmRomFolder;


private:
    // -- singleton instance
    static HW_Computer_t *singleton;

    // -- here are the settings for the application
    static QSettings *settings;

    // -- The central widget
    static QWidget *central;

    // -- Insert the Control Logic Mid-Plane
    static ControlLogic_MidPlane_t *ctrlLogic;


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

    static FetchRegisterModule_t *fetch;
    static InstructionRegisterModule_t *instr;

    static PgmRomModule_t *pgmRom;

    static ResetModule_t *reset;


    // -- Reset Hold control Bus (1-bit with pull-up)
    static HW_Bus_1_t *rHld;
    static HW_Bus_1_t *cpyHld;

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

    // -- The fetch bus from memory to the fetch register components
    static HW_Bus_16_t *fetchBus;

    // -- The Insturuction Bus
    static HW_Bus_16_t *instrBus;

    // -- The Address Copy Bus
    static HW_Bus_16_t *AddrCopyBus;

    // -- Flags for Pgm and Int contexts
    static AluFlagsModule_t *pgmFlags;
    static AluFlagsModule_t *intFlags;

    // -- Temporary elements for testing the ALU
    static HW_BusDriver_t *aluADriver;
    static HW_BusDriver_t *aluBDriver;


private:
    explicit HW_Computer_t(void) {}
    virtual ~HW_Computer_t() {};

    HW_Computer_t(const HW_Computer_t &) = delete;
    HW_Computer_t &operator=(const HW_Computer_t &) = delete;


public:
    static HW_Computer_t *Get(void);

    static HW_Bus_1_t *GetRHldBus(void) { return rHld; }
    static HW_Bus_1_t *GetCpyHldBus(void) { return cpyHld; }
    static HW_Bus_16_t *GetMainBus(void) { return mainBus; }
    static HW_Bus_16_t *GetAluABus(void) { return aluA; }
    static HW_Bus_16_t *GetAluBBus(void) { return aluB; }
    static HW_Bus_16_t *GetAddr1Bus(void) { return addr1; }
    static HW_Bus_16_t *GetAddr2Bus(void) { return addr2; }
    static HW_Bus_16_t *GetInstrBus(void) { return instrBus; }
    static HW_Bus_16_t *GetFetchBus(void) { return fetchBus; }
    static ControlLogic_MidPlane_t *GetCtrlMidPlane(void) { return ctrlLogic; }
    static HW_Bus_16_t *GetAddrCopyBus(void) { return AddrCopyBus; }

    static ClockModule_t *GetClock(void) { return clock; }
    static HW_Alu_t *GetAlu(void) { return alu; }
    static AluFlagsModule_t *GetPgmFlags(void) { return pgmFlags; }
    static AluFlagsModule_t *GetIntFlags(void) { return intFlags; }
    static PgmRomModule_t *GetPgmRom(void) { return pgmRom; }

    static QSettings *GetSettings(void) { if (!settings) settings = new QSettings("eryjus", "16bcfs-emulator"); return settings; }

public:
    static const QString &GetPgmRomFolder(void) { return pgmRomFolder; }
    static void SetPgmRomFolder(const QString &f) { pgmRomFolder = f; }


public:
    static void PerformReset(void);
    static void Initialize(void);


signals:
    void SignalOscillatorStateChanged(TriState_t state);


private slots:
    void ProcessUpdateZLatch(int state);
    void ProcessUpdateCLatch(int state);
    void ProcessUpdateNVLLatch(int state);
    void ProcessSettingsWindow(void);


private:
    static void AllocateComponents(void);
    static void BuildGui(void);
    static void WireUp(void);
    static void FinalWireUp(void);
    static void TriggerFirstUpdate(void);
};




