//===================================================================================================================
// ic-74xx151.cc -- This file implementes the a 74xx151 3:8 Demultiplexer
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-23  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx151.moc.cc"


//
// -- This is the contructor for the UP/DOWN cnter, having 16 pins
//    --------------------------------------------------------------
IC_74xx151_t::IC_74xx151_t(void)
{
    pins[A] = LOW;
    pins[B] = LOW;
    pins[C] = LOW;
    pins[D0] = LOW;
    pins[D1] = LOW;
    pins[D2] = LOW;
    pins[D3] = LOW;
    pins[D4] = LOW;
    pins[D5] = LOW;
    pins[D6] = LOW;
    pins[D7] = LOW;
    pins[STb] = HIGH;
    pins[Y] = LOW;
    pins[W] = HIGH;
}



//
// -- Trigger the initial updates for this IC
//    ---------------------------------------
inline void IC_74xx151_t::TriggerFirstUpdate(void)
{
    emit SignalWUpdated(pins[W]);
    emit SignalYUpdated(pins[Y]);
}



//
// -- Update the Outputs
//    ------------------
void IC_74xx151_t::ProcessUpdatesComplete(void)
{
    if (pins[STb] == HIGH) {
        pins[Y] = LOW;
        pins[W] = HIGH;
        TriggerFirstUpdate();

        return;
    }

    int which = ((pins[C]==HIGH?1:0) << 2) | ((pins[B]==HIGH?1:0) << 1) | ((pins[A]==HIGH?1:0) << 0);

    switch(which) {
    case 0:
        pins[Y] = pins[D0];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 1:
        pins[Y] = pins[D1];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 2:
        pins[Y] = pins[D2];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 3:
        pins[Y] = pins[D3];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 4:
        pins[Y] = pins[D4];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 5:
        pins[Y] = pins[D5];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 6:
        pins[Y] = pins[D6];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;

    case 7:
        pins[Y] = pins[D7];
        pins[W] = (pins[Y]==HIGH?LOW:HIGH);
        break;
    }

    TriggerFirstUpdate();
}


