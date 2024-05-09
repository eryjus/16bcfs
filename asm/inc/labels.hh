//===================================================================================================================
// labels.hh -- functions for managing labels in the assembled output.
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of labels.  A label may be already declared (backward
//  reference) or something that is to be forward declared.  Once a label's location is determined, all references
//  to the label are cleaned up.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once


//
// -- This structure is used to keep track of past locations which reference a label and therefore need to be
//    fixed.  All labels are absolute addressing.
//    -------------------------------------------------------------------------------------------------------
class LabelFixup_t {
public:
    LabelFixup_t *next;
    uint64_t where;
};



//
// -- This structure is used to keep track of known labels in the assembled binary file.
//    ----------------------------------------------------------------------------------
class Label_t {
public:
    std::string name;
    bool known;
    LabelFixup_t *fixes;
    uint64_t location;

public:
    static Label_t *factory(std::string &n, uint64_t l) {
        Label_t *rv = new Label_t;
        rv->name = n;
        rv->location = l;
        rv->fixes = nullptr;
        rv->known = false;

        return rv;
    }
};



//
// -- This is the collection of labels
//    --------------------------------
class Labels {
private:
    static Labels *singleton;
    static std::map<std::string, Label_t *> labels;
    static std::string currentLabel;


private:
    Labels(void) {}
    virtual ~Labels() {}


public:
    static Labels &Get(void) { if (!singleton) singleton = new Labels; return *singleton; }
    static void PrintLocationLabels(FILE *fp, uint64_t location);

    static Label_t *Find(std::string n) {
        auto it = labels.find(n[0] == '.' ? currentLabel + n : n);
        return (it == labels.end() ? nullptr : it->second);
    }

    static void FixupLocation(Label_t *lbl);
    static void NoteLabelLocation(void);
    static void NoteLabelReference(void);
    static void DumpLabelLocations(void);
    static void CheckAllLabels(void);
};



