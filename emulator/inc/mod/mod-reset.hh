//===================================================================================================================
//  mod-reset.hh -- This class emulates the system-reset circuitry
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This class will be used to handle the system reset.  This will be done both on demand and on power-on (first
//  launch).  In real hardware, this will also trigger the Control ROM to copy and will wait for that to complete.
//  In the emulator, this module may or may not control that copy, depending on how the system is compiled.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- The Reset Module class
//    ----------------------
class ResetModule_t : public QGroupBox {
    Q_OBJECT



private:
    IC_74xx00_t *nand1;
    IC_74xx00_t *nand2;
    IC_74xx08_t *and1;
    IC_74xx32_t *or1;
    IC_74xx74_t *latch;
    IC_74xx193_t *counter;
    IC_74xx00_t *nand3;
    IC_74xx02_t *nor1;
    IC_74xx123_t *vib1;
    IC_74xx03_t *oNand1;


    GUI_Led_t *resetting;
    GUI_MomentarySwitch_t *reset;
    GUI_DipSwitch_t *bitA;
    GUI_DipSwitch_t *bitB;
    GUI_DipSwitch_t *bitC;
    GUI_DipSwitch_t *bitD;




// -- Inputs into this module from External sources
public slots:
    void ProcessHighSpeedClockLatch(TriState_t state);       // TODO: known as SYSCLK on the schematic
    void ProcessHighSpeedClockOutput(TriState_t state);       // TODO: known as SYSCLK on the schematic
    void ProcessCpuClockLatch(TriState_t state);
    void ProcessCpuClockOutput(TriState_t state);
    void PowerOnReset(void);

    void DebugReset(TriState_t state) { DEBUG << "Reset is " << state; }
    void DebugResetting(TriState_t state) { DEBUG << "Resetting is " << state; }
    void DebugClocking(TriState_t state) { DEBUG << "Clocking is " << state; }
    void DebugRunning(TriState_t state) { DEBUG << "Running is " << state; }

    void DebugRHLD(TriState_t state) { DEBUG << "#RHLD " << state; }
    void DebugCPYHLD(TriState_t state) { DEBUG << "#CPYHLD " << state; }
    void DebugRun(TriState_t state) { DEBUG << "Run State is " << state; }

    void DebugVibrator1(TriState_t state) { DEBUG << "Vibrator (Copy Complete) output is " << state; }
    void DebugVibrator2(TriState_t state) { DEBUG << "Vibrator (Reset Complete) output is " << state; }


    void DebugCounterLoad(TriState_t state) { DEBUG << "Counter Load " << state; }
    void DebugCounterBorrow(TriState_t state) { DEBUG << "Counter #BO " << state; }


// -- Outputs from this module to other destinations
signals:
    void SignalSystemClock(TriState_t state);           // TODO: known as the RESET_CLK on the schematic
    void SignalReset(TriState_t state);
    void SignalResetting(TriState_t state);
    void SignalClocking(TriState_t state);
    void SignalRunning(TriState_t state);



public:
    // -- constructor/destructor
    explicit ResetModule_t(void);
    virtual ~ResetModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void SetDebug(void);                    // set up the debugging triggers
    void WireUp(void);                      // make all the necessary connections
};


