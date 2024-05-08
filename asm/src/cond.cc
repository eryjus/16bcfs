//===================================================================================================================
// cond.cc -- Handle all conditional execution constructs
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Jun-26  Initial  v0.0.7   Initial Version
//
//===================================================================================================================



#include "asm.hh"



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
    sprintf(v, "%x", (int)GetDefault());

    // -- handle the undecorated case here
    int pos = opcode.find(' ');
    std::string op = opcode;
    if (pos != std::string::npos) op = opcode.substr(0, pos);
    OpCodes::Add(strdup(op.c_str()), strdup((std::string(v) + bytes).c_str()), true);

    for (auto it = map.begin(); it != map.end(); ++ it) {
        sprintf(v, "%x", (int)it->second->Value());
        OpCodes::Add(it->second->GenerateOpcode(opcode).c_str(), (std::string(v) + bytes).c_str());
    }
}



//
// -- Add the opcodes to the opcode table
//    -----------------------------------
void Conditionals::AddOpcodes(const std::string opcode, const std::string bytes, Operand_t op1)
{
    char v[10];
    sprintf(v, "%x", (int)GetDefault());

    // -- handle the undecorated case here
    int pos = opcode.find(' ');
    std::string op = opcode;
    if (pos != std::string::npos) op = opcode.substr(0, pos);
    OpCodes::Add(strdup(op.c_str()), strdup((std::string(v) + bytes).c_str()), op1, true);

    for (auto it = map.begin(); it != map.end(); ++ it) {
        sprintf(v, "%x", (int)it->second->Value());
        OpCodes::Add(it->second->GenerateOpcode(opcode).c_str(), (std::string(v) + bytes).c_str(), op1);
    }
}



//
// -- Add the opcodes to the opcode table
//    -----------------------------------
void Conditionals::AddOpcodes(const std::string opcode, const std::string bytes, Operand_t op1, Operand_t op2)
{
    char v[10];
    sprintf(v, "%lX", GetDefault());

    // -- handle the undecorated case here
    int pos = opcode.find(' ');
    std::string op = opcode;
    if (pos != std::string::npos) op = opcode.substr(0, pos);
    OpCodes::Add(strdup(op.c_str()), strdup((std::string(v) + bytes).c_str()), op1, op2, true);

    for (auto it = map.begin(); it != map.end(); ++ it) {
        sprintf(v, "%lX", it->second->Value());
        OpCodes::Add(it->second->GenerateOpcode(opcode).c_str(), (std::string(v) + bytes).c_str(), op1, op2);
    }
}



//
// -- Add the opcodes to the opcode table
//    -----------------------------------
void Conditionals::AddOpcodes(const std::string opcode, const std::string bytes, Operand_t op1, Operand_t op2, Operand_t op3)
{
    char v[10];
    sprintf(v, "%lX", GetDefault());

    // -- handle the undecorated case here
    int pos = opcode.find(' ');
    std::string op = opcode;
    if (pos != std::string::npos) op = opcode.substr(0, pos);
    OpCodes::Add(strdup(op.c_str()), strdup((std::string(v) + bytes).c_str()), op1, op2, op3, true);

    for (auto it = map.begin(); it != map.end(); ++ it) {
        sprintf(v, "%lX", it->second->Value());
        OpCodes::Add(it->second->GenerateOpcode(opcode).c_str(), (std::string(v) + bytes).c_str(), op1, op2, op3);
    }
}



//
// -- Generate a name for a suffixed opcode -- critically checking for a ' ' and inserting the suffix there
//    -----------------------------------------------------------------------------------------------------
std::string SuffixedCondition::GenerateOpcode(const std::string &op)
{
    std::string rv = "";

    int loc = op.find(' ');

    if (loc == std::string::npos) {
        rv = op + name;
    } else {
        rv = op.substr(0, loc) + name /* + op.substr(loc) */;
    }

    return rv;
}


