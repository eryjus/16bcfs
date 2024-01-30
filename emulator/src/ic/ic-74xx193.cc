//===================================================================================================================
// ic-74xx193.cc -- This file implementes the a 74xx193 Up/Down Counter
//
//      Copyright (c) 2023-2024 - Adam Clark
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

    lastUp = pins[UP];
    lastDown = pins[DOWN];

    cnt = 0;
}


//
// -- Update the outputs
//    ------------------
void IC_74xx193_t::ProcessUpdatesComplete(void)
{
    TriState_t newQa;
    TriState_t newQb;
    TriState_t newQc;
    TriState_t newQd;
    TriState_t newCOb;
    TriState_t newBOb;


    if (pins[UP] == LOW && pins[DOWN] == LOW) {
        qDebug("Invalid state on 74xx193: Both UP and DOWN are LOW at the same time");
    }


    if (pins[CLR] == HIGH) {
        TriState_t lastQa = pins[QA];
        TriState_t lastQb = pins[QB];
        TriState_t lastQc = pins[QC];
        TriState_t lastQd = pins[QD];
        TriState_t lastCOb = pins[COb];
        TriState_t lastBOb = pins[BOb];

        lastUp = pins[UP];
        lastDown = pins[DOWN];

        cnt = 0;
        pins[QA] = LOW;
        pins[QB] = LOW;
        pins[QC] = LOW;
        pins[QD] = LOW;
        pins[COb] = HIGH;

        if (pins[DOWN] == LOW) pins[BOb] = LOW;
        else pins[BOb] = HIGH;

        if (pins[QA] != lastQa) emit SignalQaUpdated(pins[QA]);
        if (pins[QB] != lastQb) emit SignalQbUpdated(pins[QB]);
        if (pins[QC] != lastQc) emit SignalQcUpdated(pins[QC]);
        if (pins[QD] != lastQd) emit SignalQdUpdated(pins[QD]);
        if (pins[BOb] != lastBOb) emit SignalBoUpdated(pins[BOb]);
        if (pins[COb] != lastCOb) emit SignalCoUpdated(pins[COb]);

        return;
    }


    if (pins[LOADb] == LOW) {
        cnt =     ((pins[D]==HIGH?1:0) << 3)
                | ((pins[C]==HIGH?1:0) << 2)
                | ((pins[B]==HIGH?1:0) << 1)
                | ((pins[A]==HIGH?1:0) << 0);
    } else {
        if (lastUp == LOW && pins[UP] == HIGH) {
            cnt ++;
        }


        if (lastDown == LOW && pins[DOWN] == HIGH) {
            cnt --;
        }
    }


    cnt &= 0xf;


    if (cnt == 15) {
        newCOb = (pins[UP] == LOW ? LOW : HIGH);
    } else newCOb = HIGH;

    if (cnt == 0) {
        newBOb = (pins[DOWN] == LOW ? LOW : HIGH);
    } else newBOb = HIGH;

    newQa = (cnt&0b0001?HIGH:LOW);
    newQb = (cnt&0b0010?HIGH:LOW);
    newQc = (cnt&0b0100?HIGH:LOW);
    newQd = (cnt&0b1000?HIGH:LOW);


    if (newQa != pins[QA]) {
        pins[QA] = newQa;
        emit SignalQaUpdated(newQa);
    }

    if (newQb != pins[QB]) {
        pins[QB] = newQb;
        emit SignalQbUpdated(newQb);
    }

    if (newQc != pins[QC]) {
        pins[QC] = newQc;
        emit SignalQcUpdated(newQc);
    }

    if (newQd != pins[QD]) {
        pins[QD] = newQd;
        emit SignalQdUpdated(newQd);
    }

    if (newBOb != pins[BOb]) {
        pins[BOb] = newBOb;
        emit SignalBoUpdated(newBOb);
    }

    if (newCOb != pins[COb]) {
        pins[COb] = newCOb;
        emit SignalCoUpdated(newCOb);
    }


    lastUp = pins[UP];
    lastDown = pins[DOWN];
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


