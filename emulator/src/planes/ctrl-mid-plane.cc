//===================================================================================================================
//  ctrl-mid-plane.cc -- This is the mid-plane for the control logic
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



#include "16bcfs.hh"
#include "../moc/ctrl-mid-plane.moc.cc"



//
// -- Local copies of the modules
//    ---------------------------
CtrlRomCtrlModule_t *ControlLogic_MidPlane_t::ctrlCtrl = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl0 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl1 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl2 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl3 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl4 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl5 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl6 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl7 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl8 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrl9 = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrla = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrlb = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrlc = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrld = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrle = nullptr;
CtrlRomModule_t *ControlLogic_MidPlane_t::ctrlf = nullptr;

SubDemux3_t *ControlLogic_MidPlane_t::addr1Demux = nullptr;
SubDemux4_t *ControlLogic_MidPlane_t::addr2Demux = nullptr;
SubDemux4_t *ControlLogic_MidPlane_t::aluADemux = nullptr;
SubDemux4_t *ControlLogic_MidPlane_t::aluBDemux = nullptr;
SubDemux7_t *ControlLogic_MidPlane_t::mainDemux = nullptr;
SubDemux3_t *ControlLogic_MidPlane_t::carrySelectDemux = nullptr;
SubDemux3_t *ControlLogic_MidPlane_t::shiftDemux = nullptr;

HW_Bus_16_t *ControlLogic_MidPlane_t::ctrlBus = nullptr;



//
// -- construct a new Control Logic Mid-Plane
//    ---------------------------------------
ControlLogic_MidPlane_t::ControlLogic_MidPlane_t(void) : QGroupBox("Control ROM Mid-Plane")
{
    AllocateComponents();
    BuildGui();
    WireUp();
    TriggerFirstUpdate();

    ctrlBus->setObjectName("debug");
}



//
// -- Allocate all of the modules supported by this mid-plane
//    -------------------------------------------------------
void ControlLogic_MidPlane_t::AllocateComponents(void)
{
    ctrlBus = new HW_Bus_16_t("Ctrl Lookup", HW_Computer_t::GetClock());

    ctrlCtrl = new CtrlRomCtrlModule_t;
    ctrl0 = new CtrlRomModule_t("Ctrl0", "ctrl0.bin");
    ctrl1 = new CtrlRomModule_t("Ctrl1", "ctrl1.bin");
    ctrl2 = new CtrlRomModule_t("Ctrl2", "ctrl2.bin");
    ctrl3 = new CtrlRomModule_t("Ctrl3", "ctrl3.bin");
    ctrl4 = new CtrlRomModule_t("Ctrl4", "ctrl4.bin");
    ctrl5 = new CtrlRomModule_t("Ctrl5", "ctrl5.bin");
    ctrl6 = new CtrlRomModule_t("Ctrl6", "ctrl6.bin");
    ctrl7 = new CtrlRomModule_t("Ctrl7", "ctrl7.bin");
    ctrl8 = new CtrlRomModule_t("Ctrl8", "ctrl8.bin");
    ctrl9 = new CtrlRomModule_t("Ctrl9", "ctrl9.bin");
    ctrla = new CtrlRomModule_t("CtrlA", "ctrla.bin");
    ctrlb = new CtrlRomModule_t("CtrlB", "ctrlb.bin");
    ctrlc = new CtrlRomModule_t("CtrlC", "ctrlc.bin");
    ctrld = new CtrlRomModule_t("CtrlD", "ctrld.bin");
    ctrle = new CtrlRomModule_t("CtrlE", "ctrle.bin");
    ctrlf = new CtrlRomModule_t("CtrlF", "ctrlf.bin");

    addr1Demux = new SubDemux3_t;
    addr2Demux = new SubDemux4_t;
    aluADemux = new SubDemux4_t;
    aluBDemux = new SubDemux4_t;
    mainDemux = new SubDemux7_t;
    carrySelectDemux = new SubDemux3_t;
    shiftDemux = new SubDemux3_t;
}



