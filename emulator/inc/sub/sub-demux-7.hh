//===================================================================================================================
//  sub-demux-7.hh -- This class represents a 4-bit demultiplexer
//
//      Copyright (c) 2023-2025 - Adam Clark
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
class SubDemux7_t : public QObject {
    Q_OBJECT


private:
    // -- this module required the following components
    IC_74xx138_t *demuxLo;
    IC_74xx138_t *demuxHi;
    IC_74xx238_t *demux00;
    IC_74xx238_t *demux01;
    IC_74xx238_t *demux02;
    IC_74xx238_t *demux03;
    IC_74xx238_t *demux04;
    IC_74xx238_t *demux05;
    IC_74xx238_t *demux06;
    IC_74xx238_t *demux07;
    IC_74xx238_t *demux10;
    IC_74xx238_t *demux11;
    IC_74xx238_t *demux12;
    IC_74xx238_t *demux13;
    IC_74xx238_t *demux14;
    IC_74xx238_t *demux15;
    IC_74xx238_t *demux16;
    IC_74xx238_t *demux17;


public slots:
    void ProcessUpdateA(TriState_t state);
    void ProcessUpdateB(TriState_t state);
    void ProcessUpdateC(TriState_t state);
    void ProcessUpdateD(TriState_t state) { demuxLo->ProcessUpdateA(state); demuxHi->ProcessUpdateA(state); }
    void ProcessUpdateE(TriState_t state) { demuxLo->ProcessUpdateB(state); demuxHi->ProcessUpdateB(state); }
    void ProcessUpdateF(TriState_t state) { demuxLo->ProcessUpdateC(state); demuxHi->ProcessUpdateC(state); }
    void ProcessUpdateG(TriState_t state) { demuxLo->ProcessUpdateG2A(state); demuxHi->ProcessUpdateG1(state); }


// -- the numbers in the following signals are in octal form
signals:
    // driven by demuxLo; bit 0
    void SignalY000Updated(TriState_t state);
    void SignalY001Updated(TriState_t state);
    void SignalY002Updated(TriState_t state);
    void SignalY003Updated(TriState_t state);
    void SignalY004Updated(TriState_t state);
    void SignalY005Updated(TriState_t state);
    void SignalY006Updated(TriState_t state);
    void SignalY007Updated(TriState_t state);

    // driven by demuxLo; bit 1
    void SignalY010Updated(TriState_t state);
    void SignalY011Updated(TriState_t state);
    void SignalY012Updated(TriState_t state);
    void SignalY013Updated(TriState_t state);
    void SignalY014Updated(TriState_t state);
    void SignalY015Updated(TriState_t state);
    void SignalY016Updated(TriState_t state);
    void SignalY017Updated(TriState_t state);

    // driven by demuxLo; bit 2
    void SignalY020Updated(TriState_t state);
    void SignalY021Updated(TriState_t state);
    void SignalY022Updated(TriState_t state);
    void SignalY023Updated(TriState_t state);
    void SignalY024Updated(TriState_t state);
    void SignalY025Updated(TriState_t state);
    void SignalY026Updated(TriState_t state);
    void SignalY027Updated(TriState_t state);

    // driven by demuxLo; bit 3
    void SignalY030Updated(TriState_t state);
    void SignalY031Updated(TriState_t state);
    void SignalY032Updated(TriState_t state);
    void SignalY033Updated(TriState_t state);
    void SignalY034Updated(TriState_t state);
    void SignalY035Updated(TriState_t state);
    void SignalY036Updated(TriState_t state);
    void SignalY037Updated(TriState_t state);

    // driven by demuxLo; bit 4
    void SignalY040Updated(TriState_t state);
    void SignalY041Updated(TriState_t state);
    void SignalY042Updated(TriState_t state);
    void SignalY043Updated(TriState_t state);
    void SignalY044Updated(TriState_t state);
    void SignalY045Updated(TriState_t state);
    void SignalY046Updated(TriState_t state);
    void SignalY047Updated(TriState_t state);

    // driven by demuxLo; bit 5
    void SignalY050Updated(TriState_t state);
    void SignalY051Updated(TriState_t state);
    void SignalY052Updated(TriState_t state);
    void SignalY053Updated(TriState_t state);
    void SignalY054Updated(TriState_t state);
    void SignalY055Updated(TriState_t state);
    void SignalY056Updated(TriState_t state);
    void SignalY057Updated(TriState_t state);

