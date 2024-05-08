//===================================================================================================================
// labels.cc -- functions for managing labels in the assembled output.
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
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "asm.hh"

#include <cstddef>


//
// -- The singleton instance of the labels structure
//    ----------------------------------------------
Labels *Labels::singleton = nullptr;
std::map<std::string, struct Label_t *> Labels::labels;
std::string Labels::currentLabel;



//
// -- This functions prints all the labels known at a given address location
//    ----------------------------------------------------------------------
void Labels::PrintLocationLabels(FILE *fp, uint64_t location)
{
    for (auto it = labels.begin(); it != labels.end(); it ++) {
        if (it->second->location == location) {
            fprintf(fp, "%s:\n", it->second->name.c_str());
        }
    }
}


//
// -- Fix any known locations for forward declared labels
//    ---------------------------------------------------
void Labels::FixupLocation(Label_t *lbl)
{
    struct LabelFixup_t *fix = lbl->fixes;

    while (fix) {
        Binary_t::SetAddressAt(fix->where, lbl->location / 2);

        fix = fix->next;
    }

    lbl->fixes = 0;
    lbl->known = true;
}


//
// -- For a label, note its location in the global labels table
//    ---------------------------------------------------------
void Labels::NoteLabelLocation(void)
{
    if (yylval.name[strlen(yylval.name) - 1] == ':') yylval.name[strlen(yylval.name) - 1] = 0;

    std::string name = yylval.name;

    if (name[0] != '.') currentLabel = name;
    else name = currentLabel + name;

    auto it = labels.find(name);

    if (it == labels.end()) {
        Label_t *l = Label_t::factory(name, Binary_t::GetLocation());
        l->known = true;
        labels.emplace(name, l);
    } else {
        if (it->second->known) {
            Messaging::Error("Duplicate Label: %s", Parser_t::GetSourceFile(), yylineno, 0, 0, name.c_str());
        } else {
            it->second->location = Binary_t::GetLocation();
            FixupLocation(it->second);
        }
    }
}


//
// -- A label is referenced here.  Note it's location for fixup
//    ---------------------------------------------------------
void Labels::NoteLabelReference(void)
{
    if (yylval.name[strlen(yylval.name) - 1] == ':') yylval.name[strlen(yylval.name) - 1] = 0;

    std::string name = yylval.name;

    if (name[0] != '.') currentLabel = name;
    else name = currentLabel + name;

    auto it = labels.find(name);
    Label_t *lbl = it->second;

    if (it == labels.end() || lbl == nullptr) {
        lbl = Label_t::factory(name, 0);
        labels.emplace(name, lbl);
    }

    if (!lbl->known) {
        LabelFixup_t *fix = new LabelFixup_t();

        fix->next = lbl->fixes;
        fix->where = Binary_t::GetLocation() + Binary_t::GetOrganization() / 8;
        lbl->fixes = fix;
    }
}



//
// -- Dump all known label locations in the global table
//    --------------------------------------------------
void Labels::DumpLabelLocations(void)
{
    for (auto it = labels.begin(); it != labels.end(); it ++) {
        Messaging::Msg("Label `%s` is refers to address %x", it->second->name.c_str(), it->second->location);

        if (it->second->fixes != nullptr) Messaging::Error("Undefined label `%s`", 0, 0, 0, 0, it->second->name.c_str());
    }
}


//
// -- Check that all labels have been defined
//    ---------------------------------------
void Labels::CheckAllLabels(void)
{
    for (auto it = labels.begin(); it != labels.end(); it ++) {
        if (!it->second->known) Messaging::Error("Undefined label `%s`", 0, 0, 0, 0, it->second->name.c_str());
    }
}


