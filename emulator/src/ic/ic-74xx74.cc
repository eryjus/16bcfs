//===================================================================================================================
// ic-74xx74.cc -- This class implements a 74xx74 Dual D-Type Latch
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-20  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx74.moc.cc"


//
// -- construct a new 74xx74 IC
//    -------------------------
IC_74xx74_t::IC_74xx74_t(void)
{
    pins[CLR1b] = HIGH;
    pins[D1] = LOW;
    pins[CLK1] = LOW;
    pins[PRE1b] = HIGH;
    pins[Q1] = LOW;
    pins[Q1b] = HIGH;

    pins[CLR2b] = HIGH;
    pins[D2] = LOW;
    pins[CLK2] = LOW;
    pins[PRE2b] = HIGH;
    pins[Q2] = LOW;
    pins[Q2b] = HIGH;

    d1 = LOW;
    d2 = LOW;

    lastClk1 = LOW;
    lastClk2 = LOW;

    TriggerFirstUpdate();
}


//
// -- handle triggering the initial updates
//    -------------------------------------
void IC_74xx74_t::TriggerFirstUpdate(void)
{
    emit SignalQ1Updated(pins[Q1]);
    emit SignalQ1bUpdated(pins[Q1b]);
    emit SignalQ2Updated(pins[Q2]);
    emit SignalQ2bUpdated(pins[Q2b]);
}


//
// -- Handle an asynchronous clear update
//    -----------------------------------
void IC_74xx74_t::ProcessUpdateClr1(TriState_t state)
{
    if (state == LOW) {
        pins[Q1] = ((pins[PRE1b]==LOW)?HIGH:LOW);
        pins[Q1b] = HIGH;
        pins[CLR1b] = state;

        emit SignalQ1Updated(pins[Q1]);
        emit SignalQ1bUpdated(pins[Q1b]);

        return;
    }

    // -- clear is high now
    if (pins[CLR1b] == LOW && pins[PRE1b] == LOW) {
        pins[Q1b] = LOW;

        emit SignalQ1bUpdated(pins[Q1b]);
    }

    pins[CLR1b] = state;
}


//
// -- Handle an asynchronous clear update
//    -----------------------------------
void IC_74xx74_t::ProcessUpdateClr2(TriState_t state)
{
    if (state == LOW) {
        pins[Q2] = ((pins[PRE2b]==LOW)?HIGH:LOW);
        pins[Q2b] = HIGH;
        pins[CLR2b] = state;

        emit SignalQ2Updated(pins[Q2]);
        emit SignalQ2bUpdated(pins[Q2b]);

        return;
    }

    // -- clear is high now
    if (pins[CLR2b] == LOW && pins[PRE2b] == LOW) {
        pins[Q2b] = LOW;

        emit SignalQ2bUpdated(pins[Q2b]);
    }

    pins[CLR2b] = state;
}


//
// -- Handle an asynchronous set update
//    ---------------------------------
void IC_74xx74_t::ProcessUpdatePre1(TriState_t state)
{
    if (state == LOW) {
        pins[Q1] = HIGH;
        pins[Q1b] = ((pins[CLR1b]==LOW)?HIGH:LOW);
        pins[PRE1b] = state;

        emit SignalQ1Updated(pins[Q1]);
        emit SignalQ1bUpdated(pins[Q1b]);

        return;
    }

    // -- preset is high now
    if (pins[CLR1b] == LOW && pins[PRE1b] == LOW) {
        pins[Q1] = LOW;

        emit SignalQ1Updated(pins[Q1]);
    }

    pins[PRE1b] = state;
}


//
// -- Handle an asynchronous clear update
//    -----------------------------------
void IC_74xx74_t::ProcessUpdatePre2(TriState_t state)
{
    if (state == LOW) {
        pins[Q2] = HIGH;
        pins[Q2b] = ((pins[CLR2b]==LOW)?HIGH:LOW);
        pins[PRE2b] = state;

        emit SignalQ2Updated(pins[Q2]);
        emit SignalQ2bUpdated(pins[Q2b]);

        return;
    }

    // -- preset is high now
    if (pins[CLR2b] == LOW && pins[PRE2b] == LOW) {
        pins[Q2] = LOW;

        emit SignalQ2Updated(pins[Q2]);
    }

    pins[PRE2b] = state;
}


//
// -- handle a clock tick
//    -------------------
void IC_74xx74_t::ProcessUpdateClockLatch1(TriState_t state)
{
    lastClk1 = pins[CLK1];
    pins[CLK1] = state;

    if (pins[CLR1b] == LOW || pins[PRE1b] == LOW) return;

    // -- rising edge
    if (state == HIGH && lastClk1 == LOW) {
        d1 = pins[D1];
    }
}


//
// -- handle a clock tick
//    -------------------
void IC_74xx74_t::ProcessUpdateClockOutput1(TriState_t state)
{
    if (state == HIGH && lastClk1 == LOW) {
        if (pins[Q1] != d1) {
            pins[Q1] = d1;
            pins[Q1b] = pins[Q1]==HIGH?LOW:HIGH;

            emit SignalQ1Updated(pins[Q1]);
            emit SignalQ1bUpdated(pins[Q1b]);
        }
    }
}


//
// -- handle a clock tick
//    -------------------
void IC_74xx74_t::ProcessUpdateClockLatch2(TriState_t state)
{
    lastClk2 = pins[CLK2];
    pins[CLK2] = state;

    if (pins[CLR2b] == LOW || pins[PRE2b] == LOW) return;

    // -- rising edge
    if (state == HIGH && lastClk2 == LOW) {
        d2 = pins[D2];
    }
}


//
// -- handle a clock tick
//    -------------------
void IC_74xx74_t::ProcessUpdateClockOutput2(TriState_t state)
{
    if (state == HIGH && lastClk2 == LOW) {
        if (pins[Q2] != d2) {
            pins[Q2] = d2;
            pins[Q2b] = pins[Q2]==HIGH?LOW:HIGH;

            emit SignalQ2Updated(pins[Q2]);
            emit SignalQ2bUpdated(pins[Q2b]);
        }
    }
}


