//===================================================================================================================
//  hw-computer.cc -- This is the foundation of the computer build -- like a backplane or breadboard
//
//      Copyright (c) 2023 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Oct-24  Initial  v0.0.1   Reinvisioned initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "hw-computer.moc.cc"


//
// -- Static class members
//    --------------------
HW_Computer_t *HW_Computer_t::singleton = nullptr;
HW_Oscillator_t *HW_Computer_t::oscillator = nullptr;
HW_Clock_t *HW_Computer_t::clock = nullptr;
HW_Bus_t *HW_Computer_t::aluA = nullptr;
HW_Bus_t *HW_Computer_t::aluB = nullptr;
HW_Alu_t *HW_Computer_t::alu = nullptr;
HW_Bus_t *HW_Computer_t::mainBus = nullptr;
HW_BusDriver_t *HW_Computer_t::aluADriver = nullptr;
HW_BusDriver_t *HW_Computer_t::aluBDriver = nullptr;




//
// -- Initialize the computer by placing the components and hooking up the wires (signals/slots)
//    ------------------------------------------------------------------------------------------
void HW_Computer_t::Initialize(void)
{
    //
    // -- Place the oscillator
    //    --------------------
    singleton->oscillator = new HW_Oscillator_t;
    singleton->clock = new HW_Clock_t(oscillator);


    //
    // -- Create ALUA and ALUB
    //    --------------------
    singleton->aluA = new HW_Bus_t(singleton->oscillator);
    singleton->aluB = new HW_Bus_t(singleton->oscillator);
    singleton->aluADriver = new HW_BusDriver_t(singleton->aluA);
    singleton->aluBDriver = new HW_BusDriver_t(singleton->aluB);


    //
    // -- Create the Main Bus
    //    -------------------
    singleton->mainBus = new HW_Bus_t(singleton->oscillator);


    //
    // -- Create the ALU
    //    --------------
    singleton->alu = new HW_Alu_t(singleton->aluA, singleton->aluB, singleton->mainBus);


    connect(GetOscillator(), &HW_Oscillator_t::SignalStateChanged, singleton, &HW_Computer_t::SignalOscillatorStateChanged);
}