    // driven by demuxLo; bit 6
    void SignalY060Updated(TriState_t state);
    void SignalY061Updated(TriState_t state);
    void SignalY062Updated(TriState_t state);
    void SignalY063Updated(TriState_t state);
    void SignalY064Updated(TriState_t state);
    void SignalY065Updated(TriState_t state);
    void SignalY066Updated(TriState_t state);
    void SignalY067Updated(TriState_t state);

    // demuxLo; bit 7
    void SignalY070Updated(TriState_t state);
    void SignalY071Updated(TriState_t state);
    void SignalY072Updated(TriState_t state);
    void SignalY073Updated(TriState_t state);
    void SignalY074Updated(TriState_t state);
    void SignalY075Updated(TriState_t state);
    void SignalY076Updated(TriState_t state);
    void SignalY077Updated(TriState_t state);

    // demuxHi; bit 0
    void SignalY100Updated(TriState_t state);
    void SignalY101Updated(TriState_t state);
    void SignalY102Updated(TriState_t state);
    void SignalY103Updated(TriState_t state);
    void SignalY104Updated(TriState_t state);
    void SignalY105Updated(TriState_t state);
    void SignalY106Updated(TriState_t state);
    void SignalY107Updated(TriState_t state);

    // driven by demuxHi; bit 1
    void SignalY110Updated(TriState_t state);
    void SignalY111Updated(TriState_t state);
    void SignalY112Updated(TriState_t state);
    void SignalY113Updated(TriState_t state);
    void SignalY114Updated(TriState_t state);
    void SignalY115Updated(TriState_t state);
    void SignalY116Updated(TriState_t state);
    void SignalY117Updated(TriState_t state);

    // driven by demuxHi; bit 2
    void SignalY120Updated(TriState_t state);
    void SignalY121Updated(TriState_t state);
    void SignalY122Updated(TriState_t state);
    void SignalY123Updated(TriState_t state);
    void SignalY124Updated(TriState_t state);
    void SignalY125Updated(TriState_t state);
    void SignalY126Updated(TriState_t state);
    void SignalY127Updated(TriState_t state);

    // driven by demuxHi; bit 3
    void SignalY130Updated(TriState_t state);
    void SignalY131Updated(TriState_t state);
    void SignalY132Updated(TriState_t state);
    void SignalY133Updated(TriState_t state);
    void SignalY134Updated(TriState_t state);
    void SignalY135Updated(TriState_t state);
    void SignalY136Updated(TriState_t state);
    void SignalY137Updated(TriState_t state);

    // driven by demuxHi; bit 4
    void SignalY140Updated(TriState_t state);
    void SignalY141Updated(TriState_t state);
    void SignalY142Updated(TriState_t state);
    void SignalY143Updated(TriState_t state);
    void SignalY144Updated(TriState_t state);
    void SignalY145Updated(TriState_t state);
    void SignalY146Updated(TriState_t state);
    void SignalY147Updated(TriState_t state);

    // driven by demuxHi; bit 5
    void SignalY150Updated(TriState_t state);
    void SignalY151Updated(TriState_t state);
    void SignalY152Updated(TriState_t state);
    void SignalY153Updated(TriState_t state);
    void SignalY154Updated(TriState_t state);
    void SignalY155Updated(TriState_t state);
    void SignalY156Updated(TriState_t state);
    void SignalY157Updated(TriState_t state);

    // driven by demuxHi; bit 6
    void SignalY160Updated(TriState_t state);
    void SignalY161Updated(TriState_t state);
    void SignalY162Updated(TriState_t state);
    void SignalY163Updated(TriState_t state);
    void SignalY164Updated(TriState_t state);
    void SignalY165Updated(TriState_t state);
    void SignalY166Updated(TriState_t state);
    void SignalY167Updated(TriState_t state);

    // driven by demuxHi; bit 7
    void SignalY170Updated(TriState_t state);
    void SignalY171Updated(TriState_t state);
    void SignalY172Updated(TriState_t state);
    void SignalY173Updated(TriState_t state);
    void SignalY174Updated(TriState_t state);
    void SignalY175Updated(TriState_t state);
    void SignalY176Updated(TriState_t state);
    void SignalY177Updated(TriState_t state);


public:
    SubDemux7_t(void);
    virtual ~SubDemux7_t() {}


public:
    void TriggerFirstUpdate(void);


private:
    // -- intenral functions
    void AllocateComponents(void);          // Get the component memory from heap
    void WireUp(void);                      // make all the necessary connections
};




