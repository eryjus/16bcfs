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
    // -- Start with an oscillator
    HW_Oscillator_t *oscillator;

    // -- Create the elements of the 4-cycle clock
    HW_Clock4Phase_t *clk4Phase;

    // -- Tristate Latch
    HW_TriStateLatch_t *triStateLatch;

    // -- Fictitious Bus for Testing
    HW_Bus_t *bus;
    HW_BusDriver_t *driver;

public:
    explicit HW_Computer_t(void) {};
    virtual ~HW_Computer_t() {};

public:
    void Initialize(void);

    HW_Oscillator_t *GetOscillator(void) const { return oscillator; }
    HW_Clock4Phase_t *Get4PhaseClock(void) const { return clk4Phase; }
    HW_TriStateLatch_t *GetTriStateLatch(void) const { return triStateLatch; }
    HW_Bus_t *GetFictitousBus(void) const { return bus; }
    HW_BusDriver_t *GetBusDriver(void) const { return driver; }


signals:
    void SignalOscillatorStateChanged(TriState_t state);
};


