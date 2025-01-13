//===================================================================================================================
// ic-74xx03.cc -- This file implementes the 74xx03 Quad Two-Input AND Gate IC
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-22  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx03.moc.cc"


//
// -- This is the contructor for the AND Gate IC, which has 14 pins
//    -------------------------------------------------------------
IC_74xx03_t::IC_74xx03_t(void)
{
    pins[A1] = HIGH;
    pins[B1] = HIGH;
    pins[Y1] = Z;
    pins[A2] = HIGH;
    pins[B2] = HIGH;
    pins[Y2] = Z;
    pins[Y3] = Z;
    pins[B3] = HIGH;
    pins[A3] = HIGH;
    pins[Y4] = Z;
    pins[B4] = HIGH;
    pins[A4] = HIGH;
}


//
// -- use when built to make sure all the connections are updated
//    -----------------------------------------------------------
void IC_74xx03_t::TriggerFirstUpdate(void)
{
    UpdatesComplete();
    emit SignalY1Updated(pins[Y1]);
    emit SignalY2Updated(pins[Y2]);
    emit SignalY3Updated(pins[Y3]);
    emit SignalY4Updated(pins[Y4]);
}



//
// -- Update the Y1 AND Gates
//    -----------------------
void IC_74xx03_t::ProcessUpdateGate1(void)
{
    TriState_t last = pins[Y1];

    pins[Y1] = ((pins[A1]==HIGH)&&(pins[B1]==HIGH)?LOW:Z);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y1] != last) emit SignalY1Updated(pins[Y1]);
}


//
// -- Update the Y2 AND Gates
//    -----------------------
void IC_74xx03_t::ProcessUpdateGate2(void)
{
    TriState_t last = pins[Y2];

    pins[Y2] = ((pins[A2]==HIGH)&&(pins[B2]==HIGH)?LOW:Z);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y2] != last) emit SignalY2Updated(pins[Y2]);
}


//
// -- Update the Y3 AND Gates
//    -----------------------
void IC_74xx03_t::ProcessUpdateGate3(void)
{
    TriState_t last = pins[Y3];

    pins[Y3] = ((pins[A3]==HIGH)&&(pins[B3]==HIGH)?LOW:Z);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y3] != last) emit SignalY3Updated(pins[Y3]);
}

//
// -- Update the Y4 AND Gates
//    -----------------------
void IC_74xx03_t::ProcessUpdateGate4(void)
{
    TriState_t last = pins[Y4];

    pins[Y4] = ((pins[A4]==HIGH)&&(pins[B4]==HIGH)?LOW:Z);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y4] != last) emit SignalY4Updated(pins[Y4]);
}