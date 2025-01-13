//===================================================================================================================
//  mod-alu-flags.hh -- This class represents the entirety of the ALU flags module
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct an ALU flags module.  Ideally, this class would
//  correlate 1:1 against the ALU Flags Module KiCad design and its resulting PCB.  The interface points will only
//  be the inputs from and outputs to the computer backplane.  All other connections in real hardware are
//  represented here.
//
//  The inputs into this module are as follows:
//  * #RESET    all modules get this signal, but will not be used here
//  * CLK       needed for latching the flags individually
//  * X         this will be a hard-wired value in the backplane to indicate which of the 2 Flags Modules this is
//
//
//  The outputs from this module are:
//  * X         the X Flag
//
//
//  The UI components in this module are:
//  * X         the status of the X Flag
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-27  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents the whole of an ALU Flags Module
//    ------------------------------------------------------
class AluFlagsModule_t : public QGroupBox {
    Q_OBJECT


private:
    // -- here we have the X (conteXt) Flag, whereby HIGH == Interrupt context
    TriState_t xState;

    GUI_Led_t *xFlag;

    // -- here is what is needed to calculate the Z Flag
    IC_74xx74_t *zcLatch;
    IC_74xx30_t *zFlagNandMsb;
    IC_74xx30_t *zFlagNandLsb;
    IC_74xx540_t *zFlagInvMsb;
    IC_74xx540_t *zFlagInvLsb;
    IC_74xx02_t *nor1;
    IC_74xx08_t *and1;                          // needed to shape the latch signals

    GUI_Led_t *zFlag;

    // -- here are the extra components needed for the C Flag
    IC_74xx151_t *cMux;
    IC_74xx04_t *inv1;                          // TODO: Look for ways to eliminate this IC

    GUI_Led_t *cFlag;

    // -- here are the additional components for the N flag
    IC_74xx74_t *nvLatch;

    GUI_Led_t *nFlag;

    // -- add the components needed to calculate the V flag
    IC_74xx86_t *xor1;

    GUI_Led_t *vFlag;

    // -- add the components for the L Flag
    IC_74xx74_t *lLatch;
    GUI_Led_t *lFlag;



public slots:
    // -- these functions become the external inputs into this module from the backplane
    void ProcessReset(TriState_t state) {};     // all modules will have a reset, even when it is not needed
    void ProcessClockLatch(TriState_t state);
    void ProcessClockOutput(TriState_t state);
    void ProcessZLatch(TriState_t state);
    void ProcessAdderAssert(TriState_t state);
    void ProcessShifterAssert(TriState_t state);
    void ProcessLogicAssert(TriState_t state);
    void ProcessAdderCarry(TriState_t state);
    void ProcessShifterCarry(TriState_t state);
    void ProcessCLatch(TriState_t state);
    void ProcessSetCarry(TriState_t state);
    void ProcessClearCarry(TriState_t state);
    void ProcessNVLLatch(TriState_t state);
    void ProcessSetOverflow(TriState_t state);
    void ProcessClearOverflow(TriState_t state);



public:
    // -- constructor/destructor
    explicit AluFlagsModule_t(const QString name, TriState_t x = LOW);
    virtual ~AluFlagsModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};

