//===================================================================================================================
// asm.c -- support routines/globals for the assembler.
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.hh"


//
// -- The list of folders in which to search for architecture and include files
//    -------------------------------------------------------------------------
char *searchList[MAX_SEARCH_PATH] = {0};


void OpenFile (const char *file);




//
// -- The current token from the scanner
//    ----------------------------------
yylval_t yylval;



//
// -- Search the searchList for the filename and open the file if it exists; return an open FILE if found
//    ---------------------------------------------------------------------------------------------------
FILE *searchPath(char *filename)
{
    FILE *rv = fopen(filename, "r");

    if (rv) return rv;

    for (int i = 0; i < MAX_SEARCH_PATH; i ++) {
        char fullName[1024];

        if (!searchList[i]) return 0;

        sprintf(fullName, "%s/%s", searchList[i], filename);

        rv = fopen(fullName, "r");

        if (rv) return rv;
    }

    return 0;
}


//
// -- main entry point
//    ----------------
int main(int argc, char *argv[])
{
    char dftSearch[256];

    sprintf(dftSearch, "%s/%s/", getenv("HOME"), "bin/arch");
    searchList[0] = dftSearch;

    for (int i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-I") == 0) {
            i ++;
            for (int j = 0; j < MAX_SEARCH_PATH; j ++) {
                if (searchList[j] == 0) {
                    searchList[j] = argv[i];
                    break;
                }
            }

            fprintf(stderr, "Too many search paths\n");
            exit(EXIT_FAILURE);
        } else if (strncmp(argv[i], "-l=-", 4) == 0) {
            Parser_t::SetListingFile(stdout);
        } else if (strncmp(argv[i], "-l=", 3) == 0) {
            Parser_t::SetListingFile(fopen(argv[i] + 3, "w"));
            if (!Parser_t::GetListingFile()) {
                Messaging::Fatal("Unable to open listing output file", __FILE__, __LINE__, "", 0);
            }
        } else if (strncmp(argv[i], "-l", 2) == 0) {
            Parser_t::SetListingFile(fopen("output.txt", "w"));
            if (!Parser_t::GetListingFile()) {
                Messaging::Fatal("Unable to open listing output file", __FILE__, __LINE__, "", 0);
            }
        } else {
            Parser_t::SetSourceFile(argv[i]);
        }
    }

    OpenFile(Parser_t::GetSourceFile());
    Parser_t::Get()->ParseFile();
//    Labels::DumpLabelLocations();
    Binary_t::OutputListing();
    Binary_t::OutputResults();

    if (Parser_t::GetListingFile()) {
        fflush(Parser_t::GetListingFile());
        if (Parser_t::GetListingFile() != stdout) fclose(Parser_t::GetListingFile());
    }

    Binary_t::SplitOutput();
    OpCodes::OutputEnum();

    return EXIT_SUCCESS;
}



