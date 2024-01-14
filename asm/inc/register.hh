//===================================================================================================================
// register.hh -- functions for managing the registers of an architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-11    #55    v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- This class holds all of the defined registers
//    ---------------------------------------------
class Registers_t {
    Registers_t(const Registers_t &) = delete;
    Registers_t &operator=(const Registers_t &) = delete;



private:
    class Reg_t {
    public:
        std::string regName;
        int bits;

    public:
        Reg_t(const std::string &n, int b) : regName(n), bits(b) {}
        virtual ~Reg_t() {}
    };



private:
    static Registers_t *singleton;
    static Reg_t *registers[MAX_REGISTERS];



private:
    Registers_t(void) {}
    virtual ~Registers_t() {}



public:
    static Registers_t *Get(void) { if (!singleton) singleton = new Registers_t; return singleton; }



public:
    static bool Add(const std::string &r, int b);
    static void Dump(void);
    static bool Find(const std::string &n) { return Search(n) != nullptr; }
    static int Size(const std::string &n) { Reg_t *r = Search(n); return (r==nullptr?-1:r->bits); }



private:
    static Reg_t *Search(const std::string &r);
};




