//===================================================================================================================
//  labels.hh -- Handle all the code to manage labels
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  20-May-2024  Initial           Initial Version
//
//===================================================================================================================



#pragma once


//
// -- This is the encompassing structure for the labels
//    -------------------------------------------------
class Labels_t {
private:
    //
    // -- if a forward declaration, these are unreferenced
    //    ------------------------------------------------
    typedef struct LabelRef_t {
        uint16_t addr;
        struct LabelRef_t *next;
        Listing_t::ListingEntry_t *bin;
    } LabelRef_t;

    //
    // -- This is the complete label definition
    //    -------------------------------------
    typedef struct LabelLocation_t {
        std::string name;
        uint16_t loc;
        LabelRef_t *unresolved;
    } LabelLocation_t;


private:
    static std::string mainLabel;
    static std::map<std::string, LabelLocation_t *> labels;
    static void ResolveReferences(LabelLocation_t *lbl);


public:
    static void AddNewLabel(std::string name, uint16_t loc);
    static uint16_t GetLabelLocation(std::string name, uint16_t loc);
    static void CheckUnresolvedLabels(void);
    static std::string GetMainLabel(void) { return mainLabel; }
};