void ControlLogic_MidPlane_t::BuildGui(void)
{
    // -- the layout for this module will be a grid
    QGridLayout *layout = new QGridLayout;
    layout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->setContentsMargins(0, 0, 0, 0);
    setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layout->setSpacing(1);
    setLayout(layout);

//  -- before this can be enabled, we need to set a style sheet for just about every element
//    setStyleSheet("background-color: #d0d0d0");

    layout->addWidget(ctrlCtrl, 0, 0, 1, 2);
    layout->addWidget(ctrl0, 1, 0, 1, 1);
    layout->addWidget(ctrl1, 2, 0, 1, 1);
    layout->addWidget(ctrl2, 3, 0, 1, 1);
    layout->addWidget(ctrl3, 4, 0, 1, 1);
    layout->addWidget(ctrl4, 5, 0, 1, 1);
    layout->addWidget(ctrl5, 6, 0, 1, 1);
    layout->addWidget(ctrl6, 7, 0, 1, 1);
    layout->addWidget(ctrl7, 8, 0, 1, 1);
    layout->addWidget(ctrl8, 1, 1, 1, 1);
    layout->addWidget(ctrl9, 2, 1, 1, 1);
    layout->addWidget(ctrla, 3, 1, 1, 1);
    layout->addWidget(ctrlb, 4, 1, 1, 1);
    layout->addWidget(ctrlc, 5, 1, 1, 1);
    layout->addWidget(ctrld, 6, 1, 1, 1);
    layout->addWidget(ctrle, 7, 1, 1, 1);
    layout->addWidget(ctrlf, 8, 1, 1, 1);
}



//
// -- Trigger the first updates on all the components to push the proper signals
//    --------------------------------------------------------------------------
void ControlLogic_MidPlane_t::TriggerFirstUpdate(void)
{
    ctrlCtrl->TriggerFirstUpdate();
    ctrl0->TriggerFirstUpdate();
    ctrl1->TriggerFirstUpdate();
    ctrl2->TriggerFirstUpdate();
    ctrl3->TriggerFirstUpdate();
    ctrl4->TriggerFirstUpdate();
    ctrl5->TriggerFirstUpdate();
    ctrl6->TriggerFirstUpdate();
    ctrl7->TriggerFirstUpdate();
    ctrl8->TriggerFirstUpdate();
    ctrl9->TriggerFirstUpdate();
    ctrla->TriggerFirstUpdate();
    ctrlb->TriggerFirstUpdate();
    ctrlc->TriggerFirstUpdate();
    ctrld->TriggerFirstUpdate();
    ctrle->TriggerFirstUpdate();
    ctrlf->TriggerFirstUpdate();

    addr1Demux->TriggerFirstUpdate();
    addr2Demux->TriggerFirstUpdate();
    aluADemux->TriggerFirstUpdate();
    aluBDemux->TriggerFirstUpdate();
    mainDemux->TriggerFirstUpdate();
    carrySelectDemux->TriggerFirstUpdate();
    shiftDemux->TriggerFirstUpdate();
}



//
// -- Process a reset signal coming from the main back-plane
//    ------------------------------------------------------
inline void ControlLogic_MidPlane_t::ProcessReset(TriState_t state)
{
    ctrlCtrl->ProcessResetUpdate(state);

    ctrl0->ProcessCopyEeprom();
    ctrl1->ProcessCopyEeprom();
    ctrl2->ProcessCopyEeprom();
    ctrl3->ProcessCopyEeprom();
    ctrl4->ProcessCopyEeprom();
    ctrl5->ProcessCopyEeprom();
    ctrl6->ProcessCopyEeprom();
    ctrl7->ProcessCopyEeprom();
    ctrl8->ProcessCopyEeprom();
    ctrl9->ProcessCopyEeprom();
    ctrla->ProcessCopyEeprom();
    ctrlb->ProcessCopyEeprom();
    ctrlc->ProcessCopyEeprom();
    ctrld->ProcessCopyEeprom();
    ctrle->ProcessCopyEeprom();
    ctrlf->ProcessCopyEeprom();
}



