//===================================================================================================================
// cond.cc -- Handle all conditional execution constructs
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Jun-26  Initial  v0.0.7   Initial Version
//
//===================================================================================================================


#include "asm.hh"
#include "asm.h"


//
// -- This is the singleton instance of Conditionals
//    ----------------------------------------------
Conditionals *Conditionals::singleton = nullptr;


//
// -- Add a prefix string to the map; err if the name already exists
//    --------------------------------------------------------------
void Conditionals::AddPrefix(std::string name, uint64_t prefix)
{
    if (map.find(name) != map.end()) return;

    map[name] = new PrefixedCondition(name, prefix);
}


//
// -- Add a suffix string to the map; err if the name already exists
//    --------------------------------------------------------------
void Conditionals::AddSuffix(std::string name, uint64_t suffix)
{
    if (map.find(name) != map.end()) return;

    map[name] = new SuffixedCondition(name, suffix);
}


//
// -- Add the opcodes to the opcode table
//    -----------------------------------
void Conditionals::AddOpcodes(const std::string opcode, const std::string bytes)
{
    char v[10];
    sprintf(v, "%lX", GetDefault());

    // -- handle the undecorated case here
    AddOpcode(strdup(opcode.c_str()), strdup((std::string(v) + bytes).c_str()));

    for (auto it = map.begin(); it != map.end(); ++ it) {
        sprintf(v, "%lX", it->second->Value());
        AddOpcode(strdup(it->second->GenerateOpcode(opcode).c_str()), strdup((std::string(v) + bytes).c_str()));
    }
}


//
// -- Generate a name for a suffixed opcode -- critically checking for a ' ' and inserting the suffix there
//    -----------------------------------------------------------------------------------------------------
std::string SuffixedCondition::GenerateOpcode(const std::string &op)
{
    std::string rv = "";

    int loc = op.find_first_of(' ');

    if (loc == -1) rv = op + name;
    else rv = op.substr(0, loc) + name + op.substr(loc, op.length() - loc);

    return rv;
}


