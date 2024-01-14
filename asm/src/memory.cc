//===================================================================================================================
// memory.cc -- functions for managing the memory of an assembled binary and architecture
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
// -- Static members
//    --------------
uint64_t Memory::highestMem = 0;
uint64_t Memory::lowestMem = ~0;
Memory::Mem_t *Memory::memRanges[MAX_MEM_RANGES] = { nullptr };
Memory *Memory::singleton = nullptr;



//
// -- Adds a memory range to the table, checking for overlaps
//    -------------------------------------------------------
void Memory::AddRange(uint64_t from, uint64_t to, const std::string &name)
{
    int empty = -1;
    Get();

    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i] == nullptr) {
            if (empty == -1) empty = i;

            continue;
        }

        if (from >= memRanges[i]->from && from <= memRanges[i]->to) {
            Messaging::Error("Overlapping memory ranges defined for %lx to %lx\n", Parser_t::GetSourceFile(), yylineno, 0, 0, from, to);

            return;
        }

        if (to <= memRanges[i]->to && to >= memRanges[i]->from) {
            Messaging::Error("Overlapping memory ranges defined for %lx to %lx\n", Parser_t::GetSourceFile(), yylineno, 0, 0, from, to);

            return;
        }
    }

    if (empty == -1) {
        Messaging::Error("Too many memory ranges defined\n", Parser_t::GetSourceFile(), yylineno, 0, 0);

        return;
    }

    memRanges[empty] = new Mem_t(from, to, name);

    if (from < lowestMem) lowestMem = from;
    if (to > highestMem) highestMem = to;

    return;
}


//
// -- Dumps all know memory ranges
//    ----------------------------
void Memory::Dump(void)
{
    Get();

    Messaging::Msg("Overall Memory from %lx to %lx", lowestMem, highestMem);

    for (int i = 0; i < MAX_MEM_RANGES; i ++) {
        if (memRanges[i] == nullptr) continue;

        Messaging::Msg("Memory Range from %x to %x\n", memRanges[i]->from, memRanges[i]->to);
    }
}

