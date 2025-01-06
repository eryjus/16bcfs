//===================================================================================================================
//  mod-clock.hh -- This class represents the entirety of the clock module
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct a clock module.  Ideally, this class would
//  correlate 1:1 against the Clock Module KiCad design and its resulting PCB.  The interface point will only be
//  the inputs from and outputs to the computer backplane.  All other connections in real hardware are represented
//  here.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-21  Initial  v0.0.1   Initial version
//  2024-Dec-25  Initial  v0.0.1   Update/Redo the Clock Module
//===================================================================================================================


#pragma once



//
// -- The Clock Module class.  Since this will have a visual component, it will be subclassed from QGroupBox
//    ------------------------------------------------------------------------------------------------------
class ClockModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- this module required the following components
    IC_74xx00_t *nand1;
    IC_74xx00_t *nand2;
    IC_74xx02_t *nor1;
    IC_74xx08_t *and1;
    IC_74xx08_t *and2;
    IC_74xx32_t *or1;
    IC_74xx74_t *latch1;
    IC_74xx193_t *divider1;



    HW_Oscillator_t *osc;   // -- This will be the high speed crystal
    HW_Oscillator_t *adj;   // -- This will be the main adjustable 555 timer
    HW_Pot_t *speedPot;
    GUI_MomentarySwitch_t *oscMomentary;
    GUI_MomentarySwitch_t *adjMomentary;
    GUI_MomentarySwitch_t *stepMomentary;
    HW_SpdtSwitch_t *singleStepDebounced;   // -- debounced with a 555
    GUI_Led_t *oscLed;
    GUI_Led_t *adjLed;
    GUI_Led_t *stepLed;
    GUI_Led_t *actualClock;
    GUI_DipSwitch_t *div0;
    GUI_DipSwitch_t *div1;
    GUI_DipSwitch_t *div2;
    GUI_DipSwitch_t *div3;



    // -- a clock counter for debugging
    unsigned long clockCount;



public slots:
    // -- Inputs into this module from External sources
    void ProcessSignalBreak(TriState_t state) { nor1->ProcessUpdateB4(state); }
    void ProcessReset(TriState_t state) { and1->ProcessUpdateB1(state); }
    void ProcessCpyHld(TriState_t state) { latch1->ProcessUpdateClr1(state); }


    void DebugOsc(TriState_t state) { qDebug() << "Oscillator" << state; }
    void DebugAdj(TriState_t state) { qDebug() << "Adjustable" << state; }

    void DebugReset(TriState_t state) { qDebug() << "Reset" << state; }
    void DebugAdjSel(TriState_t state) { qDebug() << "/RESET & AdjMoment" << state; }
    void DebugAdjSelOutput(TriState_t state) { qDebug() << "AdjSelection Output" << state; }
    void DebugAdjClockTick(TriState_t state) { qDebug() << "AdjClockTick" << state; }

    void DebugLatchClock(TriState_t state) { qDebug() << "Latch Clock" << state; }
    void DebugLatchDataIn(TriState_t state) { qDebug() << "Latch Data" << state; }
    void DebugLatchDataOut(TriState_t state) { qDebug() << "Latch Output" << state; }

    void DebugCpuClock(TriState_t state) { qDebug() << "CPU Clock" << state; }

    void DebugDiv0(TriState_t state) { qDebug() << "Div0" << state; }
    void DebugDiv1(TriState_t state) { qDebug() << "Div1" << state; }
    void DebugDiv2(TriState_t state) { qDebug() << "Div2" << state; }
    void DebugDiv3(TriState_t state) { qDebug() << "Div3" << state; }

    void DebugBo(TriState_t state) { qDebug() << "BO" << state; }


signals:
    // -- Outputs from this module to other destinations
    void SignalHighSpeedClockLatch(TriState_t state);
    void SignalHighSpeedClockOutput(TriState_t state);
    void SignalCpuClockLatch(TriState_t state);
    void SignalCpuClockOutput(TriState_t state);



    void SignalSanityCheck(void);               // not a part of the physical build


public:
    // -- constructor/destructor
    explicit ClockModule_t(void);
    virtual ~ClockModule_t() {}



public:
    void StartClock(void);



public:
    unsigned long GetClockCount(void) const { return clockCount; }



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
    void SetDebug(void);                    // set up all the debug connections
    void TriggerFirstUpdate(void);          // trigger all the proper initial updates



private slots:
    // -- used for internal signaling
    void IncrementClockCount(TriState_t state) { if (state == HIGH) clockCount ++; }
    void ProcessHighSpeedClock(TriState_t state) {
        emit SignalHighSpeedClockLatch(state);
        emit SignalHighSpeedClockOutput(state);
    }
    void ProcessCpuClock(TriState_t state) {
        emit SignalCpuClockLatch(state);
        emit SignalCpuClockOutput(state);
    }
};

