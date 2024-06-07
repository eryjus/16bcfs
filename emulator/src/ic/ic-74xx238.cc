//===================================================================================================================
// ic-74xx238.cc -- This file implementes the a 74xx238 3:8 Demultiplexer
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-23  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx238.moc.cc"


//
// -- This is the contructor for the UP/DOWN cnter, having 16 pins
//    --------------------------------------------------------------
IC_74xx238_t::IC_74xx238_t(void)
{
    pins[A] = LOW;
    pins[B] = LOW;
    pins[C] = LOW;
    pins[G2Ab] = LOW;
    pins[G2Bb] = LOW;
    pins[G1] = LOW;
    pins[Y7] = LOW;
    pins[Y6] = LOW;
    pins[Y5] = LOW;
    pins[Y4] = LOW;
    pins[Y3] = LOW;
    pins[Y2] = LOW;
    pins[Y1] = LOW;
    pins[Y0] = LOW;
}


//
// -- Update the XOR Gates, being careful not to push any changes for performance reasons
//    -----------------------------------------------------------------------------------
void IC_74xx238_t::ProcessUpdatesComplete(void)
{
    pins[Y0] = LOW;
    pins[Y1] = LOW;
    pins[Y2] = LOW;
    pins[Y3] = LOW;
    pins[Y4] = LOW;
    pins[Y5] = LOW;
    pins[Y6] = LOW;
    pins[Y7] = LOW;

    if (pins[G1] == HIGH && pins[G2Ab] == LOW && pins[G2Bb] == LOW) {
        int which = (pins[C]?1:0) << 2 | (pins[B]?1:0) << 1 | (pins[A]?1:0) << 0;

        switch (which) {
        case 0:
            pins[Y0] = HIGH;
            break;

        case 1:
            pins[Y1] = HIGH;
            break;

        case 2:
            pins[Y2] = HIGH;
            break;

        case 3:
            pins[Y3] = HIGH;
            break;

        case 4:
            pins[Y4] = HIGH;
            break;

        case 5:
            pins[Y5] = HIGH;
            break;

        case 6:
            pins[Y6] = HIGH;
            break;

        case 7:
            pins[Y7] = HIGH;
            break;
        }
    }

    emit SignalY0Updated(pins[Y0]);
    emit SignalY1Updated(pins[Y1]);
    emit SignalY2Updated(pins[Y2]);
    emit SignalY3Updated(pins[Y3]);
    emit SignalY4Updated(pins[Y4]);
    emit SignalY5Updated(pins[Y5]);
    emit SignalY6Updated(pins[Y6]);
    emit SignalY7Updated(pins[Y7]);
}


