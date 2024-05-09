//===================================================================================================================
// cond.hh -- Handle all conditional execution constructs
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-11    #55    v0.0.1   Initial Version
//===================================================================================================================



#pragma once

#include "opcode.hh"



//
// -- This is the definition of a prefix or suffix
//    --------------------------------------------
class Condition {
protected:
    std::string name;
    uint64_t value;

public:
    Condition(const std::string &c, uint64_t v) : name(c), value(v) {}
    virtual ~Condition() {}

public:
    virtual std::string Name(void) const { return name; }
    virtual uint64_t Value(void) const { return value; }

public:
    virtual std::string GenerateOpcode(const std::string &op) = 0;
};


//
// -- This is a Prefix
//    ----------------
class PrefixedCondition : public Condition {
public:
    PrefixedCondition(const std::string &c, uint64_t v) : Condition(c, v) {}
    ~PrefixedCondition() {}

public:
    virtual std::string GenerateOpcode(const std::string &op) { return name + op; };
};



//
// -- This is a Suffix
//    ----------------
class SuffixedCondition : public Condition {
public:
    SuffixedCondition(const std::string &c, uint64_t v) : Condition(c, v) {}
    ~SuffixedCondition() {}

public:
    virtual std::string GenerateOpcode(const std::string &op);
};



//
// -- This class will handle all the conditional execution code generation
//    --------------------------------------------------------------------
class Conditionals {
    Conditionals(const Conditionals &) = delete;
    Conditionals &operator=(const Conditionals &) = delete;

private:
    static Conditionals *singleton;

private:
    uint64_t bits;
    uint64_t dft;
    std::map<std::string, Condition *> map;

public:
    static inline Conditionals &Get(void) { if (!singleton) singleton = new Conditionals(); return *singleton; }
    static inline bool IsAlloc(void) { return singleton != nullptr; }

public:
    inline Conditionals &Bits(const uint64_t b) { bits = b; return *this; }
    inline uint64_t GetBits(void) const { return bits; }
    inline Conditionals &Default(const uint64_t d) { dft = d; return *this; }
    inline uint64_t GetDefault(void) const { return dft; }

private:
    Conditionals(void) { bits = 4; dft = 0; map.clear(); }
    virtual ~Conditionals() {}

public:
    void AddPrefix(const std::string name, uint64_t prefix);
    void AddSuffix(const std::string name, uint64_t suffix);
    void AddOpcodes(const std::string name, const std::string bytes);
    void AddOpcodes(const std::string name, const std::string bytes, Operand_t op1);
    void AddOpcodes(const std::string name, const std::string bytes, Operand_t op1, Operand_t op2);
    void AddOpcodes(const std::string name, const std::string bytes, Operand_t op1, Operand_t op2, Operand_t op3);
};



//
// -- an inline access function
//    -------------------------
inline Conditionals &cond(void) { return Conditionals::Get(); }


