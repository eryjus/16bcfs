//===================================================================================================================
// ic-74xx04.cc -- This file implementes the 74xx04 Hex Inverter IC
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-26  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx04.moc.cc"



//
// -- This is the contructor for the Inverter
//    ---------------------------------------
IC_74xx04_t::IC_74xx04_t(void)
{
    pins[A1] = LOW;
    pins[Y1] = HIGH;
    pins[A2] = LOW;
    pins[Y2] = HIGH;
    pins[A3] = LOW;
    pins[Y3] = HIGH;
    pins[Y4] = HIGH;
    pins[A4] = LOW;
    pins[Y5] = HIGH;
    pins[A5] = LOW;
    pins[Y6] = HIGH;
    pins[A6] = LOW;
}


//
// -- use when built to make sure all the connections are updated
//    -----------------------------------------------------------
void IC_74xx04_t::TriggerFirstUpdate(void)
{
    UpdatesComplete();
    emit SignalY1Updated(pins[Y1]);
    emit SignalY2Updated(pins[Y2]);
    emit SignalY3Updated(pins[Y3]);
    emit SignalY4Updated(pins[Y4]);
    emit SignalY5Updated(pins[Y5]);
    emit SignalY6Updated(pins[Y6]);
}



//
// -- Update the Y1 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate1(void)
{
    TriState_t last = pins[Y1];

    pins[Y1] = (pins[A1]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y1] != last) emit SignalY1Updated(pins[Y1]);
}


//
// -- Update the Y2 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate2(void)
{
    TriState_t last = pins[Y2];

    pins[Y2] = (pins[A2]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y2] != last) emit SignalY2Updated(pins[Y2]);
}


//
// -- Update the Y3 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate3(void)
{
    TriState_t last = pins[Y3];

    pins[Y3] = (pins[A3]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y3] != last) emit SignalY3Updated(pins[Y3]);
}


//
// -- Update the Y4 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate4(void)
{
    TriState_t last = pins[Y4];

    pins[Y4] = (pins[A4]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y4] != last) emit SignalY4Updated(pins[Y4]);
}


//
// -- Update the Y5 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate5(void)
{
    TriState_t last = pins[Y5];

    pins[Y5] = (pins[A5]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y5] != last) emit SignalY5Updated(pins[Y5]);
}




//
// -- Update the Y6 Inverter Gate
//    ---------------------------
void IC_74xx04_t::ProcessUpdateGate6(void)
{
    TriState_t last = pins[Y6];

    pins[Y6] = (pins[A6]==HIGH?LOW:HIGH);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y6] != last) emit SignalY6Updated(pins[Y6]);
}


