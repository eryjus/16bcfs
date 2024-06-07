//===================================================================================================================
//  labels.cc -- The implentation of the labels
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  20-May-2024  Initial           Initial Version
//
//===================================================================================================================



#include "asm.hh"



//
// -- the static members of the Labels_t class
//    ----------------------------------------
std::string Labels_t::mainLabel = "";
std::map<std::string, Labels_t::LabelLocation_t *> Labels_t::labels;



//
// -- Add a new label into the label map
//    ----------------------------------
void Labels_t::AddNewLabel(std::string _lbl, uint16_t loc)
{
    if (_lbl[_lbl.length() - 1] == ':') _lbl = _lbl.substr(0, _lbl.length() - 1);

    std::string lbl;
    lbl.resize(_lbl.size());

    std::transform(_lbl.begin(), _lbl.end(), lbl.begin(), ::tolower);

    // -- if an intermediate label, add the main label part
    if (lbl[0] == '.') {
        lbl = mainLabel + lbl;
        _lbl = mainLabel + _lbl;
    } else {
        mainLabel = lbl;
    }

    if (lbl[0] == '.') {
        std::cerr << "Error: Label " << lbl
                << " is an intermediate label without a main label to build on; line " << yylineno << std::endl;
        Parser_t::IncErrors();
        return;
    }

    auto it = labels.find(lbl);
    if (it == labels.end()) {
        LabelLocation_t *lblLoc = new LabelLocation_t;
        lblLoc->name = lbl;
        lblLoc->loc = loc;
        lblLoc->unresolved = nullptr;

        labels[lbl] = lblLoc;
    } else {
        it->second->loc = loc;
        ResolveReferences(it->second);
    }

    Listing_t::AddLabel(_lbl);
}



//
// -- Get the location of a label
//    ---------------------------
uint16_t Labels_t::GetLabelLocation(std::string _lbl, uint16_t loc)
{
    std::string lbl;
    lbl.resize(_lbl.size());

    std::transform(_lbl.begin(), _lbl.end(), lbl.begin(), ::tolower);

    // -- if an intermediate label, add the main label part
    if (lbl[0] == '.') {
        lbl = mainLabel + lbl;
    } else {
        mainLabel = lbl;
    }

    if (lbl[0] == '.') {
        std::cerr << "Error: Label " << lbl
                << " is an intermediate label without a main label to build on" << std::endl;
        Parser_t::IncErrors();
        return 0xffff;
    }

    auto it = labels.find(lbl);
    if (it == labels.end()) {
        // -- here we need to construct an undefined reference
        LabelRef_t *ref = new LabelRef_t;
        ref->addr = loc;
        ref->next = nullptr;
        ref->bin = Listing_t::GetCurrent();

        LabelLocation_t *lblLoc = new LabelLocation_t;
        lblLoc->name = lbl;
        lblLoc->loc = 0xffff;
        lblLoc->unresolved = ref;

        labels[lbl] = lblLoc;

        return 0xffff;
    } else {
        if (it->second->unresolved == nullptr) {
            return it->second->loc;
        } else {
            LabelRef_t *ref = new LabelRef_t;
            ref->addr = loc;
            ref->next = it->second->unresolved;
            ref->bin = Listing_t::GetCurrent();

            it->second->unresolved = ref;

            return 0xffff;
        }
    }
}



//
// -- Resolve references to a label which has been forward declared
//    -------------------------------------------------------------
void Labels_t::ResolveReferences(LabelLocation_t *lbl)
{
    LabelRef_t *ref = lbl->unresolved;

    while (ref != nullptr) {
        Binary_t::UpdateBinaryLocation(ref->addr, lbl->loc);

        if (ref->bin == nullptr || ref->bin->next == nullptr) {
            std::cerr << "Unable to resolve references in the listing for label " << lbl->name << std::endl;
        }

        ref->bin->next->binary = Listing_t::MakeHex(lbl->loc);
        LabelRef_t *tmp = ref;

        ref = ref->next;

        delete ref;
    }

    lbl->unresolved = nullptr;
}



//
// -- Check for unresolved references and report each one
//    ---------------------------------------------------
void Labels_t::CheckUnresolvedLabels(void)
{
    for (auto it = labels.begin(); it != labels.end(); it ++) {
        if (it->second->unresolved != nullptr) {
            std::cerr << "Error: Unresolved location " << it->second->name << std::endl;
            Parser_t::IncErrors();
        }
    }
}


