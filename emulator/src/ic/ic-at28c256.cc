//===================================================================================================================
// ic-at28c256.cc -- This header file defines an AT28C256 Parallel EEPROM IC
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Aug-17  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-at28c256.moc.cc"



//
// -- Consturct a RAM IC
//    ------------------
IC_at28c256_t::IC_at28c256_t(const QString &file)
{
    pins[A0] = Z;
    pins[A1] = Z;
    pins[A2] = Z;
    pins[A3] = Z;
    pins[A4] = Z;
    pins[A5] = Z;
    pins[A6] = Z;
    pins[A7] = Z;
    pins[A8] = Z;
    pins[A9] = Z;
    pins[A10] = Z;
    pins[A11] = Z;
    pins[A12] = Z;
    pins[A13] = Z;
    pins[A14] = Z;
    pins[CEb] = lastCE = HIGH;
    pins[OEb] = lastOE = HIGH;

    pins[DQ0] = Z;
    pins[DQ1] = Z;
    pins[DQ2] = Z;
    pins[DQ3] = Z;
    pins[DQ4] = Z;
    pins[DQ5] = Z;
    pins[DQ6] = Z;
    pins[DQ7] = Z;

    outputValue = 0;

    memset(contents, -1, 32*1024);


    //
    // -- read the contents of the EEPROM from disk
    //    -----------------------------------------
    FILE *fp = fopen(file.toStdString().c_str(), "r");

    if (!fp) {
        qDebug() << "Unable to open file" << file;
        memset(contents, 0, sizeof(contents));
        return;
    }

    fread(contents, 1, sizeof(contents), fp);
    fclose(fp);

    TriggerFirstUpdate();
}



//
// -- Emit the first signal updates to put everything in sync
//    -------------------------------------------------------
inline void IC_at28c256_t::TriggerFirstUpdate(void)
{
    OutputZ();
}



//
// -- Set all output pins to be High-Z, taking care not to change the input state
//    ---------------------------------------------------------------------------
void IC_at28c256_t::OutputZ(void)
{
    emit SignalDq0Updated(Z);
    emit SignalDq1Updated(Z);
    emit SignalDq2Updated(Z);
    emit SignalDq3Updated(Z);
    emit SignalDq4Updated(Z);
    emit SignalDq5Updated(Z);
    emit SignalDq6Updated(Z);
    emit SignalDq7Updated(Z);
}



//
// -- Process Changes in output values
//    --------------------------------
void IC_at28c256_t::ProcessOutput(void)
{
    TriState_t nq0 = Z;
    TriState_t nq1 = Z;
    TriState_t nq2 = Z;
    TriState_t nq3 = Z;
    TriState_t nq4 = Z;
    TriState_t nq5 = Z;
    TriState_t nq6 = Z;
    TriState_t nq7 = Z;


    // -- read mode -- the only state left: CEb == LOW && OEb == LOW && WEb == HIGH
    int addr =  (pins[A14] == HIGH ? (1<<14) : 0) |
                (pins[A13] == HIGH ? (1<<13) : 0) |
                (pins[A12] == HIGH ? (1<<12) : 0) |
                (pins[A11] == HIGH ? (1<<11) : 0) |
                (pins[A10] == HIGH ? (1<<10) : 0) |
                (pins[ A9] == HIGH ? (1<< 9) : 0) |
                (pins[ A8] == HIGH ? (1<< 8) : 0) |
                (pins[ A7] == HIGH ? (1<< 7) : 0) |
                (pins[ A6] == HIGH ? (1<< 6) : 0) |
                (pins[ A5] == HIGH ? (1<< 5) : 0) |
                (pins[ A4] == HIGH ? (1<< 4) : 0) |
                (pins[ A3] == HIGH ? (1<< 3) : 0) |
                (pins[ A2] == HIGH ? (1<< 2) : 0) |
                (pins[ A1] == HIGH ? (1<< 1) : 0) |
                (pins[ A0] == HIGH ? (1<< 0) : 0);


    outputValue = contents[addr];


    nq7 = ((outputValue & (1<<7)) != 0) ? HIGH : LOW;
    nq6 = ((outputValue & (1<<6)) != 0) ? HIGH : LOW;
    nq5 = ((outputValue & (1<<5)) != 0) ? HIGH : LOW;
    nq4 = ((outputValue & (1<<4)) != 0) ? HIGH : LOW;
    nq3 = ((outputValue & (1<<3)) != 0) ? HIGH : LOW;
    nq2 = ((outputValue & (1<<2)) != 0) ? HIGH : LOW;
    nq1 = ((outputValue & (1<<1)) != 0) ? HIGH : LOW;
    nq0 = ((outputValue & (1<<0)) != 0) ? HIGH : LOW;


    emit SignalDq0Updated(nq0);
    emit SignalDq1Updated(nq1);
    emit SignalDq2Updated(nq2);
    emit SignalDq3Updated(nq3);
    emit SignalDq4Updated(nq4);
    emit SignalDq5Updated(nq5);
    emit SignalDq6Updated(nq6);
    emit SignalDq7Updated(nq7);
}



//
// -- This function handles all the business of setting the proper state after an update
//
//    There are 8 possible states for all the `Enable` lines.  These are here with their
//    respective actions:
//
//    | #CE | #OE | Action          |
//    |:---:|:---:|:----------------|
//    |  L  |  L  | ProcessOutput() |
//    |  L  | !L  | OutputZ()       |
//    | !L  |  L  | OutputZ()       |
//    | !L  | !L  | OutputZ()       |
//
//    ----------------------------------------------------------------------------------
void IC_at28c256_t::UpdateAll(void)
{
    lastCE = pins[CEb];
    lastOE = pins[OEb];


    if (pins[CEb] == LOW && pins[OEb] == LOW) {
        ProcessOutput();
        return;
    } else {
        OutputZ();
        return;
    }
}



