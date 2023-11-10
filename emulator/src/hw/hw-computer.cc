//===================================================================================================================
//  hw-computer.cc -- This is the foundation of the computer build -- like a backplane or breadboard
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-computer.moc.cc"


//
// -- Initialize the computer by placing the components and hooking up the wires (signals/slots)
//    ------------------------------------------------------------------------------------------
void HW_Computer_t::Initialize(void)
{
    //
    // -- Place the oscillator
    //    --------------------
    oscillator = new HW_Oscillator_t;


    //
    // -- Add a 4-Phase Clock
    //    -------------------
    clk4Phase = new HW_Clock4Phase_t(oscillator);


    //
    // -- Place the tri-state latch
    //    ------------------------
    triStateLatch = new HW_TriStateLatch_t;


    //
    // -- Place the fictitious bus
    //    ------------------------
    bus = new HW_Bus_t(oscillator);
    driver = new HW_BusDriver_t(bus);

    connect(GetOscillator(), &HW_Oscillator_t::SignalStateChanged, this, &HW_Computer_t::SignalOscillatorStateChanged);
}