//
// -- Process a clock tick from the main clock
//    ----------------------------------------
void ControlLogic_MidPlane_t::ProcessCpuClockLatch(TriState_t state)
{
    ctrl0->ProcessUpdateClockLatch(state);
    ctrl1->ProcessUpdateClockLatch(state);
    ctrl2->ProcessUpdateClockLatch(state);
    ctrl3->ProcessUpdateClockLatch(state);
    ctrl4->ProcessUpdateClockLatch(state);
    ctrl5->ProcessUpdateClockLatch(state);
    ctrl6->ProcessUpdateClockLatch(state);
    ctrl7->ProcessUpdateClockLatch(state);
    ctrl8->ProcessUpdateClockLatch(state);
    ctrl9->ProcessUpdateClockLatch(state);
    ctrla->ProcessUpdateClockLatch(state);
    ctrlb->ProcessUpdateClockLatch(state);
    ctrlc->ProcessUpdateClockLatch(state);
    ctrld->ProcessUpdateClockLatch(state);
    ctrle->ProcessUpdateClockLatch(state);
    ctrlf->ProcessUpdateClockLatch(state);
}



//
// -- Process a clock tick from the main clock
//    ----------------------------------------
void ControlLogic_MidPlane_t::ProcessCpuClockOutput(TriState_t state)
{
    ctrl0->ProcessUpdateClockOutput(state);
    ctrl1->ProcessUpdateClockOutput(state);
    ctrl2->ProcessUpdateClockOutput(state);
    ctrl3->ProcessUpdateClockOutput(state);
    ctrl4->ProcessUpdateClockOutput(state);
    ctrl5->ProcessUpdateClockOutput(state);
    ctrl6->ProcessUpdateClockOutput(state);
    ctrl7->ProcessUpdateClockOutput(state);
    ctrl8->ProcessUpdateClockOutput(state);
    ctrl9->ProcessUpdateClockOutput(state);
    ctrla->ProcessUpdateClockOutput(state);
    ctrlb->ProcessUpdateClockOutput(state);
    ctrlc->ProcessUpdateClockOutput(state);
    ctrld->ProcessUpdateClockOutput(state);
    ctrle->ProcessUpdateClockOutput(state);
    ctrlf->ProcessUpdateClockOutput(state);
}



//
// -- Process a clock tick from the main clock
//    ----------------------------------------
inline void ControlLogic_MidPlane_t::ProcessRawSystemClock(TriState_t state)
{
    //
    // -- This is supposed to drive the copy of the pedantic copy.  However, in the emulator, there is a dedicated
    //    clock for this.  For now, just leave this commented out and if needed I will come back and address
    //    the missing slot in `ctrlCtrl`.
    //    --------------------------------------------------------------------------------------------------------
//    ctrlCtrl->ProcessUpdateRawSystemClock(state);
}



//
// -- Check the SRAM consistency of the whole of the control ROMs
//    -----------------------------------------------------------
void ControlLogic_MidPlane_t::ProcessSanityCheck(TriState_t state)
{
    if (state != LOW) {
        ctrl0->ProcessSanityCheck(state);
        ctrl1->ProcessSanityCheck(state);
        ctrl2->ProcessSanityCheck(state);
        ctrl3->ProcessSanityCheck(state);
        ctrl4->ProcessSanityCheck(state);
        ctrl5->ProcessSanityCheck(state);
        ctrl6->ProcessSanityCheck(state);
        ctrl7->ProcessSanityCheck(state);
        ctrl8->ProcessSanityCheck(state);
        ctrl9->ProcessSanityCheck(state);
        ctrla->ProcessSanityCheck(state);
        ctrlb->ProcessSanityCheck(state);
        ctrlc->ProcessSanityCheck(state);
        ctrld->ProcessSanityCheck(state);
        ctrle->ProcessSanityCheck(state);
        ctrlf->ProcessSanityCheck(state);
    }
}


