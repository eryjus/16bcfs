//===================================================================================================================
//  mod-instr-register.hh -- This class represents the entirety of the Instruction Register
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct a instruction-register module.  Ideally, this class
//  would correlate 1:1 against the Instruction-Register Module KiCad design and its resulting PCB.  The interface
//  point will only be the inputs from and outputs to the computer backplane.  All other connections in real hardware
//  are represented here.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jun-17  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- The Instruction Register Module class.  It will be subclassed from QGroupBox
//    ----------------------------------------------------------------------------
class InstructionRegisterModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- this module contains the following components
    IC_74xx574_t *led0;
    IC_74xx574_t *led1;

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


public slots:
    void ProcessClockLatch(TriState_t state);
    void ProcessClockOutput(TriState_t state);


public:
    // -- constructor/destructor
    explicit InstructionRegisterModule_t(void);
    virtual ~InstructionRegisterModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};


