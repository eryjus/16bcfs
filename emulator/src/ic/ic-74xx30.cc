//===================================================================================================================
// ic-74xx30.cc -- This file implementes the 74xx30 8-Input NAND Gate IC
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-27  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx30.moc.cc"


//
// -- This is the contructor for the NAND Gate IC, which has 14 pins
//    --------------------------------------------------------------
IC_74xx30_t::IC_74xx30_t(void)
{
    pins[A] = LOW;
    pins[B] = LOW;
    pins[C] = LOW;
    pins[D] = LOW;
    pins[E] = LOW;
    pins[F] = LOW;
    pins[G] = LOW;
    pins[H] = LOW;
    pins[Y] = HIGH;
}


//
// -- use when built to make sure all the connections are updated
//    -----------------------------------------------------------
void IC_74xx30_t::TriggerFirstUpdate(void)
{
    UpdatesComplete();
    emit SignalYUpdated(pins[Y]);
}



//
// -- Update the Y NAND Gate
//    ----------------------
void IC_74xx30_t::ProcessUpdateGate(void)
{
    TriState_t last = pins[Y];

    pins[Y] = ((pins[A]==HIGH)
                &&(pins[B]==HIGH)
                &&(pins[C]==HIGH)
                &&(pins[D]==HIGH)
                &&(pins[E]==HIGH)
                &&(pins[F]==HIGH)
                &&(pins[G]==HIGH)
                &&(pins[H]==HIGH)?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y] != last) emit SignalYUpdated(pins[Y]);
}


