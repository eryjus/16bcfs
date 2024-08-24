//===================================================================================================================
// ic-74xx574.cc -- This class implements a 74xx574 Octal D-Type Latch
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-30  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/ic-74xx574.moc.cc"


//
// -- This is the contructor for the D-Type Latch
//    -------------------------------------------
IC_74xx574_t::IC_74xx574_t()
{
    pins[OEb] = HIGH;
    pins[D1] = LOW;
    pins[D2] = LOW;
    pins[D3] = LOW;
    pins[D4] = LOW;
    pins[D5] = LOW;
    pins[D6] = LOW;
    pins[D7] = LOW;
    pins[D8] = LOW;
    pins[CLK] = LOW;
    pins[Q8] = Z;
    pins[Q7] = Z;
    pins[Q6] = Z;
    pins[Q5] = Z;
    pins[Q4] = Z;
    pins[Q3] = Z;
    pins[Q2] = Z;
    pins[Q1] = Z;
}


//
// -- perform the initial calculation of state and push the results out to all subscribers
//    ------------------------------------------------------------------------------------
void IC_74xx574_t::TriggerFirstUpdate(void)
{
    pins[Q1] = pins[D1];
    pins[Q2] = pins[D2];
    pins[Q3] = pins[D3];
    pins[Q4] = pins[D4];
    pins[Q5] = pins[D5];
    pins[Q6] = pins[D6];
    pins[Q7] = pins[D7];
    pins[Q8] = pins[D8];


    emit SignalQ1Updated(pins[Q1]);
    emit SignalQ2Updated(pins[Q2]);
    emit SignalQ3Updated(pins[Q3]);
    emit SignalQ4Updated(pins[Q4]);
    emit SignalQ5Updated(pins[Q5]);
    emit SignalQ6Updated(pins[Q6]);
    emit SignalQ7Updated(pins[Q7]);
    emit SignalQ8Updated(pins[Q8]);
}


//
// -- change the state of the output enable pin, which requires immediate action
//    --------------------------------------------------------------------------
void IC_74xx574_t::ProcessUpdateOE(TriState_t state)
{
    pins[OEb] = state;

    emit SignalQ1Updated(pins[OEb]==HIGH?Z:pins[Q1]);
    emit SignalQ2Updated(pins[OEb]==HIGH?Z:pins[Q2]);
    emit SignalQ3Updated(pins[OEb]==HIGH?Z:pins[Q3]);
    emit SignalQ4Updated(pins[OEb]==HIGH?Z:pins[Q4]);
    emit SignalQ5Updated(pins[OEb]==HIGH?Z:pins[Q5]);
    emit SignalQ6Updated(pins[OEb]==HIGH?Z:pins[Q6]);
    emit SignalQ7Updated(pins[OEb]==HIGH?Z:pins[Q7]);
    emit SignalQ8Updated(pins[OEb]==HIGH?Z:pins[Q8]);
}


//
// -- latch the state of the
//    --------------------------------------------------------------------------
void IC_74xx574_t::ProcessUpdateClk(TriState_t state)
{
    static int iter = 0;

    pins[CLK] = state;

    if (state == HIGH) {
        pins[Q1] = pins[D1];
        pins[Q2] = pins[D2];
        pins[Q3] = pins[D3];
        pins[Q4] = pins[D4];
        pins[Q5] = pins[D5];
        pins[Q6] = pins[D6];
        pins[Q7] = pins[D7];
        pins[Q8] = pins[D8];

        emit SignalQ1Updated((pins[OEb]==HIGH)?Z:pins[Q1]);
        emit SignalQ2Updated((pins[OEb]==HIGH)?Z:pins[Q2]);
        emit SignalQ3Updated((pins[OEb]==HIGH)?Z:pins[Q3]);
        emit SignalQ4Updated((pins[OEb]==HIGH)?Z:pins[Q4]);
        emit SignalQ5Updated((pins[OEb]==HIGH)?Z:pins[Q5]);
        emit SignalQ6Updated((pins[OEb]==HIGH)?Z:pins[Q6]);
        emit SignalQ7Updated((pins[OEb]==HIGH)?Z:pins[Q7]);
        emit SignalQ8Updated((pins[OEb]==HIGH)?Z:pins[Q8]);


        if (objectName() == "debug") {
            qDebug() << ++ iter << "Asserting new value" << Qt::hex <<
                (pins[Q8] << 7 | pins[Q7] << 6 | pins[Q6] << 5 | pins[Q5] << 4 |
                pins[Q4] << 3 | pins[Q3] << 2 | pins[Q2] << 1 | pins[Q1] << 0);
        }

    }
}


