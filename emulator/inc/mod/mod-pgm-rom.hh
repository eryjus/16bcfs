//===================================================================================================================
//  mod-pgm-rom.hh -- This class emulates the program ROM
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This class contains all the components necessary to construct the program ROM.  This is a temporary incarnation,
//  which will be augmented later down the road with a more robust 2-channel memory module.
//
//  Input bus for this module is Address Bus 1.
//  Output bus from this module is the Fetch Bus.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Aug-18  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once


//
// -- The Program ROM Module class.  It will be subclassed from QGroupBox
//    -------------------------------------------------------------------
class PgmRomModule_t : public QGroupBox {
    Q_OBJECT


private:
    QString folder;


    // -- this module contains the following components
    IC_at28c256_t *lsb;
    IC_at28c256_t *msb;
    IC_74xx00_t *nand1;


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
    // -- these functions become the external inputs into this module from the backplane
    void ProcessReset(TriState_t state);


public:
    // -- constructor/destructor
    explicit PgmRomModule_t(const QString &folder);
    virtual ~PgmRomModule_t() {}



public:
    void TriggerFirstUpdate(void);         // trigger all the proper initial updates



private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void BuildGui(void);                    // place the components on the GUI
    void WireUp(void);                      // make all the necessary connections
};


