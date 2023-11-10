//===================================================================================================================
//  hw-oscillator.hh -- This is a generic oscillator
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-28  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-oscillator.moc.cc"


//
// -- handle the construction of this instance
//    ----------------------------------------
HW_Oscillator_t::HW_Oscillator_t(int interval)
{
    state = LOW;
    ProcessSetInterval(500);
    connect(this, &QTimer::timeout, this, &HW_Oscillator_t::ProcessTimerStateChange);
}


