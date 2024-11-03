//===================================================================================================================
// ic-74xx123.cc -- This file implementes the a 74xx123 Multivibrator
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-03  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx123.moc.cc"


//
// -- This is the contructor for the UP/DOWN cnter, having 16 pins
//    --------------------------------------------------------------
IC_74xx123_t::IC_74xx123_t(void)
{
    pins[A1b] = HIGH;
    pins[B1] = LOW;
    pins[RD1b] = LOW;
    pins[Q1b] = HIGH;
    pins[Q2] = LOW;
    pins[CEXT2] = Z;
    pins[RCEXT2] = LOW;
    pins[A2b] = HIGH;
    pins[B2] = LOW;
    pins[RD2b] = LOW;
    pins[Q2b] = HIGH;
    pins[Q2] = LOW;
    pins[CEXT1] = Z;
    pins[RCEXT2] = LOW;


    a1 = pins[A1b];
    b1 = pins[B1];
    rd1 = pins[RD1b];
    a2 = pins[A2b];
    b2 = pins[B2];
    rd2 = pins[RD2b];

    cycling1 = false;
    cycling2 = false;
}


//
// -- Update everything now that updates are complete
//    -----------------------------------------------
void IC_74xx123_t::ProcessUpdates1Complete(void)
{
    if ((a1 == HIGH && pins[A1b] == LOW) || (b1 == LOW && pins[B1] == HIGH) || (rd1 == LOW && pins[RD1b] == HIGH)) {
        if (!cycling1) {
            cycling1 = true;
            QTimer::singleShot(50, this, &IC_74xx123_t::ProcessUpdateRCEXT1);
        }
    }

    a1 = pins[A1b];
    b1 = pins[B1];
    rd1 = pins[RD1b];
}


//
// -- Update everything now that updates are complete
//    -----------------------------------------------
void IC_74xx123_t::ProcessUpdates2Complete(void)
{
    if ((a2 == HIGH && pins[A2b] == LOW) || (b2 == LOW && pins[B2] == HIGH) || (rd2 == LOW && pins[RD2b] == HIGH)) {
        if (!cycling2) {
            cycling2 = true;
            QTimer::singleShot(50, this, &IC_74xx123_t::ProcessUpdateRCEXT2);
        }
    }

    a2 = pins[A2b];
    b2 = pins[B2];
    rd2 = pins[RD2b];
}



//
// -- A timeout has occurred, perform the updates
//    -------------------------------------------
void IC_74xx123_t::ProcessUpdateRCEXT1(void)
{
    pins[Q1] = LOW;
    pins[Q1b] = HIGH;

    cycling1 = false;

    emit SignalQ1Updated(LOW);
    emit SignalQ1bUpdated(HIGH);
}


//
// -- A timeout has occurred, perform the updates
//    -------------------------------------------
void IC_74xx123_t::ProcessUpdateRCEXT2(void)
{
    pins[Q2] = LOW;
    pins[Q2b] = HIGH;

    cycling2 = false;

    emit SignalQ2Updated(LOW);
    emit SignalQ2bUpdated(HIGH);
}

