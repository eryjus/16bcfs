//===================================================================================================================
// ic-74xx283.hh -- This class emulates a 74xx283 Full Adder
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-10  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx283.moc.cc"


//
// -- This is the contructor for the full adder
//    -----------------------------------------
IC_74xx283_t::IC_74xx283_t()
{
    pins[CIN] = LOW;
    pins[A0] = LOW;
    pins[B0] = LOW;
    pins[A1] = LOW;
    pins[B1] = LOW;
    pins[A2] = LOW;
    pins[B2] = LOW;
    pins[A3] = LOW;
    pins[B3] = LOW;

    pins[S0] = LOW;
    pins[S1] = LOW;
    pins[S2] = LOW;
    pins[S3] = LOW;
    pins[COUT] = LOW;

    a = 0;
    b = 0;
    sum = 0;

    TriggerFirstUpdate();
}


//
// -- perform the initial calculation of state and push the results out to all subscribers
//    ------------------------------------------------------------------------------------
void IC_74xx283_t::TriggerFirstUpdate(void)
{
    emit SignalS0Updated(pins[S0]);
    emit SignalS0Updated(pins[S1]);
    emit SignalS0Updated(pins[S2]);
    emit SignalS0Updated(pins[S3]);
    emit SignalS0Updated(pins[COUT]);
}



//
// -- Update the sum based on a change of the inputs
//    ----------------------------------------------
void IC_74xx283_t::UpdateSum(void)
{
    // -- take care that these internal values are updated before something might further update in a signal
    a = ((pins[A3] << 3) | (pins[A2] << 2) | (pins[A1] << 1) | (pins[A0] << 0) & 0x0f);
    b = ((pins[B3] << 3) | (pins[B2] << 2) | (pins[B1] << 1) | (pins[B0] << 0) & 0x0f);
    sum = a + b + (pins[CIN]==HIGH?1:0);

    TriState_t bit0 = ((sum&(1<<0))?HIGH:LOW);
    TriState_t bit1 = ((sum&(1<<1))?HIGH:LOW);
    TriState_t bit2 = ((sum&(1<<2))?HIGH:LOW);
    TriState_t bit3 = ((sum&(1<<3))?HIGH:LOW);
    TriState_t bit4 = ((sum&(1<<4))?HIGH:LOW);

    if (pins[S0] != bit0) {
        pins[S0] = bit0;
        emit SignalS0Updated(pins[S0]);
    }

    if (pins[S1] != bit1) {
        pins[S1] = bit1;
        emit SignalS1Updated(pins[S1]);
    }

    if (pins[S2] != bit2) {
        pins[S2] = bit2;
        emit SignalS2Updated(pins[S2]);
    }

    if (pins[S3] != bit3) {
        pins[S3] = bit3;
        emit SignalS3Updated(pins[S3]);
    }

    if (pins[COUT] != bit4) {
        pins[COUT] = bit4;
        emit SignalCOutUpdated(pins[COUT]);
    }
}


