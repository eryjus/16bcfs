//===================================================================================================================
//  hw-bus-1.cc -- This is a generic 1-bit bus
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-bus-1.moc.cc"



//
// -- Construct a new 1-bit bus
//    -------------------------
HW_Bus_1_t::HW_Bus_1_t(ClockModule_t *clk, QObject *parent) : QObject(parent), assertedBits(new Map_t)
{
    (*assertedBits)[0] = new Asserts_t;

    connect(clk, &ClockModule_t::SignalSanityCheck, this, &HW_Bus_1_t::ProcessSanityCheck);
}



//
// -- Maintain the bits on the 8-bit bus
//    ----------------------------------
void HW_Bus_1_t::MaintainBit(TriState_t state)
{
    qDebug() << "1-bit bus update" << state;
    TriState_t old;
    QObject *obj = sender();        // do NOT de-reference this pointer!  It may not be valid

    // -- Now, check the reference count and update as appropriate
    Asserts_t *asserts = (*assertedBits)[0];

    if (obj != nullptr) {
        if (state == Z) {
            auto it = asserts->find(obj);
            if (it != asserts->end()) {
                asserts->remove(obj);
            }
        } else {
            auto it = asserts->find(obj);
            if (it != asserts->end()) {
                (*asserts)[obj] = state;
            } else {
                (*asserts)[obj] = state;
            }
        }
    }

    // -- if nothing is being asserted, then default to low
    if (asserts->isEmpty()) state = HIGH;        // pull-up resistor
    else {
        auto it = asserts->find(obj);
        if (it == asserts->end()) state = asserts->first();
        else state = it.value();
    }

    // -- emit the proper notifications
    emit SignalBit0Updated(state);
}



//
// -- Handle the sanity check at the clock high and low levels where the status should be stable
//    ------------------------------------------------------------------------------------------
void HW_Bus_1_t::ProcessSanityCheck(void)
{
    if ((*assertedBits)[0]->count() > 1) {
        qDebug() << "Bus" << objectName() << "bit 0 has" << (*assertedBits)[0]->count() << "signals asserted";
    }
}

