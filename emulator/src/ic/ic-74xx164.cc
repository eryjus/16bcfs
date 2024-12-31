//===================================================================================================================
// ic-74xx164.cc -- This header file defines a 74xx164 shift in parallel output register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Feb-05  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-74xx164.moc.cc"



//
// -- Construct a new shift in register
//    ---------------------------------
IC_74xx164_t::IC_74xx164_t(void)
{
    pins[A] = LOW;
    pins[B] = LOW;
    pins[QA] = LOW;
    pins[QB] = LOW;
    pins[QC] = LOW;
    pins[QD] = LOW;
    pins[CLK] = LOW;
    pins[CLRb] = HIGH;
    pins[QE] = LOW;
    pins[QF] = LOW;
    pins[QG] = LOW;
    pins[QH] = LOW;

    TriggerFirstUpdate();
}



//
// -- Handle the first updates to output pins
//    ---------------------------------------
void IC_74xx164_t::TriggerFirstUpdate(void)
{
    emit SignalQAUpdated(pins[QA]);
    emit SignalQBUpdated(pins[QB]);
    emit SignalQCUpdated(pins[QC]);
    emit SignalQDUpdated(pins[QD]);
    emit SignalQEUpdated(pins[QE]);
    emit SignalQFUpdated(pins[QF]);
    emit SignalQGUpdated(pins[QG]);
    emit SignalQHUpdated(pins[QH]);
}



//
// -- Process a clear pin change
//    --------------------------
void IC_74xx164_t::ProcessUpdateClr(TriState_t state)
{
    pins[CLRb] = state;

    if (state == HIGH) return;

    if (pins[QA] != LOW) {
        pins[QA] = LOW;
        emit SignalQAUpdated(pins[QA]);
    }

    if (pins[QB] != LOW) {
        pins[QB] = LOW;
        emit SignalQBUpdated(pins[QB]);
    }

    if (pins[QC] != LOW) {
        pins[QC] = LOW;
        emit SignalQCUpdated(pins[QC]);
    }

    if (pins[QD] != LOW) {
        pins[QD] = LOW;
        emit SignalQDUpdated(pins[QD]);
    }

    if (pins[QE] != LOW) {
        pins[QE] = LOW;
        emit SignalQEUpdated(pins[QE]);
    }

    if (pins[QF] != LOW) {
        pins[QF] = LOW;
        emit SignalQFUpdated(pins[QF]);
    }

    if (pins[QG] != LOW) {
        pins[QG] = LOW;
        emit SignalQGUpdated(pins[QG]);
    }

    if (pins[QH] != LOW) {
        pins[QH] = LOW;
        emit SignalQHUpdated(pins[QH]);
    }
}




//
// -- Process a clock change in state
//    -------------------------------
void IC_74xx164_t::ProcessUpdateClk(TriState_t state)
{
    if (pins[CLK] == state) return;

    pins[CLK] = state;

    if (pins[CLRb] == LOW) return;

    if (state == LOW) return;

    // -- low to high transition
    if (pins[QH] != pins[QG]) {
        pins[QH] = pins[QG];
        emit SignalQHUpdated(pins[QH]);
    }

    if (pins[QG] != pins[QF]) {
        pins[QG] = pins[QF];
        emit SignalQGUpdated(pins[QG]);
    }

    if (pins[QF] != pins[QE]) {
        pins[QF] = pins[QE];
        emit SignalQFUpdated(pins[QF]);
    }

    if (pins[QE] != pins[QD]) {
        pins[QE] = pins[QD];
        emit SignalQEUpdated(pins[QE]);
    }

    if (pins[QD] != pins[QC]) {
        pins[QD] = pins[QC];
        emit SignalQDUpdated(pins[QD]);
    }

    if (pins[QC] != pins[QB]) {
        pins[QC] = pins[QB];
        emit SignalQCUpdated(pins[QC]);
    }

    if (pins[QB] != pins[QA]) {
        pins[QB] = pins[QA];
        emit SignalQBUpdated(pins[QB]);
    }

    TriState_t a = ((pins[A]==HIGH)&&(pins[B]==HIGH)?HIGH:LOW);
    if (pins[QA] != a) {
        pins[QA] = a;
        emit SignalQAUpdated(pins[QA]);
    }
}


