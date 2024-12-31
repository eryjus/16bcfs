//===================================================================================================================
//  mod-gp-register.hh -- This class represents the entirety of a general purpose register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct a gp-register module.  Ideally, this class would
//  correlate 1:1 against the GP-Register Module KiCad design and its resulting PCB.  The interface point will only
//  be the inputs from and outputs to the computer backplane.  All other connections in real hardware are
//  represented here.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Nov-26  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- The General Purpose Register Module class.  It will be subclassed from QGroupBox
//    --------------------------------------------------------------------------------
class GpRegisterModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- this module contains the following components
    IC_74xx193_t *counter0;
    IC_74xx193_t *counter1;
    IC_74xx193_t *counter2;
    IC_74xx193_t *counter3;

    IC_74xx574_t *aluA0;
    IC_74xx574_t *aluA1;

    IC_74xx574_t *aluB0;
    IC_74xx574_t *aluB1;

    IC_74xx574_t *addr10;
    IC_74xx574_t *addr11;

    IC_74xx574_t *addr20;
    IC_74xx574_t *addr21;

    IC_74xx574_t *main0;
    IC_74xx574_t *main1;

    IC_74xx574_t *swap0;
    IC_74xx574_t *swap1;

    IC_74xx574_t *led0;
    IC_74xx574_t *led1;

    IC_74xx00_t *nand1;
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

    GUI_Led_t *load;
    GUI_Led_t *inc;
    GUI_Led_t *dec;
    GUI_Led_t *assertSwap;
    GUI_Led_t *assertMain;
    GUI_Led_t *assertAluA;
    GUI_Led_t *assertAluB;
    GUI_Led_t *assertAddr1;
    GUI_Led_t *assertAddr2;



public slots:
    // -- these functions become the external inputs into this module from the backplane
    void ProcessReset(TriState_t state);
    void ProcessClockLatch(TriState_t state);
    void ProcessClockOutput(TriState_t state);
    void ProcessLoad(TriState_t state);
    void ProcessInc(TriState_t state);
    void ProcessDec(TriState_t state);
    void ProcessAssertSwap(TriState_t state);
    void ProcessAssertMain(TriState_t state);
    void ProcessAssertAluA(TriState_t state);
    void ProcessAssertAluB(TriState_t state);
    void ProcessAssertAddr1(TriState_t state);
    void ProcessAssertAddr2(TriState_t state);

    void ProcessUpdateMainBit0(TriState_t state) { counter0->ProcessUpdateA(state); }
    void ProcessUpdateMainBit1(TriState_t state) { counter0->ProcessUpdateB(state); }
    void ProcessUpdateMainBit2(TriState_t state) { counter0->ProcessUpdateC(state); }
    void ProcessUpdateMainBit3(TriState_t state) { counter0->ProcessUpdateD(state); }
    void ProcessUpdateMainBit4(TriState_t state) { counter1->ProcessUpdateA(state); }
    void ProcessUpdateMainBit5(TriState_t state) { counter1->ProcessUpdateB(state); }
    void ProcessUpdateMainBit6(TriState_t state) { counter1->ProcessUpdateC(state); }
    void ProcessUpdateMainBit7(TriState_t state) { counter1->ProcessUpdateD(state); }
    void ProcessUpdateMainBit8(TriState_t state) { counter2->ProcessUpdateA(state); }
    void ProcessUpdateMainBit9(TriState_t state) { counter2->ProcessUpdateB(state); }
    void ProcessUpdateMainBitA(TriState_t state) { counter2->ProcessUpdateC(state); }
    void ProcessUpdateMainBitB(TriState_t state) { counter2->ProcessUpdateD(state); }
    void ProcessUpdateMainBitC(TriState_t state) { counter3->ProcessUpdateA(state); }
    void ProcessUpdateMainBitD(TriState_t state) { counter3->ProcessUpdateB(state); }
    void ProcessUpdateMainBitE(TriState_t state) { counter3->ProcessUpdateC(state); }
    void ProcessUpdateMainBitF(TriState_t state) { counter3->ProcessUpdateD(state); }


public:
    // -- constructor/destructor
    explicit GpRegisterModule_t(const QString name);
    virtual ~GpRegisterModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};


