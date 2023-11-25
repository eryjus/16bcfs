//===================================================================================================================
//  mod-clock.hh -- This class represents the entirety of the clock module
//
//      Copyright (c) 2023 - Adam Clark
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
//===================================================================================================================


#pragma once



//
// -- The Clock Module class.  Since this will have a visual component, it will be subclassed from QGroupBox
//    ------------------------------------------------------------------------------------------------------
class ClockModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- this module required the following components
    HW_Oscillator_t *timer;
    QLabel *oscillatorVisual;               // -- could be implemented as an LED
    HW_Pot_t *speedPot;
    HW_MomentarySwitch_t *runModeMomentary;
    HW_MomentarySwitch_t *stepModeMomentary;
    HW_SpdtSwitch_t *singleStepDebounced;
    GUI_Led_t *runModeLed;
    GUI_Led_t *stepModeLed;
    GUI_Led_t *actualClock;
    IC_74xx00_t *nand1;
    IC_74xx08_t *and1;
    IC_74xx32_t *or1;
    IC_74xx74_t *latch;


    // -- these are used internally
    QPixmap hi;
    QPixmap lo;


public slots:
    // -- these functions become the external inputs into this module from the backplane
    void ProcessBreak(TriState_t state);
    void ProcessReset(TriState_t state);


signals:
    // -- these functions become the outputs from this module to other parts of the build
    void SignalClockState(TriState_t state);


public:
    // -- constructor/destructor
    explicit ClockModule_t(void);
    virtual ~ClockModule_t() {}


private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates


private slots:
    // -- used for internal signaling
    void ProcessUpdateImage(TriState_t state)  { oscillatorVisual->setPixmap(state?hi:lo); }
};

