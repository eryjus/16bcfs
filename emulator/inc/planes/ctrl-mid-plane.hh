//===================================================================================================================
//  ctrl-mid-plane.hh -- This is the mid-plane for the control logic
//
//  This mid-plane is intended to sit between the back-plane and the individual modules, aggregating them into a
//  logical component.  The hope here is that when we get into real hardware, I have a better ability to test,
//  bodge, and maintain the mid-sized modules.
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-May-12  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- This class represents the whole of the control logic, supporting the individual modules and interfacing
//    with the backplane
//    -------------------------------------------------------------------------------------------------------
class ControlLogic_MidPlane_t : public QGroupBox {
    Q_OBJECT

private:
    // -- the Control ROMS
    static CtrlRomCtrlModule_t *ctrlCtrl;
    static CtrlRomModule_t *ctrl0;
    static CtrlRomModule_t *ctrl1;
    static CtrlRomModule_t *ctrl2;
    static CtrlRomModule_t *ctrl3;
    static CtrlRomModule_t *ctrl4;
    static CtrlRomModule_t *ctrl5;
    static CtrlRomModule_t *ctrl6;
    static CtrlRomModule_t *ctrl7;
    static CtrlRomModule_t *ctrl8;
    static CtrlRomModule_t *ctrl9;
    static CtrlRomModule_t *ctrla;
    static CtrlRomModule_t *ctrlb;
    static CtrlRomModule_t *ctrlc;
    static CtrlRomModule_t *ctrld;
    static CtrlRomModule_t *ctrle;
    static CtrlRomModule_t *ctrlf;

    static SubDemux3_t *addr1Demux;
    static SubDemux4_t *addr2Demux;
    static SubDemux4_t *aluADemux;
    static SubDemux4_t *aluBDemux;
    static SubDemux7_t *mainDemux;
    static SubDemux3_t *carrySelectDemux;
    static SubDemux3_t *shiftDemux;

    static HW_Bus_16_t *ctrlBus;



    //
    // -- ***********************************************************************************************************
    //    ***********************************************************************************************************
    //    *****************   PROGRAMMER'S NOTE   *******************************************************************
    //    ***********************************************************************************************************
    //    ***********************************************************************************************************
    //
    //    The only thing which is not included in the emulator are the buses which pass from the individual
    //    modules to the back-plane.  The signals will need to be included in real hardware, but here in the
    //    emulator, the modules themselves will be responsible for reading and asserting to those buses directly.
    //    They are noted here for completeness, not the following lines should not be uncommented.
    //
    //    The alternative would be to duplicate every signal here just to pass it along.  That would be a recipe for
    //    disaster.
    //    ===========================================================================================================
    // -- The Insturuction Bus providing input from the instruction register
    /* static HW_Bus_16_t *instrBus; */

    // -- The Address Copy Bus providing output to the memory ROM->RAM copy
    /* static HW_Bus_16_t *AddrCopyBus; */

    // -- Reset Hold control Bus (1-bit with pull-up) providing a reset-hold signal until reset is complete
    /* static HW_Bus_1_t *rHld; */


public slots:
    void ProcessReset(TriState_t state);            // Main Reset Signal
    void ProcessCpuClock(TriState_t state);         // Cpu Clock
    void ProcessRawSystemClock(TriState_t state);   // Main high-speed system clock
    void ProcessSanityCheck(TriState_t state);      // Check the consistenc of the RAM in the Control Modules


signals:
    // -- Signals from Control ROM 0
    void SignalUpdateCLC(TriState_t state);                         // bit 7
    void SignalUpdateSTC(TriState_t state);                         // bit 6
    void SignalUpdatePgmZLatch(TriState_t state);                   // bit 5
    void SignalUpdateIntZLatch(TriState_t state);                   // bit 4
    void SignalUpdatePgmCLatch(TriState_t state);                   // bit 3
    void SignalUpdateIntCLatch(TriState_t state);                   // bit 2
    void SignalAddrBus1AssertPgmPC(TriState_t state);               // bits 1:0
    void SignalAddrBus1AssertPgmRA(TriState_t state);
    void SignalAddrBus1AssertIntPC(TriState_t state);
    void SignalAddrBus1AssertIntRA(TriState_t state);


