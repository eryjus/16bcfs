//===================================================================================================================
// ic-as5lc62256.cc -- This header file defines an AS6C62256 Parallel SRAM IC
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-17  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-as6c62256.moc.cc"



//
// -- Consturct a RAM IC
//    ------------------
IC_as6c62256_t::IC_as6c62256_t(IC_25lc256_t *sanity) : reference(sanity)
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
    pins[WEb] = lastWE = HIGH;

    pins[DQ0] = Z;
    pins[DQ1] = Z;
    pins[DQ2] = Z;
    pins[DQ3] = Z;
    pins[DQ4] = Z;
    pins[DQ5] = Z;
    pins[DQ6] = Z;
    pins[DQ7] = Z;

    hold0 = Z;
    hold1 = Z;
    hold2 = Z;
    hold3 = Z;
    hold4 = Z;
    hold5 = Z;
    hold6 = Z;
    hold7 = Z;

    outputting = false;
    outputValue = 0;

    updating = false;

    memset(contents, -1, 32*1024);

    TriggerFirstUpdate();
}



//
// -- Emit the first signal updates to put everything in sync
//    -------------------------------------------------------
inline void IC_as6c62256_t::TriggerFirstUpdate(void)
{
    OutputZ();
}



//
// -- Set all output pins to be High-Z, taking care not to change the input state
//    ---------------------------------------------------------------------------
void IC_as6c62256_t::OutputZ(void)
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
// -- Process Changes in input values
//    -------------------------------
void IC_as6c62256_t::ProcessInput(void)
{
    pins[DQ0] = hold0;
    pins[DQ1] = hold1;
    pins[DQ2] = hold2;
    pins[DQ3] = hold3;
    pins[DQ4] = hold4;
    pins[DQ5] = hold5;
    pins[DQ6] = hold6;
    pins[DQ7] = hold7;



    //
    // -- Write mode -- update the byte in memory
    //    NOTE: if the address is not static, then several bytes will be updated.  KEEP AN EYE OUT!
    //    -----------------------------------------------------------------------------------------
    uint8_t byte = (pins[DQ7] == HIGH ? (1<<7) : 0) |
                    (pins[DQ6] == HIGH ? (1<<6) : 0) |
                    (pins[DQ5] == HIGH ? (1<<5) : 0) |
                    (pins[DQ4] == HIGH ? (1<<4) : 0) |
                    (pins[DQ3] == HIGH ? (1<<3) : 0) |
                    (pins[DQ2] == HIGH ? (1<<2) : 0) |
                    (pins[DQ1] == HIGH ? (1<<1) : 0) |
                    (pins[DQ0] == HIGH ? (1<<0) : 0);

    int addr = (pins[A14] == HIGH ? (1<<14) : 0) |
                (pins[A13] == HIGH ? (1<<13) : 0) |
                (pins[A12] == HIGH ? (1<<12) : 0) |
                (pins[A11] == HIGH ? (1<<11) : 0) |
                (pins[A10] == HIGH ? (1<<10) : 0) |
                (pins[A9] == HIGH ? (1<<9) : 0) |
                (pins[A8] == HIGH ? (1<<8) : 0) |
                (pins[A7] == HIGH ? (1<<7) : 0) |
                (pins[A6] == HIGH ? (1<<6) : 0) |
                (pins[A5] == HIGH ? (1<<5) : 0) |
                (pins[A4] == HIGH ? (1<<4) : 0) |
                (pins[A3] == HIGH ? (1<<3) : 0) |
                (pins[A2] == HIGH ? (1<<2) : 0) |
                (pins[A1] == HIGH ? (1<<1) : 0) |
                (pins[A0] == HIGH ? (1<<0) : 0);


    contents[addr] = byte;
}



