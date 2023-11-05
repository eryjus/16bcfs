//===================================================================================================================
// asm.c -- support routines/globals for the assembler.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"


//
// -- The list of folders in which to search for architecture and include files
//    -------------------------------------------------------------------------
char *searchList[MAX_SEARCH_PATH] = {0};


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




