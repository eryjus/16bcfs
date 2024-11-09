//===================================================================================================================
//  sub-demux-4.cc -- This class is a 3-bit demultiplexer sub-module
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Apr-30  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/sub-demux-4.moc.cc"


//
// -- Construct a new 3-Bit Demultiplexer
//    -----------------------------------
SubDemux4_t::SubDemux4_t(void) : QObject()
{
    AllocateComponents();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- Allocate components for this sub-module
//    ---------------------------------------
void SubDemux4_t::AllocateComponents(void)
{
    demuxLo = new IC_74xx238_t;
    demuxHi = new IC_74xx238_t;
}



//
// -- Connect all the inputs/outputs for the sub-module
//    -------------------------------------------------
void SubDemux4_t::WireUp(void)
{
    demuxLo->ProcessUpdateG1(HIGH);
    demuxLo->ProcessUpdateG2B(LOW);
    demuxHi->ProcessUpdateG2A(LOW);
    demuxHi->ProcessUpdateG2B(LOW);

    connect(demuxLo, &IC_74xx238_t::SignalY0Updated, this, &SubDemux4_t::SignalY0Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY1Updated, this, &SubDemux4_t::SignalY1Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY2Updated, this, &SubDemux4_t::SignalY2Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY3Updated, this, &SubDemux4_t::SignalY3Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY4Updated, this, &SubDemux4_t::SignalY4Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY5Updated, this, &SubDemux4_t::SignalY5Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY6Updated, this, &SubDemux4_t::SignalY6Updated);
    connect(demuxLo, &IC_74xx238_t::SignalY7Updated, this, &SubDemux4_t::SignalY7Updated);

    connect(demuxHi, &IC_74xx238_t::SignalY0Updated, this, &SubDemux4_t::SignalY0Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY1Updated, this, &SubDemux4_t::SignalY1Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY2Updated, this, &SubDemux4_t::SignalY2Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY3Updated, this, &SubDemux4_t::SignalY3Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY4Updated, this, &SubDemux4_t::SignalY4Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY5Updated, this, &SubDemux4_t::SignalY5Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY6Updated, this, &SubDemux4_t::SignalY6Updated);
    connect(demuxHi, &IC_74xx238_t::SignalY7Updated, this, &SubDemux4_t::SignalY7Updated);
}



