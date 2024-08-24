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

    connect(demuxLo, &IC_74xx138_t::SignalY0Updated, demux00, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY1Updated, demux01, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY2Updated, demux02, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY3Updated, demux03, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY4Updated, demux04, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY5Updated, demux05, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY6Updated, demux06, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxLo, &IC_74xx138_t::SignalY7Updated, demux07, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);

    connect(demuxHi, &IC_74xx138_t::SignalY0Updated, demux10, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY1Updated, demux11, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY2Updated, demux12, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY3Updated, demux13, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY4Updated, demux14, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY5Updated, demux15, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY6Updated, demux16, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);
    connect(demuxHi, &IC_74xx138_t::SignalY7Updated, demux17, &IC_74xx238_t::ProcessUpdateG2A, CNN_TYPE);

    connect(demux00, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY000Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY001Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY002Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY003Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY004Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY005Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY006Updated, CNN_TYPE);
    connect(demux00, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY007Updated, CNN_TYPE);

    connect(demux01, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY010Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY011Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY012Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY013Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY014Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY015Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY016Updated, CNN_TYPE);
    connect(demux01, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY017Updated, CNN_TYPE);

    connect(demux02, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY020Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY021Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY022Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY023Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY024Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY025Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY026Updated, CNN_TYPE);
    connect(demux02, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY027Updated, CNN_TYPE);

    connect(demux03, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY030Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY031Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY032Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY033Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY034Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY035Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY036Updated, CNN_TYPE);
    connect(demux03, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY037Updated, CNN_TYPE);

    connect(demux04, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY040Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY041Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY042Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY043Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY044Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY045Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY046Updated, CNN_TYPE);
    connect(demux04, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY047Updated, CNN_TYPE);

    connect(demux05, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY050Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY051Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY052Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY053Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY054Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY055Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY056Updated, CNN_TYPE);
    connect(demux05, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY057Updated, CNN_TYPE);

    connect(demux06, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY060Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY061Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY062Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY063Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY064Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY065Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY066Updated, CNN_TYPE);
    connect(demux06, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY067Updated, CNN_TYPE);

    connect(demux07, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY070Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY071Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY072Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY073Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY074Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY075Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY076Updated, CNN_TYPE);
    connect(demux07, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY077Updated, CNN_TYPE);

    connect(demux10, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY000Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY001Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY002Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY003Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY004Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY005Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY006Updated, CNN_TYPE);
    connect(demux10, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY007Updated, CNN_TYPE);

    connect(demux11, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY110Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY111Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY112Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY113Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY114Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY115Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY116Updated, CNN_TYPE);
    connect(demux11, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY117Updated, CNN_TYPE);

    connect(demux12, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY120Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY121Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY122Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY123Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY124Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY125Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY126Updated, CNN_TYPE);
    connect(demux12, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY127Updated, CNN_TYPE);

    connect(demux13, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY130Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY131Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY132Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY133Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY134Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY135Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY136Updated, CNN_TYPE);
    connect(demux13, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY137Updated, CNN_TYPE);

    connect(demux14, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY140Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY141Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY142Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY143Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY144Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY145Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY146Updated, CNN_TYPE);
    connect(demux14, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY147Updated, CNN_TYPE);

    connect(demux15, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY150Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY151Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY152Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY153Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY154Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY155Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY156Updated, CNN_TYPE);
    connect(demux15, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY157Updated, CNN_TYPE);

    connect(demux16, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY160Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY161Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY162Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY163Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY164Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY165Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY166Updated, CNN_TYPE);
    connect(demux16, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY167Updated, CNN_TYPE);

    connect(demux17, &IC_74xx238_t::SignalY0Updated, this, &SubDemux7_t::SignalY170Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY1Updated, this, &SubDemux7_t::SignalY171Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY2Updated, this, &SubDemux7_t::SignalY172Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY3Updated, this, &SubDemux7_t::SignalY173Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY4Updated, this, &SubDemux7_t::SignalY174Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY5Updated, this, &SubDemux7_t::SignalY175Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY6Updated, this, &SubDemux7_t::SignalY176Updated, CNN_TYPE);
    connect(demux17, &IC_74xx238_t::SignalY7Updated, this, &SubDemux7_t::SignalY177Updated, CNN_TYPE);
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



