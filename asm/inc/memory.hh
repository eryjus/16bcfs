//===================================================================================================================
// memory.hh -- functions for managing the memory of an assembled binary and architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- The maximum number of memory ranges supported by this assembler
//    ---------------------------------------------------------------
#define MAX_MEM_RANGES 10


//
// -- The max name size of each memory range
//    --------------------------------------
#define MAX_NAME 128



class Memory {
private:
    class Mem_t {
        Mem_t(void) = delete;
        Mem_t(const Mem_t &) = delete;

    public:
        uint64_t from;
        uint64_t to;
        std::string name;

    public:
        Mem_t(uint64_t f, uint64_t t, const std::string n) : from(f), to(t), name(n) {}
        virtual ~Mem_t() {}
    };


private:
    static uint64_t highestMem;
    static uint64_t lowestMem;
    static Mem_t *memRanges[MAX_MEM_RANGES];
    static Memory *singleton;


private:
    Memory(void) {}
    virtual ~Memory() {}


public:
    static Memory *Get(void) { if (!singleton) singleton = new Memory; return singleton; }

    static uint64_t GetStartingMemory(void) { Get(); return lowestMem; }
    static uint64_t GetEndingMemory(void) { Get(); return highestMem; }
    static void AddRange(uint64_t from, uint64_t to, const std::string &name);
    static void Dump(void);
};




