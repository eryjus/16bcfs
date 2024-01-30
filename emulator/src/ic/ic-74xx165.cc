//===================================================================================================================
// ic-74xx165.hh -- This header file defines a 74xx165 Parallel load shift out register
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-74xx165.moc.cc"



//
// -- Construct a new shift out register
//    ----------------------------------
IC_74xx165_t::IC_74xx165_t(void)
{
    pins[SH_LD] = LOW;
    pins[CLK] = HIGH;
    pins[E] = LOW;
    pins[F] = LOW;
    pins[G] = LOW;
    pins[H] = LOW;
    pins[QHb] = HIGH;
    pins[QH] = LOW;
    pins[SER] = LOW;
    pins[A] = LOW;
    pins[B] = LOW;
    pins[C] = LOW;
    pins[D] = LOW;
    pins[CLK_INH] = HIGH;

    val = 0;
    clockState = HIGH;

    TriggerFirstUpdate();
}



//
// -- Process a clock change in state
//    -------------------------------
void IC_74xx165_t::ProcessClock(void)
{
    TriState_t newState = ((pins[CLK]==HIGH)||(pins[CLK_INH]==HIGH)?HIGH:LOW);

    if (newState == clockState) return;
    if (newState == LOW) {
        clockState = LOW;
        return;
    }

    // -- are we in loading mode?  if so, no shifting
    if (pins[SH_LD] == LOW) return;

    // -- the only thing left here is a transition from LOW to HIGH and we are shifting
    val <<= 1;
    val |= (pins[SER] == HIGH ? 1 : 0);

    pins[QH] = ((val & 0x80) != 0 ? HIGH : LOW);
    pins[QHb] = (pins[QH] == HIGH ? LOW : HIGH);

    emit SignalQHUpdated(pins[QH]);
    emit SignalQHbUpdated(pins[QHb]);
}



//
// -- A parallel load pin has been updated and we are in load mode
//    ------------------------------------------------------------
void IC_74xx165_t::ProcessNewPin(void)
{
    val = (pins[H] == HIGH ? 0x80 : 0x00) |
            (pins[G] == HIGH ? 0x40 : 0x00) |
            (pins[F] == HIGH ? 0x20 : 0x00) |
            (pins[E] == HIGH ? 0x10 : 0x00) |
            (pins[D] == HIGH ? 0x08 : 0x00) |
            (pins[C] == HIGH ? 0x04 : 0x00) |
            (pins[B] == HIGH ? 0x02 : 0x00) |
            (pins[A] == HIGH ? 0x01 : 0x00);


    pins[QH] = ((val & 0x80) != 0 ? HIGH : LOW);
    pins[QHb] = (pins[QH] == HIGH ? LOW : HIGH);

    emit SignalQHUpdated(pins[QH]);
    emit SignalQHbUpdated(pins[QHb]);
}


