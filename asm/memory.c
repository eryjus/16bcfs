//===================================================================================================================
// memory.c -- functions for managing the memory of an assembled binary and architecture
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"
#include <stdint.h>
#include <string.h>
#include <sys/types.h>


//
// -- The maximum number of memory ranges supported by this assembler
//    ---------------------------------------------------------------
#define MAX_MEM_RANGES 10


//
// -- The max name size of each memory range
//    --------------------------------------
#define MAX_NAME 128


//
// -- Each memory range is defined with the following structure
//    ---------------------------------------------------------
struct Mem_t {
    uint64_t from;
    uint64_t to;
    char name[MAX_NAME];
};


//
// -- These are the  memory limits for the architecture.  Note the trick with `~0` to set all the bits
//    ------------------------------------------------------------------------------------------------
uint64_t highestMem = 0;
uint64_t lowestMem = ~0;


//
// -- here are all the memory ranges for the architecture.  they may not overlap
//    --------------------------------------------------------------------------
struct Mem_t memRanges[MAX_MEM_RANGES] = { { 0 } };


//
// -- get the starting memory address
//    -------------------------------
uint64_t GetStartingMemory(void)
{
    return lowestMem;
}


//
// -- get the ending memory address
//    -----------------------------
uint64_t GetEndingMemory(void)
{
    return highestMem;
}


//
// -- Adds a memory range to the table, checking for overlaps
//    -------------------------------------------------------
void AddMemoryRange(uint64_t from, uint64_t to, const char *name)
{
    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i].from == 0 && memRanges[i].to == 0) {
            memRanges[i].from = from;
            memRanges[i].to = to;

            if (from < lowestMem) lowestMem = from;
            if (to > highestMem) highestMem = to;

            strncpy(memRanges[i].name, name, MAX_NAME - 1);

            return;
        }

        if (from >= memRanges[i].from && from <= memRanges[i].to) {
            Error("Overlapping memory ranges defined for %lx to %lx\n", sourceFile, yylineno, 0, 0, from, to);
            return;
        }

        if (to <= memRanges[i].to && to >= memRanges[i].from) {
            Error("Overlapping memory ranges defined for %lx to %lx\n", sourceFile, yylineno, 0, 0, from, to);
            return;
        }
    }

    Error("Too many memory ranges defined\n", sourceFile, yylineno, 0, 0);
}


//
// -- Dumps all know memory ranges
//    ----------------------------
void DumpMemory(void)
{
    Msg("Overall Memory from %lx to %lx", lowestMem, highestMem);

    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i].from == 0 && memRanges[i].to == 0) {
            return;
        }

        Msg("Memory Range from %x to %x\n", memRanges[i].from, memRanges[i].to);
    }
}