//
// -- Process Changes in output values
//    --------------------------------
void IC_as6c62256_t::ProcessOutput(void)
{
    // -- save the current state
    TriState_t dq0 = (outputValue & 0x01) != 0 ? HIGH : LOW;
    TriState_t dq1 = (outputValue & 0x02) != 0 ? HIGH : LOW;
    TriState_t dq2 = (outputValue & 0x04) != 0 ? HIGH : LOW;
    TriState_t dq3 = (outputValue & 0x08) != 0 ? HIGH : LOW;
    TriState_t dq4 = (outputValue & 0x10) != 0 ? HIGH : LOW;
    TriState_t dq5 = (outputValue & 0x20) != 0 ? HIGH : LOW;
    TriState_t dq6 = (outputValue & 0x40) != 0 ? HIGH : LOW;
    TriState_t dq7 = (outputValue & 0x80) != 0 ? HIGH : LOW;

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

#if !defined(PEDANTIC_COPY) || (PEDANTIC_COPY == 0)
    emit SignalDq0Updated(nq0);
    emit SignalDq1Updated(nq1);
    emit SignalDq2Updated(nq2);
    emit SignalDq3Updated(nq3);
    emit SignalDq4Updated(nq4);
    emit SignalDq5Updated(nq5);
    emit SignalDq6Updated(nq6);
    emit SignalDq7Updated(nq7);
#else
    if (dq0 != nq0) emit SignalDq0Updated(nq0);
    if (dq1 != nq1) emit SignalDq1Updated(nq1);
    if (dq2 != nq2) emit SignalDq2Updated(nq2);
    if (dq3 != nq3) emit SignalDq3Updated(nq3);
    if (dq4 != nq4) emit SignalDq4Updated(nq4);
    if (dq5 != nq5) emit SignalDq5Updated(nq5);
    if (dq6 != nq6) emit SignalDq6Updated(nq6);
    if (dq7 != nq7) emit SignalDq7Updated(nq7);
#endif

    if (objectName() == "debug") {
        qDebug() << "Asserting new value" << Qt::hex << outputValue;
    }
}



//
// -- This function handles all the business of setting the proper state after an update
//
//    There are 8 possible states for all the `Enable` lines.  These are here with their
//    respective actions:
//
//    | #CE | #WE | #OE | Action                       |
//    |:---:|:---:|:---:|:-----------------------------|
//    |  L  |  L  |  L  | ProcessInput()
//    |  L  |  L  | !L  | ProcessInput()
//    |  L  | !L  |  L  | ProcessOutput()
//    |  L  | !L  | !L  | OutputZ()
//    | !L  |  L  |  L  | OutputZ()
//    | !L  |  L  | !L  | OutputZ()
//    | !L  | !L  |  L  | OutputZ()
//    | !L  | !L  | !L  | OutputZ()
//
//    ----------------------------------------------------------------------------------
void IC_as6c62256_t::UpdateAll(void)
{
    if (updating) return;
    updating = true;

    lastCE = pins[CEb];
    lastOE = pins[OEb];
    lastWE = pins[WEb];


    // -- 4 cases handled here
    if (pins[CEb] != LOW) {
        OutputZ();
        updating = false;
        return;
    }

    // -- 1 case handled here
    if (pins[CEb] == LOW && pins[OEb] != LOW && pins[WEb] != LOW) {
        OutputZ();
        updating = false;
        return;
    }

    // -- 2 cases handled here
    if (pins[CEb] == LOW && pins[WEb] == LOW) {
        ProcessInput();
        updating = false;
        return;
    }

    // -- 1 final case handled here
    if (pins[CEb] == LOW && pins[OEb] == LOW && pins[WEb] == HIGH) {
        ProcessOutput();
        updating = false;
        return;
    }

    updating = false;
}



//
// -- Do a sanity check by checking the contents of the RAM against the ROM when enabled
//    ----------------------------------------------------------------------------------
void IC_as6c62256_t::ProcessSanityCheck(QString name)
{
    if (reference == nullptr) return;

    // -- note that this is a friend function to the ROM class definition, so the private members are accessible here
    int loc;
    for (loc = 0; loc < 32 * 1024; loc ++)
    {
        if (reference->contents[loc] != this->contents[loc]) break;
    }

    if (loc != 32 * 1024) {
        qDebug() << "ERROR!!!  " << name << "RAM is no longer congruent with the reference ROM!!!";
        qDebug() << "The first instance with a problem is at index" << loc;
        qDebug() << "Expected" << Qt::hex << reference->contents[loc] << "and read" << Qt::hex << this->contents[loc];
        exit(EXIT_FAILURE);
    } else {
//        qDebug() << "... data is sane.";
    }
}



//
// -- If we are not performing a pedantic copy of the EEPROM to SRAM, knock it out quickly here
//    -----------------------------------------------------------------------------------------
void IC_as6c62256_t::CopyEeprom(void)
{
#if !defined(PEDANTIC_COPY) || (PEDANTIC_COPY == 0)
    if (reference == nullptr) return;

    memcpy(this->contents, reference->contents, 32*1024);
#endif
}


