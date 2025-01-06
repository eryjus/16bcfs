//===================================================================================================================
//  mod-fetch-register.hh -- This class represents the entirety of the Fetch Register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct a fetch-register module.  Ideally, this class
//  would correlate 1:1 against the Fetch-Register Module KiCad design and its resulting PCB.  The interface point
//  will only be the inputs from and outputs to the computer backplane.  All other connections in real hardware are
//  represented here.
//
//  Note that several components in this module will be connected directly to the Fetch Bus on the backplane.  This
//  is because there will be nothing but pin headers between the Fetch Bus and these components.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jun-08  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- The Fetch Register Module class.  It will be subclassed from QGroupBox
//    ----------------------------------------------------------------------
class FetchRegisterModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- this module contains the following components
    IC_74xx574_t *instrRegBus0;
    IC_74xx574_t *instrRegBus1;

    IC_74xx574_t *aluB0;
    IC_74xx574_t *aluB1;

    IC_74xx574_t *addr20;
    IC_74xx574_t *addr21;

    IC_74xx574_t *main0;
    IC_74xx574_t *main1;

    IC_74xx574_t *led0;
    IC_74xx574_t *led1;

    IC_74xx04_t *inv1;

    GUI_Led_t *bit0;
    GUI_Led_t *bit1;
    GUI_Led_t *bit2;
    GUI_Led_t *bit3;
    GUI_Led_t *bit4;
    GUI_Led_t *bit5;
    GUI_Led_t *bit6;
    GUI_Led_t *bit7;
    GUI_Led_t *bit8;
    GUI_Led_t *bit9;
    GUI_Led_t *bitA;
    GUI_Led_t *bitB;
    GUI_Led_t *bitC;
    GUI_Led_t *bitD;
    GUI_Led_t *bitE;
    GUI_Led_t *bitF;

    GUI_Led_t *assertMain;
    GUI_Led_t *assertAluB;
    GUI_Led_t *assertAddr2;
    GUI_Led_t *instrSuppress;



public slots:
    // -- these functions become the external inputs into this module from the backplane
    void ProcessReset(TriState_t state) { /* This board will take #RESET as a signal but will do nothing with it */ }
    void ProcessClockLatch(TriState_t state);
    void ProcessClockOutput(TriState_t state);
    void ProcessAssertMain(TriState_t state);
    void ProcessAssertAluB(TriState_t state);
    void ProcessAssertAddr2(TriState_t state);
    void ProcessInstructionSuppress(TriState_t state);


public:
    // -- constructor/destructor
    explicit FetchRegisterModule_t(void);
    virtual ~FetchRegisterModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};


