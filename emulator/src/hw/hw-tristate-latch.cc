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

    tristateNand2->UpdateA3(GND);
    tristateNand2->UpdateB3(GND);
    tristateNand2->UpdateA4(GND);
    tristateNand2->UpdateB4(GND);
    tristateNor->UpdateA4(GND);
    tristateNor->UpdateB4(GND);

    connect(tristateNand1, &IC_74xx00_t::Y1Updated, tristateNand1, &IC_74xx00_t::UpdateA2);
    connect(tristateNand1, &IC_74xx00_t::Y2Updated, tristateNand1, &IC_74xx00_t::UpdateB1);
    connect(tristateNand1, &IC_74xx00_t::Y3Updated, tristateNand1, &IC_74xx00_t::UpdateA4);
    connect(tristateNand1, &IC_74xx00_t::Y4Updated, tristateNand1, &IC_74xx00_t::UpdateB3);
    connect(tristateNand2, &IC_74xx00_t::Y1Updated, tristateNand2, &IC_74xx00_t::UpdateA2);
    connect(tristateNand2, &IC_74xx00_t::Y2Updated, tristateNand2, &IC_74xx00_t::UpdateB1);

    connect(tristateNand1, &IC_74xx00_t::Y1Updated, tristateNor, &IC_74xx02_t::UpdateA2);
    connect(tristateNand1, &IC_74xx00_t::Y1Updated, tristateNor, &IC_74xx02_t::UpdateA3);
    connect(tristateNand1, &IC_74xx00_t::Y3Updated, tristateNor, &IC_74xx02_t::UpdateA1);
    connect(tristateNand1, &IC_74xx00_t::Y3Updated, tristateNor, &IC_74xx02_t::UpdateB3);
    connect(tristateNand2, &IC_74xx00_t::Y1Updated, tristateNor, &IC_74xx02_t::UpdateB1);
    connect(tristateNand2, &IC_74xx00_t::Y1Updated, tristateNor, &IC_74xx02_t::UpdateB2);

    connect(tristateNor, &IC_74xx02_t::Y1Updated, tristateNand1, &IC_74xx00_t::UpdateB2);
    connect(tristateNor, &IC_74xx02_t::Y2Updated, tristateNand1, &IC_74xx00_t::UpdateB4);
    connect(tristateNor, &IC_74xx02_t::Y3Updated, tristateNand2, &IC_74xx00_t::UpdateB2);

    connect(tristateNand1, &IC_74xx00_t::Y1Updated, this, &HW_TriStateLatch_t::UpdateQ1);
    connect(tristateNand1, &IC_74xx00_t::Y3Updated, this, &HW_TriStateLatch_t::UpdateQ2);
    connect(tristateNand2, &IC_74xx00_t::Y1Updated, this, &HW_TriStateLatch_t::UpdateQ3);

    // -- set the initial state
    tristateNand2->A1High();
    tristateNand1->A3High();
    tristateNand1->A1Low();
    tristateNand1->A1High();

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





