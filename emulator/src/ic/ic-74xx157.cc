//===================================================================================================================
// ic-74xx157.hh -- This header file defines a 74xx157 Quad 2:1 Multiplexer
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-74xx157.moc.cc"



//
// -- Construct a new 2:1 Mux
//    -----------------------
IC_74xx157_t::IC_74xx157_t(void)
{
    pins[A_B] = LOW;
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
    pins[Gb] = HIGH;

    TriggerFirstUpdate();
}



//
// -- Emit the first signals to update all subscribers
//    ------------------------------------------------
void IC_74xx157_t::TriggerFirstUpdate(void)
{
    emit SignalY1Updated(pins[Y1]);
    emit SignalY2Updated(pins[Y2]);
    emit SignalY3Updated(pins[Y3]);
    emit SignalY4Updated(pins[Y4]);
}



//
// -- Process an update to a pin
//    --------------------------
void IC_74xx157_t::ProcessUpdate(void)
{
    TriState_t newY1;
    TriState_t newY2;
    TriState_t newY3;
    TriState_t newY4;

    if (pins[Gb] != LOW) {
        newY1 = newY2 = newY3 = newY4 = LOW;

        goto exit;
    }

    if (pins[A_B] == LOW) {
        newY1 = pins[A1];
        newY2 = pins[A2];
        newY3 = pins[A3];
        newY4 = pins[A4];

        goto exit;
    } else {
        newY1 = pins[B1];
        newY2 = pins[B2];
        newY3 = pins[B3];
        newY4 = pins[B4];

        goto exit;
    }


exit:
    if (newY1 != pins[Y1]) {
        pins[Y1] = newY1;
        emit SignalY1Updated(pins[Y1]);
    }

    if (newY2 != pins[Y2]) {
        pins[Y2] = newY2;
        emit SignalY2Updated(pins[Y2]);
    }

    if (newY3 != pins[Y3]) {
        pins[Y3] = newY3;
        emit SignalY3Updated(pins[Y3]);
    }

    if (newY4 != pins[Y4]) {
        pins[Y4] = newY4;
        emit SignalY4Updated(pins[Y4]);
    }
}



