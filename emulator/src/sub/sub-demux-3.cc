//===================================================================================================================
//  sub-demux-3.cc -- This class is a 3-bit demultiplexer sub-module
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Apr-30  Initial  v0.0.1   Initial version
//===================================================================================================================


#include "16bcfs.hh"
#include "../moc/sub-demux-3.moc.cc"


//
// -- Construct a new 3-Bit Demultiplexer
//    -----------------------------------
SubDemux3_t::SubDemux3_t(void) : QObject()
{
    AllocateComponents();
    WireUp();
    TriggerFirstUpdate();
}



//
// -- Allocate components for this sub-module
//    ---------------------------------------
void SubDemux3_t::AllocateComponents(void)
{
    demux = new IC_74xx238_t;
}



//
// -- Connect all the inputs/outputs for the sub-module
//    -------------------------------------------------
void SubDemux3_t::WireUp(void)
{
    demux->ProcessUpdateG1(HIGH);
    demux->ProcessUpdateG2A(LOW);
    demux->ProcessUpdateG2B(LOW);

    connect(demux, &IC_74xx238_t::SignalY0Updated, this, &SubDemux3_t::SignalY0Updated);
    connect(demux, &IC_74xx238_t::SignalY1Updated, this, &SubDemux3_t::SignalY1Updated);
    connect(demux, &IC_74xx238_t::SignalY2Updated, this, &SubDemux3_t::SignalY2Updated);
    connect(demux, &IC_74xx238_t::SignalY3Updated, this, &SubDemux3_t::SignalY3Updated);
    connect(demux, &IC_74xx238_t::SignalY4Updated, this, &SubDemux3_t::SignalY4Updated);
    connect(demux, &IC_74xx238_t::SignalY5Updated, this, &SubDemux3_t::SignalY5Updated);
    connect(demux, &IC_74xx238_t::SignalY6Updated, this, &SubDemux3_t::SignalY6Updated);
    connect(demux, &IC_74xx238_t::SignalY7Updated, this, &SubDemux3_t::SignalY7Updated);
}