    // -- Signals from Control ROM 1
    void SignalUpdatePgmNLatch(TriState_t state);                   // bit 7
    void SignalUpdateIntNLatch(TriState_t state);                   // bit 6
    void SignalUpdatePgmVLatch(TriState_t state);                   // bit 5
    void SignalUpdateIntVLatch(TriState_t state);                   // bit 4
    void SignalAddrBus2AssertNone(TriState_t state);                // bits 3:0
    void SignalAddrBus2AssertR1(TriState_t state);
    void SignalAddrBus2AssertR2(TriState_t state);
    void SignalAddrBus2AssertR3(TriState_t state);
    void SignalAddrBus2AssertR4(TriState_t state);
    void SignalAddrBus2AssertR5(TriState_t state);
    void SignalAddrBus2AssertR6(TriState_t state);
    void SignalAddrBus2AssertR7(TriState_t state);
    void SignalAddrBus2AssertR8(TriState_t state);
    void SignalAddrBus2AssertR9(TriState_t state);
    void SignalAddrBus2AssertR10(TriState_t state);
    void SignalAddrBus2AssertR11(TriState_t state);
    void SignalAddrBus2AssertR12(TriState_t state);
    void SignalAddrBus2AssertFetch(TriState_t state);
    void SignalAddrBus2AssertPgmSP(TriState_t state);
    void SignalAddrBus2AssertIntSP(TriState_t state);


    // -- Signals from Control ROM 2
    void SignalUpdatePgmLLatch(TriState_t state);                   // bit 7
    void SignalUpdateIntLLatch(TriState_t state);                   // bit 6
    void SignalUpdateCLV(TriState_t state);                         // bit 5
    void SignalUpdateSTV(TriState_t state);                         // bit 4
    void SignalALUBusAAssertNone(TriState_t state);                // bits 3:0
    void SignalALUBusAAssertR1(TriState_t state);
    void SignalALUBusAAssertR2(TriState_t state);
    void SignalALUBusAAssertR3(TriState_t state);
    void SignalALUBusAAssertR4(TriState_t state);
    void SignalALUBusAAssertR5(TriState_t state);
    void SignalALUBusAAssertR6(TriState_t state);
    void SignalALUBusAAssertR7(TriState_t state);
    void SignalALUBusAAssertR8(TriState_t state);
    void SignalALUBusAAssertR9(TriState_t state);
    void SignalALUBusAAssertR10(TriState_t state);
    void SignalALUBusAAssertR11(TriState_t state);
    void SignalALUBusAAssertR12(TriState_t state);
    void SignalALUBusAAssertPgmSP(TriState_t state);
    void SignalALUBusAAssertIntSP(TriState_t state);



    // -- Signals from Control ROM 3
    void SignalALULogicResultBit3(TriState_t state);                // bit 7
    void SignalALULogicResultBit2(TriState_t state);                // bit 6
    void SignalALULogicResultBit1(TriState_t state);                // bit 5
    void SignalALULogicResultBit0(TriState_t state);                // bit 4
    void SignalALUBusBAssertNone(TriState_t state);                 // bits 3:0
    void SignalALUBusBAssertR1(TriState_t state);
    void SignalALUBusBAssertR2(TriState_t state);
    void SignalALUBusBAssertR3(TriState_t state);
    void SignalALUBusBAssertR4(TriState_t state);
    void SignalALUBusBAssertR5(TriState_t state);
    void SignalALUBusBAssertR6(TriState_t state);
    void SignalALUBusBAssertR7(TriState_t state);
    void SignalALUBusBAssertR8(TriState_t state);
    void SignalALUBusBAssertR9(TriState_t state);
    void SignalALUBusBAssertR10(TriState_t state);
    void SignalALUBusBAssertR11(TriState_t state);
    void SignalALUBusBAssertR12(TriState_t state);
    void SignalALUBusBAssertFetch(TriState_t state);
    void SignalALUBusBAssertPgmSP(TriState_t state);
    void SignalALUBusBAssertIntSP(TriState_t state);


