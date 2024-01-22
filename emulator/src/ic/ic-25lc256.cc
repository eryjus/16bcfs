//===================================================================================================================
// ic-25lc256.cc -- This header file defines a 25lc256 32KByte SPI EEPROM
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  CRITICAL NOTE:
//  This IC is only partially emulated.  See the notes in the header for specifics.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-15  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "16bcfs.hh"
#include "../moc/ic-25lc256.moc.cc"



//
// -- construct an EEPROM, loading its contents from disk
//    ---------------------------------------------------
IC_25lc256_t::IC_25lc256_t(const QString &file)
{
    pins[CSb] = HIGH;
    pins[SO] = Z;
    pins[WPb] = HIGH;
    pins[SI] = Z;
    pins[SCK] = LOW;
    pins[HOLDb] = HIGH;

    TriggerFirstUpdate();

    QSettings *settings = HW_Computer_t::GetSettings();
    QString folder = settings->value(key).toString();
    QString fn = folder + "/" + file;
    FILE *fp = fopen(fn.toStdString().c_str(), "r");

    if (!fp) {
        qDebug() << "Unable to open file" << fn;
        memset(contents, 0, sizeof(contents));
        return;
    }

    fread(contents, 1, sizeof(contents), fp);
    fclose(fp);
}



//
// -- Handle the chip select signal
//    -----------------------------
void IC_25lc256_t::ProcessUpdateCs(TriState_t state)
{
    pins[CSb] = state;

    // -- it really doesn't matter what it changes to; reset no matter what
    addr = 0;
    byte = 0;
    bits = 0;
    mode = INSTRUCTION;

    pins[SO] = Z;
    emit SignalSoUpdated(Z);
}


