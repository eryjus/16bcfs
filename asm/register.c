//===================================================================================================================
// register.c -- functions for managing the registers of an architecture
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"


//
// -- A structure representing a register definition
//    ----------------------------------------------
struct reg_t {
    char *regName;
    int bits;
};


//
// -- The list of registers known
//    ---------------------------
struct reg_t registers[MAX_REGISTERS] = { { 0 } };


//
// -- Add a register to the list.  Checks for duplicates
//    --------------------------------------------------
struct reg_t *AddRegister(char *r, int b)
{
    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i].regName == 0) {
            registers[i].regName = r;
            registers[i].bits = b;
            return &registers[i];
        }

        if (strcmp(r, registers[i].regName) == 0) {
            Error("Register %s already defined as %d bits wide", sourceFile, yylineno, 0, 0, r, b);
            return &registers[i];
        }
    }

    Error("Too many registers", sourceFile, yylineno, 0, 0);
    return 0;
}


//
// -- Dump the list of registers
//    --------------------------
void DumpRegisters(void)
{
    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i].regName == 0) return;

        Msg("Register %s is %d bytes wide\n", registers[i].regName, registers[i].bits);
    }
}