    // -- Signals from Control ROM 4
    void SignalInstructionSuppress(TriState_t state);               // bit 7
    void SignalMainBusAssertNone(TriState_t state);                 // bits 6:0
    void SignalMainBusAssertR1(TriState_t state);
    void SignalMainBusAssertR2(TriState_t state);
    void SignalMainBusAssertR3(TriState_t state);
    void SignalMainBusAssertR4(TriState_t state);
    void SignalMainBusAssertR5(TriState_t state);
    void SignalMainBusAssertR6(TriState_t state);
    void SignalMainBusAssertR7(TriState_t state);
    void SignalMainBusAssertR8(TriState_t state);
    void SignalMainBusAssertR9(TriState_t state);
    void SignalMainBusAssertR10(TriState_t state);
    void SignalMainBusAssertR11(TriState_t state);
    void SignalMainBusAssertR12(TriState_t state);
    void SignalMainBusAssertPgmSP(TriState_t state);
    void SignalMainBusAssertPgmRA(TriState_t state);
    void SignalMainBusAssertPgmPC(TriState_t state);
    void SignalMainBusAssertIntSP(TriState_t state);
    void SignalMainBusAssertIntRA(TriState_t state);
    void SignalMainBusAssertIntPC(TriState_t state);
    void SignalMainBusAssertFetch(TriState_t state);
    void SignalMainBusAssertDev01(TriState_t state);
    void SignalMainBusAssertDev02(TriState_t state);
    void SignalMainBusAssertDev03(TriState_t state);
    void SignalMainBusAssertDev04(TriState_t state);
    void SignalMainBusAssertDev05(TriState_t state);
    void SignalMainBusAssertDev06(TriState_t state);
    void SignalMainBusAssertDev07(TriState_t state);
    void SignalMainBusAssertDev08(TriState_t state);
    void SignalMainBusAssertDev09(TriState_t state);
    void SignalMainBusAssertDev10(TriState_t state);
    void SignalMainBusAssertALUResult(TriState_t state);
    void SignalMainBusAssertMemory(TriState_t state);
    void SignalMainBusAssertCtl01(TriState_t state);
    void SignalMainBusAssertCtl02(TriState_t state);
    void SignalMainBusAssertCtl03(TriState_t state);
    void SignalMainBusAssertCtl04(TriState_t state);
    void SignalMainBusAssertCtl05(TriState_t state);
    void SignalMainBusAssertCtl06(TriState_t state);
    void SignalMainBusAssertCtl07(TriState_t state);
    void SignalMainBusAssertCtl08(TriState_t state);
    void SignalMainBusAssertCtl09(TriState_t state);
    void SignalMainBusAssertCtl10(TriState_t state);
    void SignalMainBusAssertSwapR1(TriState_t state);
    void SignalMainBusAssertSwapR2(TriState_t state);
    void SignalMainBusAssertSwapR3(TriState_t state);
    void SignalMainBusAssertSwapR4(TriState_t state);
    void SignalMainBusAssertSwapR5(TriState_t state);
    void SignalMainBusAssertSwapR6(TriState_t state);
    void SignalMainBusAssertSwapR7(TriState_t state);
    void SignalMainBusAssertSwapR8(TriState_t state);
    void SignalMainBusAssertSwapR9(TriState_t state);
    void SignalMainBusAssertSwapR10(TriState_t state);
    void SignalMainBusAssertSwapR11(TriState_t state);
    void SignalMainBusAssertSwapR12(TriState_t state);
    void SignalMainBusAssertSwapPgmSP(TriState_t state);
    void SignalMainBusAssertSwapPgmRA(TriState_t state);
    void SignalMainBusAssertSwapPgmPC(TriState_t state);
    void SignalMainBusAssertSwapIntSP(TriState_t state);
    void SignalMainBusAssertSwapIntRA(TriState_t state);
    void SignalMainBusAssertSwapIntPC(TriState_t state);
    void SignalMainBusAssertSwapFetch(TriState_t state);
    void SignalMainBusAssertSwapDev01(TriState_t state);
    void SignalMainBusAssertSwapDev02(TriState_t state);
    void SignalMainBusAssertSwapDev03(TriState_t state);
    void SignalMainBusAssertSwapDev04(TriState_t state);
    void SignalMainBusAssertSwapDev05(TriState_t state);
    void SignalMainBusAssertSwapDev06(TriState_t state);
    void SignalMainBusAssertSwapDev07(TriState_t state);
    void SignalMainBusAssertSwapDev08(TriState_t state);
    void SignalMainBusAssertSwapDev09(TriState_t state);
    void SignalMainBusAssertSwapDev10(TriState_t state);
    void SignalMainBusAssertSwapALUResult(TriState_t state);
    void SignalMainBusAssertSwapMemory(TriState_t state);
    void SignalMainBusAssertSwapCtl01(TriState_t state);
    void SignalMainBusAssertSwapCtl02(TriState_t state);
    void SignalMainBusAssertSwapCtl03(TriState_t state);
    void SignalMainBusAssertSwapCtl04(TriState_t state);
    void SignalMainBusAssertSwapCtl05(TriState_t state);
    void SignalMainBusAssertSwapCtl06(TriState_t state);
    void SignalMainBusAssertSwapCtl07(TriState_t state);
    void SignalMainBusAssertSwapCtl08(TriState_t state);
    void SignalMainBusAssertSwapCtl09(TriState_t state);
    void SignalMainBusAssertSwapCtl10(TriState_t state);