//
// -- for any given control ROM module, wire up its inputs
//    ----------------------------------------------------
void ControlLogic_MidPlane_t::WireUpControlROM(CtrlRomModule_t *rom, CtrlRomCtrlModule_t *ctrl)
{
    connect(ctrl, &CtrlRomCtrlModule_t::SignalEepromCsUpdated, rom, &CtrlRomModule_t::ProcessUpdateChipSelect, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalEepromCmdAddrUpdated, rom, &CtrlRomModule_t::ProcessUpdateCmdAddr, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalQcUpdated, rom, &CtrlRomModule_t::ProcessUpdateLatchOe, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalQcbUpdated, rom, &CtrlRomModule_t::ProcessUpdateDriverOe, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalQrbUpdated, rom, &CtrlRomModule_t::ProcessUpdateClear, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalShiftClockUpdated, rom, &CtrlRomModule_t::ProcessUpdateShiftClk, CNN_TYPE);
    // cpu clock above
    connect(ctrl, &CtrlRomCtrlModule_t::SignalSramWeUpdated, rom, &CtrlRomModule_t::ProcessUpdateWriteEnable, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalSramOeUpdated, rom, &CtrlRomModule_t::ProcessUpdateOutputEnable, CNN_TYPE);
    connect(ctrl, &CtrlRomCtrlModule_t::SignalSramCeUpdated, rom, &CtrlRomModule_t::ProcessUpdateChipEnable, CNN_TYPE);
}


