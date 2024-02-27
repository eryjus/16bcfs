//===================================================================================================================
//  mod-ctrl-ctrl.hh -- This class the control circuit for the Control ROMs, loading from EEPROM to SRAM
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This class contains all the control signals needed to load the Control ROM from EEPROM into SRAM.  Since all
//  12 (up to 16) ROMs will be loaded at the same time, it is adventageous to pull all the repetitive components
//  into a single control module.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-25  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents the whole the Control ROM Control Module
//    --------------------------------------------------------------
class CtrlRomCtrlModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- A D-Type Latch to control the Resetting and copying conditions
    IC_74xx74_t *resetting;



    // -- a NAND gate to handle the SR Latch
    IC_74xx00_t *nand1;
    IC_74xx00_t *nand2;



    // -- several pins require CLK ANDed with Qc
    IC_74xx08_t *and1;
    IC_74xx08_t *and2;
    IC_74xx08_t *and3;



    // -- OR gate to combine several signal options
    IC_74xx32_t *or1;



    // -- we need an open drain AND gate for #RHLD
    IC_74xx03_t *oNand1;



    // -- ac inverter for shaping signals properly
    IC_74xx04_t *inv1;



    // -- we need 4 74xx157 Quad 2:1 Muxes to handle multiplexing the address
    IC_74xx157_t *mux0;
    IC_74xx157_t *mux4;
    IC_74xx157_t *mux8;
    IC_74xx157_t *muxC;



    // -- we need a counter to count 8 bits
    IC_74xx193_t *bits;



    // -- and 4 more counter ICs to count address bits
    IC_74xx193_t *addr0;
    IC_74xx193_t *addr4;
    IC_74xx193_t *addr8;
    IC_74xx193_t *addrC;



    // -- We need a shift register to shift command and address to the EEPROM
    IC_74xx165_t *shift;



    // -- We are going to use our own clock for the copy to make sure it's fast
    HW_Oscillator_t *clock;



    // -- set up for some hardware to check state
    GUI_Led_t *clk;
    GUI_Led_t *qr;
    GUI_Led_t *qc;
    GUI_Led_t *qs;
    GUI_Led_t *ql;

    GUI_Led_t *led0;
    GUI_Led_t *led1;
    GUI_Led_t *led2;
    GUI_Led_t *led3;
    GUI_Led_t *led4;
    GUI_Led_t *led5;
    GUI_Led_t *led6;
    GUI_Led_t *led7;
    GUI_Led_t *led8;
    GUI_Led_t *led9;
    GUI_Led_t *ledA;
    GUI_Led_t *ledB;
    GUI_Led_t *ledC;
    GUI_Led_t *ledD;
    GUI_Led_t *ledE;

    GUI_Led_t *bit0;
    GUI_Led_t *bit1;
    GUI_Led_t *bit2;
    GUI_Led_t *bit3;



public slots:
    // -- here are the external inputs into this module: note the instruction will have a bus
    void ProcessResetUpdate(TriState_t state);


    //
    // -- additional inputs not here:
    //    * Instruction -- from the Instruction Register (15-bit bus)
    //    -----------------------------------------------------------


private slots:
    void ProcessQcb(TriState_t state) { if (state == HIGH) clock->stop(); }



signals:
    // -- here are the outputs from this module into other components
    void SignalQrUpdated(TriState_t state);
    void SignalQrbUpdated(TriState_t state);
    void SignalQcUpdated(TriState_t state);
    void SignalQcbUpdated(TriState_t state);
    void SignalQsUpdated(TriState_t state);
    void SignalQsbUpdated(TriState_t state);

    void SignalShiftClockUpdated(TriState_t state);         // Qc * CLK * Qs
    void SignalEepromCsUpdated(TriState_t state);           // Qr + #Qc
    void SignalEepromCmdAddrUpdated(TriState_t state);      // 74HC165 output

    void SignalSramOeUpdated(TriState_t state);             // Qc
    void SignalSramWeUpdated(TriState_t state);             // Qs
    void SignalSramCeUpdated(TriState_t state);             // Qs + #Qc


    //
    // -- some additional outputs not here:
    //    * #RHLD -- a 1-bit bus
    //    * CTRL -- control address for eeprom lookup
    //    * Address Counter -- for the Program ROM to copy of desired (15-bit bus)
    //    ------------------------------------------------------------------------



public:
    // -- constructor/destructor
    explicit CtrlRomCtrlModule_t(void);
    virtual ~CtrlRomCtrlModule_t() {}



public:
    void TriggerFirstUpdate(void);          // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};



