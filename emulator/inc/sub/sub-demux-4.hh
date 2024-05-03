//===================================================================================================================
//  sub-demux-4.hh -- This class represents a 4-bit demultiplexer
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-May-01  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- The 4-bit demultiplexer class
//    -----------------------------
class SubDemux4_t : public QObject {
    Q_OBJECT


private:
    // -- this module required the following components
    IC_74xx238_t *demuxLo;
    IC_74xx238_t *demuxHi;


public slots:
    void ProcessUpdateA(TriState_t state) { demuxLo->ProcessUpdateA(state); demuxHi->ProcessUpdateA(state); }
    void ProcessUpdateB(TriState_t state) { demuxLo->ProcessUpdateB(state); demuxHi->ProcessUpdateB(state); }
    void ProcessUpdateC(TriState_t state) { demuxLo->ProcessUpdateC(state); demuxHi->ProcessUpdateC(state); }
    void ProcessUpdateD(TriState_t state) { demuxLo->ProcessUpdateG2A(state); demuxHi->ProcessUpdateG1(state); }


signals:
    void SignalY0Updated(TriState_t state);
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
    void SignalY5Updated(TriState_t state);
    void SignalY6Updated(TriState_t state);
    void SignalY7Updated(TriState_t state);
    void SignalY8Updated(TriState_t state);
    void SignalY9Updated(TriState_t state);
    void SignalYAUpdated(TriState_t state);
    void SignalYBUpdated(TriState_t state);
    void SignalYCUpdated(TriState_t state);
    void SignalYDUpdated(TriState_t state);
    void SignalYEUpdated(TriState_t state);
    void SignalYFUpdated(TriState_t state);


public:
    SubDemux4_t(void);
    virtual ~SubDemux4_t() {}


public:
    void TriggerFirstUpdate(void) { demuxLo->TriggerFirstUpdate(); demuxHi->TriggerFirstUpdate(); }


private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void WireUp(void);                      // make all the necessary connections
};




