//===================================================================================================================
//  sub-demux-7.cc -- This class is a 3-bit demultiplexer sub-module
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Apr-30  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/sub-demux-7.moc.cc"


//
// -- Construct a new 3-Bit Demultiplexer
//    -----------------------------------
SubDemux7_t::SubDemux7_t(void) : QObject()
{
    AllocateComponents();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- Allocate components for this sub-module
//    ---------------------------------------
void SubDemux7_t::AllocateComponents(void)
{
    demuxLo = new IC_74xx138_t;
    demuxHi = new IC_74xx138_t;

    demux00 = new IC_74xx238_t;
    demux01 = new IC_74xx238_t;
    demux02 = new IC_74xx238_t;
    demux03 = new IC_74xx238_t;
    demux04 = new IC_74xx238_t;
    demux05 = new IC_74xx238_t;
    demux06 = new IC_74xx238_t;
    demux07 = new IC_74xx238_t;
    demux10 = new IC_74xx238_t;
    demux11 = new IC_74xx238_t;
    demux12 = new IC_74xx238_t;
    demux13 = new IC_74xx238_t;
    demux14 = new IC_74xx238_t;
    demux15 = new IC_74xx238_t;
    demux16 = new IC_74xx238_t;
    demux17 = new IC_74xx238_t;
}



//
// -- Connect all the inputs/outputs for the sub-module
//    -------------------------------------------------
void SubDemux7_t::WireUp(void)
{
    demuxLo->ProcessUpdateG1(HIGH);
    demuxLo->ProcessUpdateG2B(LOW);
    demuxHi->ProcessUpdateG2A(LOW);
    demuxHi->ProcessUpdateG2B(LOW);

    demux00->ProcessUpdateG2B(LOW);
    demux00->ProcessUpdateG1(HIGH);
    demux01->ProcessUpdateG2B(LOW);
    demux01->ProcessUpdateG1(HIGH);
    demux02->ProcessUpdateG2B(LOW);
    demux02->ProcessUpdateG1(HIGH);
    demux03->ProcessUpdateG2B(LOW);
    demux03->ProcessUpdateG1(HIGH);
    demux04->ProcessUpdateG2B(LOW);
    demux04->ProcessUpdateG1(HIGH);
    demux05->ProcessUpdateG2B(LOW);
    demux05->ProcessUpdateG1(HIGH);
    demux06->ProcessUpdateG2B(LOW);
    demux06->ProcessUpdateG1(HIGH);
    demux07->ProcessUpdateG2B(LOW);
    demux07->ProcessUpdateG1(HIGH);

    demux10->ProcessUpdateG2B(LOW);
    demux10->ProcessUpdateG1(HIGH);
    demux11->ProcessUpdateG2B(LOW);
    demux11->ProcessUpdateG1(HIGH);
    demux12->ProcessUpdateG2B(LOW);
    demux12->ProcessUpdateG1(HIGH);
    demux13->ProcessUpdateG2B(LOW);
    demux13->ProcessUpdateG1(HIGH);
    demux14->ProcessUpdateG2B(LOW);
    demux14->ProcessUpdateG1(HIGH);
    demux15->ProcessUpdateG2B(LOW);
    demux15->ProcessUpdateG1(HIGH);
    demux16->ProcessUpdateG2B(LOW);
    demux16->ProcessUpdateG1(HIGH);
    demux17->ProcessUpdateG2B(LOW);
    demux17->ProcessUpdateG1(HIGH);

    connect(demuxLo, &IC_74xx138_t::SignalY0Updated, demux00, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY1Updated, demux01, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY2Updated, demux02, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY3Updated, demux03, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY4Updated, demux04, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY5Updated, demux05, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY6Updated, demux06, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxLo, &IC_74xx138_t::SignalY7Updated, demux07, &IC_74xx238_t::ProcessUpdateG2A);

    connect(demuxHi, &IC_74xx138_t::SignalY0Updated, demux10, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY1Updated, demux11, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY2Updated, demux12, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY3Updated, demux13, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY4Updated, demux14, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY5Updated, demux15, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY6Updated, demux16, &IC_74xx238_t::ProcessUpdateG2A);
    connect(demuxHi, &IC_74xx138_t::SignalY7Updated, demux17, &IC_74xx238_t::ProcessUpdateG2A);

    connect(demux00, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY000Updated);
    connect(demux00, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY001Updated);
    connect(demux00, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY002Updated);
    connect(demux00, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY003Updated);
    connect(demux00, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY004Updated);
    connect(demux00, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY005Updated);
    connect(demux00, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY006Updated);
    connect(demux00, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY007Updated);

    connect(demux01, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY010Updated);
    connect(demux01, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY011Updated);
    connect(demux01, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY012Updated);
    connect(demux01, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY013Updated);
    connect(demux01, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY014Updated);
    connect(demux01, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY015Updated);
    connect(demux01, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY016Updated);
    connect(demux01, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY017Updated);

    connect(demux02, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY020Updated);
    connect(demux02, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY021Updated);
    connect(demux02, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY022Updated);
    connect(demux02, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY023Updated);
    connect(demux02, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY024Updated);
    connect(demux02, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY025Updated);
    connect(demux02, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY026Updated);
    connect(demux02, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY027Updated);

    connect(demux03, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY030Updated);
    connect(demux03, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY031Updated);
    connect(demux03, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY032Updated);
    connect(demux03, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY033Updated);
    connect(demux03, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY034Updated);
    connect(demux03, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY035Updated);
    connect(demux03, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY036Updated);
    connect(demux03, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY037Updated);

    connect(demux04, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY040Updated);
    connect(demux04, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY041Updated);
    connect(demux04, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY042Updated);
    connect(demux04, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY043Updated);
    connect(demux04, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY044Updated);
    connect(demux04, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY045Updated);
    connect(demux04, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY046Updated);
    connect(demux04, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY047Updated);

    connect(demux05, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY050Updated);
    connect(demux05, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY051Updated);
    connect(demux05, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY052Updated);
    connect(demux05, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY053Updated);
    connect(demux05, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY054Updated);
    connect(demux05, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY055Updated);
    connect(demux05, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY056Updated);
    connect(demux05, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY057Updated);

    connect(demux06, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY060Updated);
    connect(demux06, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY061Updated);
    connect(demux06, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY062Updated);
    connect(demux06, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY063Updated);
    connect(demux06, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY064Updated);
    connect(demux06, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY065Updated);
    connect(demux06, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY066Updated);
    connect(demux06, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY067Updated);

    connect(demux07, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY070Updated);
    connect(demux07, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY071Updated);
    connect(demux07, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY072Updated);
    connect(demux07, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY073Updated);
    connect(demux07, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY074Updated);
    connect(demux07, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY075Updated);
    connect(demux07, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY076Updated);
    connect(demux07, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY077Updated);

    connect(demux10, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY000Updated);
    connect(demux10, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY001Updated);
    connect(demux10, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY002Updated);
    connect(demux10, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY003Updated);
    connect(demux10, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY004Updated);
    connect(demux10, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY005Updated);
    connect(demux10, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY006Updated);
    connect(demux10, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY007Updated);

    connect(demux11, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY110Updated);
    connect(demux11, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY111Updated);
    connect(demux11, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY112Updated);
    connect(demux11, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY113Updated);
    connect(demux11, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY114Updated);
    connect(demux11, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY115Updated);
    connect(demux11, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY116Updated);
    connect(demux11, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY117Updated);

    connect(demux12, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY120Updated);
    connect(demux12, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY121Updated);
    connect(demux12, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY122Updated);
    connect(demux12, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY123Updated);
    connect(demux12, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY124Updated);
    connect(demux12, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY125Updated);
    connect(demux12, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY126Updated);
    connect(demux12, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY127Updated);

    connect(demux13, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY130Updated);
    connect(demux13, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY131Updated);
    connect(demux13, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY132Updated);
    connect(demux13, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY133Updated);
    connect(demux13, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY134Updated);
    connect(demux13, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY135Updated);
    connect(demux13, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY136Updated);
    connect(demux13, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY137Updated);

    connect(demux14, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY140Updated);
    connect(demux14, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY141Updated);
    connect(demux14, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY142Updated);
    connect(demux14, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY143Updated);
    connect(demux14, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY144Updated);
    connect(demux14, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY145Updated);
    connect(demux14, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY146Updated);
    connect(demux14, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY147Updated);

    connect(demux15, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY150Updated);
    connect(demux15, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY151Updated);
    connect(demux15, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY152Updated);
    connect(demux15, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY153Updated);
    connect(demux15, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY154Updated);
    connect(demux15, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY155Updated);
    connect(demux15, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY156Updated);
    connect(demux15, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY157Updated);

    connect(demux16, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY160Updated);
    connect(demux16, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY161Updated);
    connect(demux16, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY162Updated);
    connect(demux16, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY163Updated);
    connect(demux16, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY164Updated);
    connect(demux16, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY165Updated);
    connect(demux16, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY166Updated);
    connect(demux16, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY167Updated);

    connect(demux17, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY170Updated);
    connect(demux17, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY171Updated);
    connect(demux17, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY172Updated);
    connect(demux17, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY173Updated);
    connect(demux17, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY174Updated);
    connect(demux17, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY175Updated);
    connect(demux17, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY176Updated);
    connect(demux17, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY177Updated);
}


//
// -- Process Updated to bit A
//    ------------------------
void SubDemux7_t::ProcessUpdateA(TriState_t state)
{
    demux00->ProcessUpdateA(state);
    demux01->ProcessUpdateA(state);
    demux02->ProcessUpdateA(state);
    demux03->ProcessUpdateA(state);
    demux04->ProcessUpdateA(state);
    demux05->ProcessUpdateA(state);
    demux06->ProcessUpdateA(state);
    demux07->ProcessUpdateA(state);
    demux10->ProcessUpdateA(state);
    demux11->ProcessUpdateA(state);
    demux12->ProcessUpdateA(state);
    demux13->ProcessUpdateA(state);
    demux14->ProcessUpdateA(state);
    demux15->ProcessUpdateA(state);
    demux16->ProcessUpdateA(state);
    demux17->ProcessUpdateA(state);
}



//
// -- Process Updated to bit B
//    ------------------------
void SubDemux7_t::ProcessUpdateB(TriState_t state)
{
    demux00->ProcessUpdateB(state);
    demux01->ProcessUpdateB(state);
    demux02->ProcessUpdateB(state);
    demux03->ProcessUpdateB(state);
    demux04->ProcessUpdateB(state);
    demux05->ProcessUpdateB(state);
    demux06->ProcessUpdateB(state);
    demux07->ProcessUpdateB(state);
    demux10->ProcessUpdateB(state);
    demux11->ProcessUpdateB(state);
    demux12->ProcessUpdateB(state);
    demux13->ProcessUpdateB(state);
    demux14->ProcessUpdateB(state);
    demux15->ProcessUpdateB(state);
    demux16->ProcessUpdateB(state);
    demux17->ProcessUpdateB(state);
}



//
// -- Process Updated to bit C
//    ------------------------
void SubDemux7_t::ProcessUpdateC(TriState_t state)
{
    demux00->ProcessUpdateC(state);
    demux01->ProcessUpdateC(state);
    demux02->ProcessUpdateC(state);
    demux03->ProcessUpdateC(state);
    demux04->ProcessUpdateC(state);
    demux05->ProcessUpdateC(state);
    demux06->ProcessUpdateC(state);
    demux07->ProcessUpdateC(state);
    demux10->ProcessUpdateC(state);
    demux11->ProcessUpdateC(state);
    demux12->ProcessUpdateC(state);
    demux13->ProcessUpdateC(state);
    demux14->ProcessUpdateC(state);
    demux15->ProcessUpdateC(state);
    demux16->ProcessUpdateC(state);
    demux17->ProcessUpdateC(state);
}



//
// -- Trigger the initial updates
//    ---------------------------
void SubDemux7_t::TriggerFirstUpdate(void)
{
    demuxLo->TriggerFirstUpdate();
    demuxHi->TriggerFirstUpdate();
    demux00->TriggerFirstUpdate();
    demux01->TriggerFirstUpdate();
    demux02->TriggerFirstUpdate();
    demux03->TriggerFirstUpdate();
    demux04->TriggerFirstUpdate();
    demux05->TriggerFirstUpdate();
    demux06->TriggerFirstUpdate();
    demux07->TriggerFirstUpdate();
    demux10->TriggerFirstUpdate();
    demux11->TriggerFirstUpdate();
    demux12->TriggerFirstUpdate();
    demux13->TriggerFirstUpdate();
    demux14->TriggerFirstUpdate();
    demux15->TriggerFirstUpdate();
    demux16->TriggerFirstUpdate();
    demux17->TriggerFirstUpdate();
}



