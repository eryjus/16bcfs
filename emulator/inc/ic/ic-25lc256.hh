//===================================================================================================================
// ic-25lc256.hh -- This header file defines a 25lc256 32KByte SPI EEPROM
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  CRITICAL NOTE:
//  This IC is only partially emulated.  The #WP and the #HOLD signals are not going to be emulated.  The pins
//  will be defined and accept inputs, but the inputs will have absolutely zero impact on the emualted operation
//  of the EEPROM.  Take special care, as these will not failthfully represent what happens in real life and
//  errors is design may arise from this discrepancy.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-15  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This is the class which will emulate the SPI EEPROM
//    ---------------------------------------------------
class IC_25lc256_t : public QObject {
    friend void IC_as6c62256_t::ProcessSanityCheck(QString name);

#if !defined(PEDANTIC_COPY) || (PEDANTIC_COPY == 0)
    friend void IC_as6c62256_t::CopyEeprom(void);
#endif

    Q_OBJECT



public:
    enum {
        CSb = 1,
        SO = 2,
        WPb = 3,
        SI = 5,
        SCK = 6,
        HOLDb = 7,
    };


private:
    //
    // -- detail out what goes into each of the possible states the EEPROM can be in
    //    (at lease for the READ command we are emulating here)
    //    --------------------------------------------------------------------------
    enum {                      // `mode` to be bitwise or'd with the `bits` field or'd with the clock -- creates FSM
        INSTRUCTION = 0x00,     // receiving an instruction
        ADDRESS_MSB = 0x10,     // receiving a relevant address (MSB)
        ADDRESS_LSB = 0x20,     // receiving a relevant address (LSB)
        READING     = 0x30,     // reading the EEPROM and sending the contents
        WRITING     = 0x40,     // writing the data send to the EEPROM: not implemented

                                // these represent which bit number sent/rcvd -- for easier reading of code
        BIT0        = 0x00,
        BIT1        = 0x01,
        BIT2        = 0x02,
        BIT3        = 0x03,
        BIT4        = 0x04,
        BIT5        = 0x05,
        BIT6        = 0x06,
        BIT7        = 0x07,
                                // 2 clock states possible for each of the above conditions when matrixed together
        CLK_HI      = 0x80,
        CLK_LO      = 0x00,
    };


private:
    TriState_t pins[PIN_CNT(8)];
    uint32_t addr;              // this is the internal address register
    uint8_t byte;               // this is the internal byte register (will be shifted out)
    int bits;                   // this is a bit counter; the value here will always be `(bits % 8)`
    uint8_t contents[32*1024];  // this is the rom image
    int mode;                   // this is what is going on



public:
    IC_25lc256_t(const QString &file);
    virtual ~IC_25lc256_t() {};



public:
    void TriggerFirstUpdate(void) { emit SignalSoUpdated(pins[SO]); }



public slots:
    void ProcessUpdateCs(TriState_t state);
    void ProcessUpdateWp(TriState_t state) { /* !!!UNIMPLEMENTED!!! */ pins[WPb] = state; }
    void ProcessUpdateSi(TriState_t state) { pins[SI] = state; }
    void ProcessUpdateSck(TriState_t state);
    void ProcessUpdateHold(TriState_t state) { /* !!!UNIMPLEMENTED!!! */ pins[HOLDb] = state; }


signals:
    void SignalSoUpdated(TriState_t state);
};


