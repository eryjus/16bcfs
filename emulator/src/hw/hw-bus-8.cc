//===================================================================================================================
//  hw-bus-8.cc -- This is a generic bus
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-bus-8.moc.cc"



//
// -- Construct a new 8-bit bus
//    -------------------------
HW_Bus_8_t::HW_Bus_8_t(const QString &name, ClockModule_t *clk, QObject *parent)
        : QObject(parent), assertedBits(new Map_t)
{
    setObjectName(name);

    for (int i = BIT_0; i <= BIT_7; i ++) (*assertedBits)[i] = new Asserts_t;

    connect(clk, &ClockModule_t::SignalSanityCheck, this, &HW_Bus_8_t::ProcessSanityCheck);
}



//
// -- Maintain the bits on the 8-bit bus
//    ----------------------------------
void HW_Bus_8_t::MaintainBit(int bit, TriState_t state)
{
    TriState_t old;
    QObject *obj = sender();        // do NOT de-reference this pointer!  It may not be valid

    // -- Now, check the reference count and update as appropriate
    Asserts_t *asserts = (*assertedBits)[bit];

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
    if (asserts->isEmpty()) state = LOW;        // pull-down resistor
    else {
        auto it = asserts->find(obj);
        if (it == asserts->end()) state = asserts->first();
        else state = it.value();
    }

    // -- emit the proper notifications
    switch(bit) {
    case BIT_0: emit SignalBit0Updated(state); break;
    case BIT_1: emit SignalBit1Updated(state); break;
    case BIT_2: emit SignalBit2Updated(state); break;
    case BIT_3: emit SignalBit3Updated(state); break;
    case BIT_4: emit SignalBit4Updated(state); break;
    case BIT_5: emit SignalBit5Updated(state); break;
    case BIT_6: emit SignalBit6Updated(state); break;
    case BIT_7: emit SignalBit7Updated(state); break;
    }
}



//
// -- Handle the sanity check at the clock high and low levels where the status should be stable
//    ------------------------------------------------------------------------------------------
void HW_Bus_8_t::ProcessSanityCheck(void)
{
    for (int i = BIT_0; i <= BIT_7; i ++) {
        if ((*assertedBits)[i]->count() > 1) {
            qDebug() << "Bus" << objectName() << "bit" << i << "has" << (*assertedBits)[i]->count() << "signals asserted";
        }
    }
}

