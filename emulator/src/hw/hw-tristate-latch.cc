//===================================================================================================================
//  hw-tristate-latch.cc -- This class implements a 3-state latch, where 1 of 3 states can be selected
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-29  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/hw-tristate-latch.moc.cc"



//
// -- Construct a tristate latch
//    --------------------------
HW_TriStateLatch_t::HW_TriStateLatch_t(QObject *parent) : QObject(parent)
{
    tristateNand1 = new IC_74xx00_t;
    tristateNand2 = new IC_74xx00_t;
    tristateNor = new IC_74xx02_t;

    tristateNand2->ProcessUpdateA3(GND);
    tristateNand2->ProcessUpdateB3(GND);
    tristateNand2->ProcessUpdateA4(GND);
    tristateNand2->ProcessUpdateB4(GND);
    tristateNor->ProcessUpdateA4(GND);
    tristateNor->ProcessUpdateB4(GND);

    connect(tristateNand1, &IC_74xx00_t::SignalY1Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateA2);
    connect(tristateNand1, &IC_74xx00_t::SignalY2Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateB1);
    connect(tristateNand1, &IC_74xx00_t::SignalY3Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateA4);
    connect(tristateNand1, &IC_74xx00_t::SignalY4Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateB3);
    connect(tristateNand2, &IC_74xx00_t::SignalY1Updated, tristateNand2, &IC_74xx00_t::ProcessUpdateA2);
    connect(tristateNand2, &IC_74xx00_t::SignalY2Updated, tristateNand2, &IC_74xx00_t::ProcessUpdateB1);

    connect(tristateNand1, &IC_74xx00_t::SignalY1Updated, tristateNor, &IC_74xx02_t::ProcessUpdateA2);
    connect(tristateNand1, &IC_74xx00_t::SignalY1Updated, tristateNor, &IC_74xx02_t::ProcessUpdateA3);
    connect(tristateNand1, &IC_74xx00_t::SignalY3Updated, tristateNor, &IC_74xx02_t::ProcessUpdateA1);
    connect(tristateNand1, &IC_74xx00_t::SignalY3Updated, tristateNor, &IC_74xx02_t::ProcessUpdateB3);
    connect(tristateNand2, &IC_74xx00_t::SignalY1Updated, tristateNor, &IC_74xx02_t::ProcessUpdateB1);
    connect(tristateNand2, &IC_74xx00_t::SignalY1Updated, tristateNor, &IC_74xx02_t::ProcessUpdateB2);

    connect(tristateNor, &IC_74xx02_t::SignalY1Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateB2);
    connect(tristateNor, &IC_74xx02_t::SignalY2Updated, tristateNand1, &IC_74xx00_t::ProcessUpdateB4);
    connect(tristateNor, &IC_74xx02_t::SignalY3Updated, tristateNand2, &IC_74xx00_t::ProcessUpdateB2);

    connect(tristateNand1, &IC_74xx00_t::SignalY1Updated, this, &HW_TriStateLatch_t::ProcessUpdateQ1);
    connect(tristateNand1, &IC_74xx00_t::SignalY3Updated, this, &HW_TriStateLatch_t::ProcessUpdateQ2);
    connect(tristateNand2, &IC_74xx00_t::SignalY1Updated, this, &HW_TriStateLatch_t::ProcessUpdateQ3);

    // -- set the initial state
    tristateNand2->ProcessA1High();
    tristateNand1->ProcessA3High();
    tristateNand1->ProcessA1Low();
    tristateNand1->ProcessA1High();

    TriggerFirstUpdates();
}


//
// -- Trigger the fitst updates for all children
//    ------------------------------------------
void HW_TriStateLatch_t::TriggerFirstUpdates(void)
{
    tristateNand1->TriggerFirstUpdates();
    tristateNand2->TriggerFirstUpdates();
    tristateNor->TriggerFirstUpdates();
}





