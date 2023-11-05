//===================================================================================================================
// ic-74xx138.cc -- This file implementes the a 74xx138 3:8 Demultiplexer
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-23  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx138.moc.cc"


//
// -- This is the contructor for the UP/DOWN cnter, having 16 pins
//    --------------------------------------------------------------
IC_74xx138_t::IC_74xx138_t(void)
{
    pins[A] = LOW;
    pins[B] = LOW;
    pins[C] = LOW;
    pins[G2Ab] = LOW;
    pins[G2Bb] = LOW;
    pins[G1] = LOW;
    pins[Y7] = HIGH;
    pins[Y6] = HIGH;
    pins[Y5] = HIGH;
    pins[Y4] = HIGH;
    pins[Y3] = HIGH;
    pins[Y2] = HIGH;
    pins[Y1] = HIGH;
    pins[Y0] = HIGH;
}


//
// -- Update the XOR Gates, being careful not to push any changes for performance reasons
//    -----------------------------------------------------------------------------------
void IC_74xx138_t::UpdatesComplete(void)
{
    pins[Y0] = HIGH;
    pins[Y1] = HIGH;
    pins[Y2] = HIGH;
    pins[Y3] = HIGH;
    pins[Y4] = HIGH;
    pins[Y5] = HIGH;
    pins[Y6] = HIGH;
    pins[Y7] = HIGH;

    if (pins[G1] == HIGH && pins[G2Ab] == LOW && pins[G2Bb] == LOW) {
        int which = (pins[C]?1:0) << 2 | (pins[B]?1:0) << 1 | (pins[A]?1:0) << 0;

        switch (which) {
        case 0:
            pins[Y0] = LOW;
            break;

        case 1:
            pins[Y1] = LOW;
            break;

        case 2:
            pins[Y2] = LOW;
            break;

        case 3:
            pins[Y3] = LOW;
            break;

        case 4:
            pins[Y4] = LOW;
            break;

        case 5:
            pins[Y5] = LOW;
            break;

        case 6:
            pins[Y6] = LOW;
            break;

        case 7:
            pins[Y7] = LOW;
            break;
        }
    }

    emit Y0Updated(pins[Y0]);
    emit Y1Updated(pins[Y1]);
    emit Y2Updated(pins[Y2]);
    emit Y3Updated(pins[Y3]);
    emit Y4Updated(pins[Y4]);
    emit Y5Updated(pins[Y5]);
    emit Y6Updated(pins[Y6]);
    emit Y7Updated(pins[Y7]);

    emit AllUpdated();
}