    // -- Signals from Control ROM 5
                                                                    // bits 7:2 are unused
    void SignalCarrySelect0(TriState_t state);                      // bits 1:0
    void SignalCarrySelectLast(TriState_t state);
    void SignalCarrySelectInverted(TriState_t state);
    void SignalCarrySelect1(TriState_t state);


    // -- Signals from Control ROM 6
                                                                    // bits 7:6 are unused
    void SignalBreak(TriState_t state);                             // bit 5
    void SignalLeaveIntContext(TriState_t state);                   // bit 4
    void SignalFetchSuppress(TriState_t state);                     // bit 3
    void SignalAluShiftNone(TriState_t state);                      // bits 2:0
    void SignalAluArighShiftLeft(TriState_t state);
    void SignalAluArighShiftRight(TriState_t state);
    void SignalAluLogicShiftLeft(TriState_t state);
    void SignalAluRotateCarryLeft(TriState_t state);
    void SignalAluRotateCarryRight(TriState_t state);
    void SignalAluRotateLeft(TriState_t state);
    void SignalAluRotateRight(TriState_t state);


    // -- Signals from Control ROM 7
    void SignalPgmPCInc(TriState_t state);                          // bit 7
    void SignalPgmPCLoad(TriState_t state);                         // bit 6
    void SignalPgmRAInc(TriState_t state);                          // bit 5
    void SignalPgmRALoad(TriState_t state);                         // bit 4
    void SignalIntPCInc(TriState_t state);                          // bit 3
    void SignalIntPCLoad(TriState_t state);                         // bit 2
    void SignalIntRAInc(TriState_t state);                          // bit 1
    void SignalIntRALoad(TriState_t state);                         // bit 0


    // -- Signals from Control ROM 8
    void SignalPgmSPLoad(TriState_t state);                         // bit 7
    void SignalPgmSPInc(TriState_t state);                          // bit 6
    void SignalPgmSPDec(TriState_t state);                          // bit 5
    void SignalIntSPLoad(TriState_t state);                         // bit 4
    void SignalIntSPInc(TriState_t state);                          // bit 3
    void SignalIntSPDec(TriState_t state);                          // bit 2
    void SignalAluSubtract(TriState_t state);                       // bit 1
    void SignalMemoryWrite(TriState_t state);                       // bit 0



    // -- Signals from Control ROM 9
    void SignalR1Load(TriState_t state);                            // bit 7
    void SingalR1Inc(TriState_t state);                             // bit 6
    void SingalR1Dec(TriState_t state);                             // bit 5
    void SignalR2Load(TriState_t state);                            // bit 4
    void SingalR2Inc(TriState_t state);                             // bit 3
    void SingalR2Dec(TriState_t state);                             // bit 2
    void SignalR3Load(TriState_t state);                            // bit 1
    void SingalR3Inc(TriState_t state);                             // bit 0


