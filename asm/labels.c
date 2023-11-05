//===================================================================================================================
// labels.c -- functions for managing labels in the assembled output.
//
//  These functions and variables are used to take care of labels.  A label may be already declared (backward
//  reference) or something that is to be forward declared.  Once a label's location is determined, all references
//  to the label are cleaned up.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"
#include <stdint.h>


//
// -- This structure is used to keep track of past locations which reference a label and therefore need to be
//    fixed.  All labels are absolute addressing.
//    -------------------------------------------------------------------------------------------------------
struct LabelFixup_t {
    struct LabelFixup_t *next;
    uint64_t where;
};


//
// -- This structure is used to keep track of known labels in the assembled binary file.
//    ----------------------------------------------------------------------------------
struct Label_t {
    char *name;
    uint64_t location;
    struct LabelFixup_t *fixes;
};


//
// -- This is all the known labels in the assembled binary.
//    -----------------------------------------------------
struct Label_t labels[MAX_LABELS] = { { 0 } };


//
// -- This functions prints all the labels known at a given address location
//    ----------------------------------------------------------------------
void PrintLocationLabels(FILE *fp, uint64_t location)
{
    for (int i = 0; i < MAX_LABELS; i ++) {
        if (labels[i].location == location && labels[i].name != 0) {
            fprintf(fp, "%s:\n", labels[i].name);
        }
    }
}


//
// -- Fix any known locations for forward declared labels
//    ---------------------------------------------------
void FixupLocation(struct Label_t *lbl)
{
    struct LabelFixup_t *fix = lbl->fixes;

    while (fix) {
        SetAddressAt(fix->where, lbl->location);

        fix = fix->next;
    }

    lbl->fixes = 0;
}


//
// -- For a label, note its location in the global labels table
//    ---------------------------------------------------------
void NoteLabelLocation(void)
{
    if (yylval.name[strlen(yylval.name) - 1] == ':') yylval.name[strlen(yylval.name) - 1] = 0;

    for (int i = 0; i < MAX_LABELS; i ++) {
        if (!labels[i].name) {
            labels[i].name = yylval.name;
            labels[i].location = GetLocation();
            labels[i].fixes = 0;

            return;
        }

        if (strcmp(labels[i].name, yylval.name) == 0) {
            if (labels[i].fixes) {
                FixupLocation(&labels[i]);
                return;
            }

            Error("Duplicate Label: %s", sourceFile, yylineno, 0, 0, yylval.name);
            return;
        }
    }
}


//
// -- Dump all known label locations in the global table
//    --------------------------------------------------
void DumpLabelLocations(void)
{
    for (int i = 0; i < MAX_LABELS; i ++) {
        if (!labels[i].name) {
            return;
        }

        Msg("Label `%s` is refers to address %x", labels[i].name, labels[i].location);

        if (labels[i].fixes) Error("Undefined labels %s", 0, 0, 0, 0, labels[i].name);
    }
}


