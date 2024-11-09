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
    pins[A1b] = LOW;
    pins[B1] = HIGH;
    pins[RD1b] = LOW;
    pins[Q1b] = Z;
    pins[Q2] = Z;
    pins[CEXT2] = Z;
    pins[RCEXT2] = LOW;
    pins[A2b] = LOW;
    pins[B2] = HIGH;
    pins[RD2b] = LOW;
    pins[Q2b] = Z;
    pins[Q2] = Z;
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

//    connect(this, &IC_74xx123_t::SignalQ1Updated, this, &IC_74xx123_t::DebugQ1);
//    connect(this, &IC_74xx123_t::SignalQ1bUpdated, this, &IC_74xx123_t::DebugQ1b);
//    connect(this, &IC_74xx123_t::SignalQ2Updated, this, &IC_74xx123_t::DebugQ2);
//    connect(this, &IC_74xx123_t::SignalQ2bUpdated, this, &IC_74xx123_t::DebugQ2b);
}


//
// -- Update everything now that updates are complete
//    -----------------------------------------------
void IC_74xx123_t::ProcessUpdates1Complete(void)
{
    if (pins[A1b] == LOW && pins[B1] == HIGH && pins[RD1b] == HIGH) {
        if (a1 == HIGH || b1 == LOW || rd1 == LOW) {
            if (!cycling1) {
                DEBUG << "Triggering vibrator 1";

                if (pins[A1b] == LOW && a1 == HIGH) DEBUG << "  A transition";
                if (pins[B1] == HIGH && b1 == LOW) DEBUG << "  B transition";
                if (pins[RD1b] == LOW && rd1 == HIGH) DEBUG << "  RD transition";

                pins[Q1] = HIGH;
                pins[Q1b] = LOW;

//                DEBUG << "Setting timer 1";

                emit SignalQ1Updated(HIGH);
                emit SignalQ1bUpdated(LOW);

                cycling1 = true;
                QTimer::singleShot(50, this, &IC_74xx123_t::ProcessUpdateRCEXT1);
            }
        }
    }

    a1 = pins[A1b];
    b1 = pins[B1];
    rd1 = pins[RD1b];

//    DEBUG << "vib1:1 update complete";
//    DEBUG << "  a1 " << a1;
//    DEBUG << "  b1 " << b1;
//    DEBUG << "  rd1 " << rd1;
}


//
// -- Update everything now that updates are complete
//    -----------------------------------------------
void IC_74xx123_t::ProcessUpdates2Complete(void)
{
    if (pins[A2b] == LOW && pins[B2] == HIGH && pins[RD2b] == HIGH) {
        if (a2 == HIGH || b2 == LOW || rd2 == LOW) {
            if (!cycling2) {
                DEBUG << "Triggering vibrator 2";

                if (pins[A2b] == LOW && a2 == HIGH) DEBUG << "  A transition";
                if (pins[B2] == HIGH && b2 == LOW) DEBUG << "  B transition";
                if (pins[RD2b] == LOW && rd2 == HIGH) DEBUG << "  RD transition";

                pins[Q2] = HIGH;
                pins[Q2b] = LOW;

//                DEBUG << "Setting timer 2";

                emit SignalQ2Updated(HIGH);
                emit SignalQ2bUpdated(LOW);

                cycling2 = true;
                QTimer::singleShot(50, this, &IC_74xx123_t::ProcessUpdateRCEXT2);
            }
        }
    }

    a2 = pins[A2b];
    b2 = pins[B2];
    rd2 = pins[RD2b];

//    DEBUG << "vib1:2 update complete";
//    DEBUG << "  a2 " << a2;
//    DEBUG << "  b2 " << b2;
//    DEBUG << "  rd2 " << rd2;
}



//
// -- A timeout has occurred, perform the updates
//    -------------------------------------------
void IC_74xx123_t::ProcessUpdateRCEXT1(void)
{
//    DebugRC1();

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
//    DebugRC2();

    pins[Q2] = LOW;
    pins[Q2b] = HIGH;

    cycling2 = false;

    emit SignalQ2Updated(LOW);
    emit SignalQ2bUpdated(HIGH);
}