//
// -- Process the Clock input -- the lion's share of what drives this IC
//    ------------------------------------------------------------------
void IC_25lc256_t::ProcessUpdateSck(TriState_t state)
{
    if (pins[CSb] != LOW) return;               // if not active, do nothing

    int currentState = (pins[CSb] == HIGH ? CLK_HI : CLK_LO) | mode | bits;

    switch (currentState) {
    case INSTRUCTION | BIT0 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT0 | CLK_LO:
        bits = BIT1;
        break;

    case INSTRUCTION | BIT1 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT1 | CLK_LO:
        bits = BIT2;
        break;

    case INSTRUCTION | BIT2 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT2 | CLK_LO:
        bits = BIT3;
        break;

    case INSTRUCTION | BIT3 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT3 | CLK_LO:
        bits = BIT4;
        break;

    case INSTRUCTION | BIT4 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT4 | CLK_LO:
        bits = BIT5;
        break;

    case INSTRUCTION | BIT5 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT5 | CLK_LO:
        bits = BIT6;
        break;

    case INSTRUCTION | BIT6 | CLK_HI:
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT6 | CLK_LO:
        break;

    case INSTRUCTION | BIT7 | CLK_HI:
        bits = BIT7;
        byte = (byte << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case INSTRUCTION | BIT7 | CLK_LO:
        if (byte == 0b00000011) mode = ADDRESS_MSB;
        else mode = WRITING;
        bits = BIT0;
        break;


    case ADDRESS_MSB | BIT0 | CLK_HI:
        // the most significant bit of the most significant byte is ignored
        addr = 0;
        break;

    case ADDRESS_MSB | BIT0 | CLK_LO:
        bits = BIT1;
        break;

    case ADDRESS_MSB | BIT1 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT1 | CLK_LO:
        bits = BIT2;
        break;

    case ADDRESS_MSB | BIT2 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT2 | CLK_LO:
        bits = BIT3;
        break;

    case ADDRESS_MSB | BIT3 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT3 | CLK_LO:
        bits = BIT4;
        break;

    case ADDRESS_MSB | BIT4 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT4 | CLK_LO:
        bits = BIT5;
        break;

    case ADDRESS_MSB | BIT5 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT5 | CLK_LO:
        bits = BIT6;
        break;

    case ADDRESS_MSB | BIT6 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT6 | CLK_LO:
        bits = BIT7;
        break;

    case ADDRESS_MSB | BIT7 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_MSB | BIT7 | CLK_LO:
        mode = ADDRESS_LSB;
        bits = BIT0;
        break;


    case ADDRESS_LSB | BIT0 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT0 | CLK_LO:
        bits = BIT1;
        break;

    case ADDRESS_LSB | BIT1 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT1 | CLK_LO:
        bits = BIT2;
        break;

    case ADDRESS_LSB | BIT2 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT2 | CLK_LO:
        bits = BIT3;
        break;

    case ADDRESS_LSB | BIT3 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT3 | CLK_LO:
        bits = BIT4;
        break;

    case ADDRESS_LSB | BIT4 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT4 | CLK_LO:
        bits = BIT5;
        break;

    case ADDRESS_LSB | BIT5 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT5 | CLK_LO:
        bits = BIT6;
        break;

    case ADDRESS_LSB | BIT6 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT6 | CLK_LO:
        bits = BIT7;
        break;

    case ADDRESS_LSB | BIT7 | CLK_HI:
        addr = (addr << 1) | (pins[SI] == LOW ? 0 : 1);
        break;

    case ADDRESS_LSB | BIT7 | CLK_LO:
        mode = READING;
        byte = contents[addr];
        bits = BIT0;
        break;


    case READING | BIT0 | CLK_HI:
        pins[SO] = (byte & (1<<7) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT0 | CLK_LO:
        bits = BIT1;
        break;

    case READING | BIT1 | CLK_HI:
        pins[SO] = (byte & (1<<6) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT1 | CLK_LO:
        bits = BIT2;
        break;

    case READING | BIT2 | CLK_HI:
        pins[SO] = (byte & (1<<5) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT2 | CLK_LO:
        bits = BIT3;
        break;

    case READING | BIT3 | CLK_HI:
        pins[SO] = (byte & (1<<4) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT3 | CLK_LO:
        bits = BIT4;
        break;

    case READING | BIT4 | CLK_HI:
        pins[SO] = (byte & (1<<3) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT4 | CLK_LO:
        bits = BIT5;
        break;

    case READING | BIT5 | CLK_HI:
        pins[SO] = (byte & (1<<2) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT5 | CLK_LO:
        bits = BIT6;
        break;

    case READING | BIT6 | CLK_HI:
        pins[SO] = (byte & (1<<1) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT6 | CLK_LO:
        bits = BIT7;
        break;

    case READING | BIT7 | CLK_HI:
        pins[SO] = (byte & (1<<0) ? HIGH : LOW);
        emit SignalSoUpdated(pins[SO]);
        break;

    case READING | BIT7 | CLK_LO:
        addr ++;
        addr &= 0x7fff;         // make sure it wraps properly
        byte = contents[addr];
        bits = BIT0;
        break;


    case WRITING | BIT0 | CLK_HI:
    case WRITING | BIT0 | CLK_LO:
    case WRITING | BIT1 | CLK_HI:
    case WRITING | BIT1 | CLK_LO:
    case WRITING | BIT2 | CLK_HI:
    case WRITING | BIT2 | CLK_LO:
    case WRITING | BIT3 | CLK_HI:
    case WRITING | BIT3 | CLK_LO:
    case WRITING | BIT4 | CLK_HI:
    case WRITING | BIT4 | CLK_LO:
    case WRITING | BIT5 | CLK_HI:
    case WRITING | BIT5 | CLK_LO:
    case WRITING | BIT6 | CLK_HI:
    case WRITING | BIT6 | CLK_LO:
    case WRITING | BIT7 | CLK_HI:
    case WRITING | BIT7 | CLK_LO:
    default:
        // -- if we get here, we are trying to do something that is not implemented...
        if (currentState & CLK_LO) (++ bits) %= 8;
    }
}