//===================================================================================================================
// ic-74xx541.cc -- This class implements a 74xx541 Line Driver
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-30  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx541.moc.cc"


//
// -- This is the contructor for the Line Driver
//    ------------------------------------------
IC_74xx541_t::IC_74xx541_t()
{
    pins[OE1b] = HIGH;
    pins[D0] = LOW;
    pins[D1] = LOW;
    pins[D2] = LOW;
    pins[D3] = LOW;
    pins[D4] = LOW;
    pins[D5] = LOW;
    pins[D6] = LOW;
    pins[D7] = LOW;
    pins[Y7] = Z;
    pins[Y6] = Z;
    pins[Y5] = Z;
    pins[Y4] = Z;
    pins[Y3] = Z;
    pins[Y2] = Z;
    pins[Y1] = Z;
    pins[Y0] = Z;
    pins[OE2b] = HIGH;
}


//
// -- Update the outputs based on the OE pins
//    ---------------------------------------
void IC_74xx541_t::UpdateOutputs(void)
{
    TriState_t oe = (pins[OE1b] == HIGH || pins[OE2b] == HIGH) ? HIGH : LOW;

    if (oe == HIGH) {
        pins[Y0] = Z;
        pins[Y1] = Z;
        pins[Y2] = Z;
        pins[Y3] = Z;
        pins[Y4] = Z;
        pins[Y5] = Z;
        pins[Y6] = Z;
        pins[Y7] = Z;
    } else {
        pins[Y0] = pins[D0];
        pins[Y1] = pins[D1];
        pins[Y2] = pins[D2];
        pins[Y3] = pins[D3];
        pins[Y4] = pins[D4];
        pins[Y5] = pins[D5];
        pins[Y6] = pins[D6];
        pins[Y7] = pins[D7];
    }

    emit SignalY0Updated(oe==HIGH?Z:pins[Y0]);
    emit SignalY1Updated(oe==HIGH?Z:pins[Y1]);
    emit SignalY2Updated(oe==HIGH?Z:pins[Y2]);
    emit SignalY3Updated(oe==HIGH?Z:pins[Y3]);
    emit SignalY4Updated(oe==HIGH?Z:pins[Y4]);
    emit SignalY5Updated(oe==HIGH?Z:pins[Y5]);
    emit SignalY6Updated(oe==HIGH?Z:pins[Y6]);
    emit SignalY7Updated(oe==HIGH?Z:pins[Y7]);
}