//
// -- connect up all the "wires" to the different modules and sub-mudules
//    -------------------------------------------------------------------
void ControlLogic_MidPlane_t::WireUp(void)
{
    // -- Control ROM Control Module inputs
    // Raw System Clock (above)
    // Reset (above)


    // -- Control ROM Modules
    WireUpControlROM(ctrl0, ctrlCtrl);
    WireUpControlROM(ctrl1, ctrlCtrl);
    WireUpControlROM(ctrl2, ctrlCtrl);
    WireUpControlROM(ctrl3, ctrlCtrl);
    WireUpControlROM(ctrl4, ctrlCtrl);
    WireUpControlROM(ctrl5, ctrlCtrl);
    WireUpControlROM(ctrl6, ctrlCtrl);
    WireUpControlROM(ctrl7, ctrlCtrl);
    WireUpControlROM(ctrl8, ctrlCtrl);
    WireUpControlROM(ctrl9, ctrlCtrl);
    WireUpControlROM(ctrla, ctrlCtrl);
    WireUpControlROM(ctrlb, ctrlCtrl);
    WireUpControlROM(ctrlc, ctrlCtrl);
    WireUpControlROM(ctrld, ctrlCtrl);
    WireUpControlROM(ctrle, ctrlCtrl);
    WireUpControlROM(ctrlf, ctrlCtrl);


    // -- Demux sub-modules; ctrl0
    connect(ctrl0, &CtrlRomModule_t::SignalBit0Updated, addr1Demux, &SubDemux3_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit1Updated, addr1Demux, &SubDemux3_t::ProcessUpdateB, CNN_TYPE);
    addr1Demux->ProcessUpdateC(LOW);

    // -- ctrl1
    connect(ctrl1, &CtrlRomModule_t::SignalBit0Updated, addr2Demux, &SubDemux4_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit1Updated, addr2Demux, &SubDemux4_t::ProcessUpdateB, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit2Updated, addr2Demux, &SubDemux4_t::ProcessUpdateC, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit3Updated, addr2Demux, &SubDemux4_t::ProcessUpdateD, CNN_TYPE);

    // -- ctrl2
    connect(ctrl2, &CtrlRomModule_t::SignalBit0Updated, aluADemux, &SubDemux4_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit1Updated, aluADemux, &SubDemux4_t::ProcessUpdateB, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit2Updated, aluADemux, &SubDemux4_t::ProcessUpdateC, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit3Updated, aluADemux, &SubDemux4_t::ProcessUpdateD, CNN_TYPE);

    // -- ctrl3
    connect(ctrl3, &CtrlRomModule_t::SignalBit0Updated, aluBDemux, &SubDemux4_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit1Updated, aluBDemux, &SubDemux4_t::ProcessUpdateB, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit2Updated, aluBDemux, &SubDemux4_t::ProcessUpdateC, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit3Updated, aluBDemux, &SubDemux4_t::ProcessUpdateD, CNN_TYPE);

    // -- ctrl4
    connect(ctrl4, &CtrlRomModule_t::SignalBit0Updated, mainDemux, &SubDemux7_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit1Updated, mainDemux, &SubDemux7_t::ProcessUpdateB, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit2Updated, mainDemux, &SubDemux7_t::ProcessUpdateC, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit3Updated, mainDemux, &SubDemux7_t::ProcessUpdateD, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit4Updated, mainDemux, &SubDemux7_t::ProcessUpdateE, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit5Updated, mainDemux, &SubDemux7_t::ProcessUpdateF, CNN_TYPE);
    connect(ctrl4, &CtrlRomModule_t::SignalBit6Updated, mainDemux, &SubDemux7_t::ProcessUpdateG, CNN_TYPE);

    // -- ctrl5
    connect(ctrl5, &CtrlRomModule_t::SignalBit0Updated, carrySelectDemux, &SubDemux3_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl5, &CtrlRomModule_t::SignalBit1Updated, carrySelectDemux, &SubDemux3_t::ProcessUpdateB, CNN_TYPE);
    carrySelectDemux->ProcessUpdateC(LOW);

    // -- ctrl6
    connect(ctrl6, &CtrlRomModule_t::SignalBit0Updated, shiftDemux, &SubDemux3_t::ProcessUpdateA, CNN_TYPE);
    connect(ctrl6, &CtrlRomModule_t::SignalBit1Updated, shiftDemux, &SubDemux3_t::ProcessUpdateB, CNN_TYPE);
    connect(ctrl6, &CtrlRomModule_t::SignalBit2Updated, shiftDemux, &SubDemux3_t::ProcessUpdateC, CNN_TYPE);



    // -- Wire up all the signal outputs for each control rom; ctrl0
    connect(ctrl0, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalUpdateCLC, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalUpdateSTC, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalUpdatePgmZLatch, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalUpdateIntZLatch, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalUpdatePgmCLatch, CNN_TYPE);
    connect(ctrl0, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalUpdateIntCLatch, CNN_TYPE);
    connect(addr1Demux, &SubDemux3_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus1AssertPgmPC, CNN_TYPE);
    connect(addr1Demux, &SubDemux3_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus1AssertPgmRA, CNN_TYPE);
    connect(addr1Demux, &SubDemux3_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus1AssertIntPC, CNN_TYPE);
    connect(addr1Demux, &SubDemux3_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus1AssertIntRA, CNN_TYPE);


    // -- ctrl1
    connect(ctrl1, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalUpdatePgmNLatch, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalUpdateIntNLatch, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalUpdatePgmVLatch, CNN_TYPE);
    connect(ctrl1, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalUpdateIntVLatch, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertNone, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR1, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR2, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR3, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY4Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR4, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY5Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR5, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY6Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR6, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY7Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR7, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY8Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR8, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalY9Updated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR9, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYAUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR10, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYBUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR11, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYCUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertR12, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYDUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertFetch, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYEUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertPgmSP, CNN_TYPE);
    connect(addr2Demux, &SubDemux4_t::SignalYFUpdated, this, &ControlLogic_MidPlane_t::SignalAddrBus2AssertIntSP, CNN_TYPE);


    // -- ctrl2
    connect(ctrl2, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalUpdatePgmLLatch, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalUpdateIntLLatch, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalUpdateCLV, CNN_TYPE);
    connect(ctrl2, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalUpdateSTC, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertNone, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR1, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR2, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR3, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY4Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR4, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY5Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR5, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY6Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR6, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY7Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR7, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY8Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR8, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY9Updated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR9, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYAUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR10, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYBUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR11, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYCUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertR12, CNN_TYPE);
    // 0b1101 is unused
    connect(aluADemux, &SubDemux4_t::SignalYEUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertPgmSP, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYFUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusAAssertIntSP, CNN_TYPE);


    // -- ctrl3
    connect(ctrl3, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalALULogicResultBit3, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalALULogicResultBit2, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalALULogicResultBit1, CNN_TYPE);
    connect(ctrl3, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalALULogicResultBit0, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertNone, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR1, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR2, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR3, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY4Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR4, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY5Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR5, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY6Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR6, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY7Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR7, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY8Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR8, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalY9Updated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR9, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYAUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR10, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYBUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR11, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYCUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertR12, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYCUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertFetch, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYEUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertPgmSP, CNN_TYPE);
    connect(aluADemux, &SubDemux4_t::SignalYFUpdated, this, &ControlLogic_MidPlane_t::SignalALUBusBAssertIntSP, CNN_TYPE);


    // -- ctrl4
    connect(ctrl4, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalInstructionSuppress, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY000Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertNone, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY001Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR1, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY002Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR2, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY003Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR3, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY004Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR4, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY005Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR5, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY006Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR6, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY007Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR7, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY010Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR8, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY011Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR9, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY012Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR10, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY013Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR11, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY014Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertR12, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY015Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertPgmSP, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY016Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertPgmRA, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY017Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertPgmPC, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY020Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertIntSP, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY021Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertIntRA, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY022Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertIntPC, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY023Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertFetch, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY024Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev01, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY025Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev02, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY026Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev03, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY027Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev04, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY030Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev05, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY031Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev06, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY032Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev07, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY033Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev08, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY034Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev09, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY035Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertDev10, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY036Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertALUResult, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY037Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertMemory, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY044Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl01, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY045Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl02, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY046Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl03, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY047Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl04, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY050Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl05, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY051Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl06, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY052Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl07, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY053Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl08, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY054Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl09, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY055Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertCtl10, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY101Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR1, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY102Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR2, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY103Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR3, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY104Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR4, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY105Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR5, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY106Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR6, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY107Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR7, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY110Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR8, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY111Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR9, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY112Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR10, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY113Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR11, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY114Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapR12, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY115Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapPgmSP, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY116Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapPgmRA, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY117Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapPgmPC, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY120Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapIntSP, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY121Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapIntRA, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY122Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapIntPC, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY123Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapFetch, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY124Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev01, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY125Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev02, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY126Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev03, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY127Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev04, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY130Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev05, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY131Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev06, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY132Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev07, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY133Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev08, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY134Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev09, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY135Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapDev10, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY136Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapALUResult, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY137Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapMemory, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY144Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl01, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY145Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl02, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY146Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl03, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY147Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl04, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY150Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl05, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY151Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl06, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY152Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl07, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY153Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl08, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY154Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl09, CNN_TYPE);
    connect(mainDemux, &SubDemux7_t::SignalY155Updated, this, &ControlLogic_MidPlane_t::SignalMainBusAssertSwapCtl10, CNN_TYPE);


    // -- ctrl5
    // Bits 7:2 are unused
    connect(carrySelectDemux, &SubDemux3_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalCarrySelect0, CNN_TYPE);
    connect(carrySelectDemux, &SubDemux3_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalCarrySelectLast, CNN_TYPE);
    connect(carrySelectDemux, &SubDemux3_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalCarrySelectInverted, CNN_TYPE);
    connect(carrySelectDemux, &SubDemux3_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalCarrySelect1, CNN_TYPE);


    // -- ctrl6
    // Bits 7:6 are unused
    connect(ctrl6, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalBreak, CNN_TYPE);
    connect(ctrl6, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalLeaveIntContext, CNN_TYPE);
    connect(ctrl6, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalFetchSuppress, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY0Updated, this, &ControlLogic_MidPlane_t::SignalAluShiftNone, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY1Updated, this, &ControlLogic_MidPlane_t::SignalAluArighShiftLeft, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY2Updated, this, &ControlLogic_MidPlane_t::SignalAluArighShiftRight, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY3Updated, this, &ControlLogic_MidPlane_t::SignalAluLogicShiftLeft, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY4Updated, this, &ControlLogic_MidPlane_t::SignalAluRotateCarryLeft, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY5Updated, this, &ControlLogic_MidPlane_t::SignalAluRotateCarryRight, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY6Updated, this, &ControlLogic_MidPlane_t::SignalAluRotateLeft, CNN_TYPE);
    connect(shiftDemux, &SubDemux3_t::SignalY7Updated, this, &ControlLogic_MidPlane_t::SignalAluRotateRight, CNN_TYPE);


    // -- ctrl7
    connect(ctrl7, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalPgmPCInc, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalPgmPCLoad, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalPgmRAInc, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalPgmRALoad, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalIntPCInc, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalIntPCLoad, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalIntRAInc, CNN_TYPE);
    connect(ctrl7, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalIntRALoad, CNN_TYPE);


    // -- ctrl8
    connect(ctrl8, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalPgmSPLoad, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalPgmSPInc, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalPgmSPDec, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalIntSPLoad, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalIntSPInc, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalIntSPDec, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalAluSubtract, CNN_TYPE);
    connect(ctrl8, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalMemoryWrite, CNN_TYPE);


    // -- ctrl9
    connect(ctrl9, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalR1Load, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SingalR1Inc, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SingalR1Dec, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalR2Load, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SingalR2Inc, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SingalR2Dec, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalR3Load, CNN_TYPE);
    connect(ctrl9, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SingalR3Inc, CNN_TYPE);


    // -- ctrl10
    connect(ctrla, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SingalR3Dec, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalR4Load, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SingalR4Inc, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SingalR4Dec, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalR5Load, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SingalR5Inc, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SingalR5Dec, CNN_TYPE);
    connect(ctrla, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalR6Load, CNN_TYPE);


    // -- ctrl11
    connect(ctrlb, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SingalR6Inc, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SingalR6Dec, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalR7Load, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SingalR7Inc, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SingalR7Dec, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalR8Load, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SingalR8Inc, CNN_TYPE);
    connect(ctrlb, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SingalR8Dec, CNN_TYPE);


    // -- ctrl12
    connect(ctrlc, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalR9Load, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SingalR9Inc, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SingalR9Dec, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalR10Load, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SingalR10Inc, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SingalR10Dec, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalR11Load, CNN_TYPE);
    connect(ctrlc, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SingalR11Inc, CNN_TYPE);


    // -- ctrl13
    connect(ctrld, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SingalR11Dec, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalR12Load, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SingalR12Inc, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SingalR12Dec, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalDev01Load, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalCtl01Load, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalDev02Load, CNN_TYPE);
    connect(ctrld, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalCtl02Load, CNN_TYPE);


    // -- ctrl14
    connect(ctrle, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalDev03Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalCtl03Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalDev04Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalCtl04Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalDev05Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalCtl05Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalDev06Load, CNN_TYPE);
    connect(ctrle, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalCtl06Load, CNN_TYPE);


    // -- ctrl15
    connect(ctrlf, &CtrlRomModule_t::SignalBit7Updated, this, &ControlLogic_MidPlane_t::SignalDev07Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit6Updated, this, &ControlLogic_MidPlane_t::SignalCtl07Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit5Updated, this, &ControlLogic_MidPlane_t::SignalDev08Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit4Updated, this, &ControlLogic_MidPlane_t::SignalCtl08Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit3Updated, this, &ControlLogic_MidPlane_t::SignalDev09Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit2Updated, this, &ControlLogic_MidPlane_t::SignalCtl09Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit1Updated, this, &ControlLogic_MidPlane_t::SignalDev10Load, CNN_TYPE);
    connect(ctrlf, &CtrlRomModule_t::SignalBit0Updated, this, &ControlLogic_MidPlane_t::SignalCtl10Load, CNN_TYPE);


    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateLatch, this, &ControlLogic_MidPlane_t::ProcessCpuClockLatch, CNN_TYPE);
    connect(HW_Computer_t::GetClock(), &ClockModule_t::SignalClockStateOutput, this, &ControlLogic_MidPlane_t::ProcessCpuClockOutput, CNN_TYPE);
}