    // -- Signals from Control ROM 10
    void SingalR3Dec(TriState_t state);                             // bit 7
    void SignalR4Load(TriState_t state);                            // bit 6
    void SingalR4Inc(TriState_t state);                             // bit 5
    void SingalR4Dec(TriState_t state);                             // bit 4
    void SignalR5Load(TriState_t state);                            // bit 3
    void SingalR5Inc(TriState_t state);                             // bit 2
    void SingalR5Dec(TriState_t state);                             // bit 1
    void SignalR6Load(TriState_t state);                            // bit 0


    // -- Signals from Control ROM 11
    void SingalR6Inc(TriState_t state);                             // bit 7
    void SingalR6Dec(TriState_t state);                             // bit 6
    void SignalR7Load(TriState_t state);                            // bit 5
    void SingalR7Inc(TriState_t state);                             // bit 4
    void SingalR7Dec(TriState_t state);                             // bit 3
    void SignalR8Load(TriState_t state);                            // bit 2
    void SingalR8Inc(TriState_t state);                             // bit 1
    void SingalR8Dec(TriState_t state);                             // bit 0


    // -- Signals from Control ROM 12
    void SignalR9Load(TriState_t state);                            // bit 7
    void SingalR9Inc(TriState_t state);                             // bit 6
    void SingalR9Dec(TriState_t state);                             // bit 5
    void SignalR10Load(TriState_t state);                           // bit 4
    void SingalR10Inc(TriState_t state);                            // bit 3
    void SingalR10Dec(TriState_t state);                            // bit 2
    void SignalR11Load(TriState_t state);                           // bit 1
    void SingalR11Inc(TriState_t state);                            // bit 0


    // -- Signals from Control ROM 13
    void SingalR11Dec(TriState_t state);                            // bit 7
    void SignalR12Load(TriState_t state);                           // bit 6
    void SingalR12Inc(TriState_t state);                            // bit 5
    void SingalR12Dec(TriState_t state);                            // bit 4
    void SignalDev01Load(TriState_t state);                         // bit 3
    void SignalCtl01Load(TriState_t state);                         // bit 2
    void SignalDev02Load(TriState_t state);                         // bit 1
    void SignalCtl02Load(TriState_t state);                         // bit 0


    // -- Signals from Control ROM 14
    void SignalDev03Load(TriState_t state);                         // bit 7
    void SignalCtl03Load(TriState_t state);                         // bit 6
    void SignalDev04Load(TriState_t state);                         // bit 5
    void SignalCtl04Load(TriState_t state);                         // bit 4
    void SignalDev05Load(TriState_t state);                         // bit 3
    void SignalCtl05Load(TriState_t state);                         // bit 2
    void SignalDev06Load(TriState_t state);                         // bit 1
    void SignalCtl06Load(TriState_t state);                         // bit 0


    // -- Signals from Control ROM 15
    void SignalDev07Load(TriState_t state);                         // bit 7
    void SignalCtl07Load(TriState_t state);                         // bit 6
    void SignalDev08Load(TriState_t state);                         // bit 5
    void SignalCtl08Load(TriState_t state);                         // bit 4
    void SignalDev09Load(TriState_t state);                         // bit 3
    void SignalCtl09Load(TriState_t state);                         // bit 2
    void SignalDev10Load(TriState_t state);                         // bit 1
    void SignalCtl10Load(TriState_t state);                         // bit 0




public:
    // -- constructor/destructor
    explicit ControlLogic_MidPlane_t(void);
    virtual ~ControlLogic_MidPlane_t() {}


public:
    // -- access functions
    static HW_Bus_16_t *GetCtrlBus(void) { return ctrlBus; }


public:
    void TriggerFirstUpdate(void);          // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // create the GUI
    void WireUp(void);                      // make all the necessary connections
    void WireUpControlROM(CtrlRomModule_t *rom, CtrlRomCtrlModule_t *ctrl);
};


