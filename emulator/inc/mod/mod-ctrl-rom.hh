//===================================================================================================================
//  mod-ctrl-rom.hh -- This class the control ROM implementation, with EEPROM and SRAM
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Feb-06  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- This class represents the whole the Control ROM Module
//    ------------------------------------------------------
class CtrlRomModule_t : public QGroupBox {
    CtrlRomModule_t(void) = delete;
    Q_OBJECT


private:
    const QString filename;

    IC_25lc256_t *eeprom;
    IC_as6c62256_t *sram;
    IC_74xx164_t *shift;
    IC_74xx574_t *latch;
    IC_74xx541_t *driver;

    HW_Bus_8_t *ctrl;

    GUI_Led_t *led0;
    GUI_Led_t *led1;
    GUI_Led_t *led2;
    GUI_Led_t *led3;
    GUI_Led_t *led4;
    GUI_Led_t *led5;
    GUI_Led_t *led6;
    GUI_Led_t *led7;



public slots:
    void ProcessUpdateClockLatch(TriState_t state) { latch->ProcessUpdateClockLatch(state); }
    void ProcessUpdateClockOutput(TriState_t state) { latch->ProcessUpdateClockOutput(state); }

    void ProcessUpdateCmdAddr(TriState_t state) { eeprom->ProcessUpdateSi(state); }
    void ProcessUpdateChipSelect(TriState_t state) { eeprom->ProcessUpdateCs(state); }

    void ProcessUpdateChipEnable(TriState_t state) { sram->ProcessUpdateCE(state); }
    void ProcessUpdateWriteEnable(TriState_t state) { sram->ProcessUpdateWE(state); }
    void ProcessUpdateOutputEnable(TriState_t state) { sram->ProcessUpdateOE(state); }

    void ProcessUpdateClear(TriState_t state) { shift->ProcessUpdateClr(state); }
    void ProcessUpdateShiftClk(TriState_t state) { shift->ProcessUpdateClk(state); eeprom->ProcessUpdateSck(state); }
    void ProcessUpdateLatchOe(TriState_t state) { latch->ProcessUpdateOE(state); }
    void ProcessUpdateDriverOe(TriState_t state) { driver->ProcessUpdateOE2(state); }
    void ProcessSanityCheck(TriState_t state) { if (state != LOW) sram->ProcessSanityCheck(objectName()); }
    void ProcessCopyEeprom(void) { sram->CopyEeprom(); }



signals:
    void SignalBit0Updated(TriState_t state);
    void SignalBit1Updated(TriState_t state);
    void SignalBit2Updated(TriState_t state);
    void SignalBit3Updated(TriState_t state);
    void SignalBit4Updated(TriState_t state);
    void SignalBit5Updated(TriState_t state);
    void SignalBit6Updated(TriState_t state);
    void SignalBit7Updated(TriState_t state);



public:
    // -- constructor/destructor
    explicit CtrlRomModule_t(const QString &name, const QString &file);
    virtual ~CtrlRomModule_t() {}



public:
    void TriggerFirstUpdate(void);          // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};
