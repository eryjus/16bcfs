//===================================================================================================================
//  sub-demux-3.hh -- This class represents a 3-bit demultiplexer
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  A 2-bit demultiplexer will use this 3-bit one and tie the most significant select digit to ground.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Apr-30  Initial  v0.0.1   Initial version
//===================================================================================================================


#pragma once



//
// -- The 3-bit demultiplexer class
//    -----------------------------
class SubDemux3_t : public QObject {
    Q_OBJECT


private:
    // -- this module required the following components
    IC_74xx238_t *demux;


public slots:
    void ProcessUpdateA(TriState_t state) { demux->ProcessUpdateA(state); }
    void ProcessUpdateB(TriState_t state) { demux->ProcessUpdateB(state); }
    void ProcessUpdateC(TriState_t state) { demux->ProcessUpdateC(state); }


signals:
    void SignalY0Updated(TriState_t state);
    void SignalY1Updated(TriState_t state);
    void SignalY2Updated(TriState_t state);
    void SignalY3Updated(TriState_t state);
    void SignalY4Updated(TriState_t state);
    void SignalY5Updated(TriState_t state);
    void SignalY6Updated(TriState_t state);
    void SignalY7Updated(TriState_t state);


public:
    SubDemux3_t(void);
    virtual ~SubDemux3_t() {}


public:
    void TriggerFirstUpdate(void) { demux->TriggerFirstUpdate(); }


private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void WireUp(void);                      // make all the necessary connections
};




