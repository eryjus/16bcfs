//===================================================================================================================
// ic-74xx08.cc -- This file implementes the 74xx08 Quad Two-Input AND Gate IC
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-22  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx08.moc.cc"


//
// -- This is the contructor for the AND Gate IC, which has 14 pins
//    -------------------------------------------------------------
IC_74xx08_t::IC_74xx08_t(void)
{
    pins[A1] = LOW;
    pins[B1] = LOW;
    pins[Y1] = LOW;
    pins[A2] = LOW;
    pins[B2] = LOW;
    pins[Y2] = LOW;
    pins[Y3] = LOW;
    pins[B3] = LOW;
    pins[A3] = LOW;
    pins[Y4] = LOW;
    pins[B4] = LOW;
    pins[A4] = LOW;
}


//
// -- use when built to make sure all the connections are updated
//    -----------------------------------------------------------
void IC_74xx08_t::TriggerFirstUpdates(void)
{
    UpdatesComplete();
    emit Y1Updated(pins[Y1]);
    emit Y2Updated(pins[Y2]);
    emit Y3Updated(pins[Y3]);
    emit Y4Updated(pins[Y4]);
}



//
// -- Update the Y1 AND Gates
//    -----------------------
void IC_74xx08_t::UpdateGate1(void)
{
    TriState_t last = pins[Y1];

    pins[Y1] = ((pins[A1]==HIGH)&&(pins[B1]==HIGH)?HIGH:LOW);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y1] != last) emit Y1Updated(pins[Y1]);
}


//
// -- Update the Y2 AND Gates
//    -----------------------
void IC_74xx08_t::UpdateGate2(void)
{
    TriState_t last = pins[Y2];

    pins[Y2] = ((pins[A2]==HIGH)&&(pins[B2]==HIGH)?HIGH:LOW);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y2] != last) emit Y2Updated(pins[Y2]);
}


//
// -- Update the Y3 AND Gates
//    -----------------------
void IC_74xx08_t::UpdateGate3(void)
{
    TriState_t last = pins[Y3];

    pins[Y3] = ((pins[A3]==HIGH)&&(pins[B3]==HIGH)?HIGH:LOW);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y3] != last) emit Y3Updated(pins[Y3]);
}

//
// -- Update the Y4 AND Gates
//    -----------------------
void IC_74xx08_t::UpdateGate4(void)
{
    TriState_t last = pins[Y4];

    pins[Y4] = ((pins[A4]==HIGH)&&(pins[B4]==HIGH)?HIGH:LOW);

    // -- protect against infinite recursion when nothing changes
    if (pins[Y4] != last) emit Y4Updated(pins[Y4]);
}