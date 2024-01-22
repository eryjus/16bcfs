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
IC_AS6C62256_t::IC_AS6C62256_t(IC_25lc256_t *sanity) : reference(sanity)
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
    pins[CEb] = HIGH;
    pins[OEb] = HIGH;
    pins[WEb] = HIGH;

    pins[DQ0] = Z;
    pins[DQ1] = Z;
    pins[DQ2] = Z;
    pins[DQ3] = Z;
    pins[DQ4] = Z;
    pins[DQ5] = Z;
    pins[DQ6] = Z;
    pins[DQ7] = Z;

    TriggerFirstUpdates();
}



//
// -- Emit the first signal updates to put everything in sync
//    -------------------------------------------------------
void IC_AS6C62256_t::TriggerFirstUpdates(void)
{
    emit SignalDq0Updated(pins[DQ0]);
    emit SignalDq1Updated(pins[DQ1]);
    emit SignalDq2Updated(pins[DQ2]);
    emit SignalDq3Updated(pins[DQ3]);
    emit SignalDq4Updated(pins[DQ4]);
    emit SignalDq5Updated(pins[DQ5]);
    emit SignalDq6Updated(pins[DQ6]);
    emit SignalDq7Updated(pins[DQ7]);
}



//
// -- Set all output pins to be High-Z, taking care not to change the input state
//    ---------------------------------------------------------------------------
void IC_AS6C62256_t::OutputZ(void)
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
// -- This function handles all the business of setting the proper state after an update
//    ----------------------------------------------------------------------------------
void IC_AS6C62256_t::UpdateAll(void)
{
    // -- standby mode -- no updates
    if (pins[CEb] != LOW) {
        OutputZ();

        return;
    }

    //
    // -- Write mode -- update the byte in memory
    //    NOTE: if the address is not static, then several bytes will be updated.  KEEP AN EYE OUT!
    //    -----------------------------------------------------------------------------------------
    if (pins[CEb] == LOW && pins[WEb] == LOW) {
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

        return;
    }


    // -- output disable mode -- output highZ
    if (pins[CEb] == LOW && pins[OEb] != LOW && pins[WEb] != LOW) {
        OutputZ();

        return;
    }


    // -- read mode -- the only state left: CEb == LOW && OEb == LOW && WEb == HIGH
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

    uint8_t byte = contents[addr];

    pins[DQ7] = (byte&(1<<7)) ? HIGH : LOW;
    pins[DQ6] = (byte&(1<<6)) ? HIGH : LOW;
    pins[DQ5] = (byte&(1<<5)) ? HIGH : LOW;
    pins[DQ4] = (byte&(1<<4)) ? HIGH : LOW;
    pins[DQ3] = (byte&(1<<3)) ? HIGH : LOW;
    pins[DQ2] = (byte&(1<<2)) ? HIGH : LOW;
    pins[DQ1] = (byte&(1<<1)) ? HIGH : LOW;
    pins[DQ0] = (byte&(1<<0)) ? HIGH : LOW;

    TriggerFirstUpdates();
}



//
// -- Do a sanity check by checking the contents of the RAM against the ROM when enabled
//    ----------------------------------------------------------------------------------
void IC_AS6C62256_t::ProcessSanityCheck(void)
{
    if (reference == nullptr) return;

    // -- note that this is a friend function to the ROM class definition, so the private members are accessible here
    if (memcmp(reference->contents, this->contents, 32 * 1024) != 0) {
        qDebug() << "ERROR!!!  RAM is no longer congruent with the reference ROM!!!";
    }
}



