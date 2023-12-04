//===================================================================================================================
// ic-74xx193.cc -- This file implementes the a 74xx193 Up/Down Counter
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-25  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx193.moc.cc"


//
// -- This is the contructor for the UP/DOWN cnter, having 16 pins
//    --------------------------------------------------------------
IC_74xx193_t::IC_74xx193_t()
{
    pins[B] = LOW;
    pins[QB] = LOW;
    pins[QA] = LOW;
    pins[DOWN] = HIGH;
    pins[UP] = HIGH;
    pins[QC] = LOW;
    pins[QD] = LOW;
    pins[D] = LOW;
    pins[C] = LOW;
    pins[LOADb] = HIGH;
    pins[COb] = HIGH;
    pins[BOb] = HIGH;
    pins[CLR] = LOW;
    pins[A] = LOW;

    lastUp = LOW;
    lastDown = LOW;

    cnt = 0;
}


//
// -- Update the outputs
//    ------------------
void IC_74xx193_t::ProcessUpdatesComplete(void)
{
    if (pins[UP] == LOW && pins[DOWN] == LOW) {
        qDebug("Invalid state on 74xx193: Both UP and DOWN are LOW at the same time");
    }


    if (pins[CLR] == HIGH) {
        pins[A] = LOW;
        pins[B] = LOW;
        pins[C] = LOW;
        pins[D] = LOW;
        cnt = 0;
    } else {
        if (pins[LOADb] == LOW) {
            cnt =     ((pins[D]==HIGH?1:0) << 3)
                    | ((pins[C]==HIGH?1:0) << 2)
                    | ((pins[B]==HIGH?1:0) << 1)
                    | ((pins[A]==HIGH?1:0) << 0);
        }

        if (pins[UP] == LOW && cnt == 15) {
            // -- Count Up High-to-Low transition: Set COb low
            pins[COb] = LOW;
            emit SignalCoUpdated(LOW);
        }

        if (pins[DOWN] == LOW && cnt == 0) {
            // -- Count Up High-to-Low transition: Set BOb low
            pins[BOb] = LOW;
            emit SignalBoUpdated(LOW);
        }

        if (lastUp == LOW && pins[UP] == HIGH) {
            cnt ++;
            pins[COb] = HIGH;
            emit SignalCoUpdated(HIGH);
        }

        if (lastDown == LOW && pins[DOWN] == HIGH) {
            cnt --;
            pins[BOb] = HIGH;
            emit SignalBoUpdated(HIGH);
        }
    }

    cnt &= 0xf;
    lastUp = pins[UP];
    lastDown = pins[DOWN];

    pins[QA] = (cnt&0b0001?HIGH:LOW);
    pins[QB] = (cnt&0b0010?HIGH:LOW);
    pins[QC] = (cnt&0b0100?HIGH:LOW);
    pins[QD] = (cnt&0b1000?HIGH:LOW);

    emit SignalQaUpdated(pins[QA]);
    emit SignalQbUpdated(pins[QB]);
    emit SignalQcUpdated(pins[QC]);
    emit SignalQdUpdated(pins[QD]);
}


//
// -- perform the initial calculation of state and push the results out to all subscribers
//    ------------------------------------------------------------------------------------
void IC_74xx193_t::TriggerFirstUpdate(void)
{
    ProcessUpdatesComplete();

    emit SignalQaUpdated(pins[QA]);
    emit SignalQbUpdated(pins[QB]);
    emit SignalQcUpdated(pins[QC]);
    emit SignalQdUpdated(pins[QD]);
    emit SignalBoUpdated(pins[BOb]);
    emit SignalCoUpdated(pins[COb]);
}


