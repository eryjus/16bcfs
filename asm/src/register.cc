//===================================================================================================================
// register.cc -- functions for managing the registers of an architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//  2024-Jan-11    #55    v0.0.1   Update to C++ and clean up code
//===================================================================================================================



#include "asm.hh"



//
// -- some static members
//    -------------------
Registers_t *Registers_t::singleton = nullptr;
Registers_t::Reg_t *Registers_t::registers[MAX_REGISTERS] = { nullptr };



//
// -- Search the table for the register and return it if found
//    --------------------------------------------------------
Registers_t::Reg_t *Registers_t::Search(const std::string &n)
{
    Get();

    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i] == nullptr) continue;
        if (registers[i]->regName == n) return registers[i];
    }

    return nullptr;
}


//
// -- Add a register to the list.  Checks for duplicates
//    --------------------------------------------------
bool Registers_t::Add(const std::string &r, int b)
{
    Get();

    if (Search(r) != nullptr) {
        Messaging::Error("Register %s already defined as %d bits wide", Parser_t::GetSourceFile(), yylineno, 0, 0, r.c_str(), b);

        return false;
    }

    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i] == nullptr) {
            registers[i] = new Reg_t(r, b);

            return true;
        }
    }

    // -- we get here and it is not found
    Messaging::Error("Too many registers", Parser_t::GetSourceFile(), yylineno, 0, 0);

    return false;
}


//
// -- Dump the list of registers
//    --------------------------
void Registers_t::Dump(void)
{
    Get();

    for (int i = 0; i < MAX_REGISTERS; i ++)
    {
        if (registers[i] == nullptr) continue;

        Messaging::Msg("Register %s is %d bytes wide\n", registers[i]->regName.c_str(), registers[i]->bits);
    }
}




