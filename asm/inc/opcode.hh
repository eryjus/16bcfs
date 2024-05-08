//===================================================================================================================
// opcode.hh -- functions for managing the opcodes supported by the architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once

#include <vector>


//
// -- This class holds the definition of an operand for an opcode
//    -----------------------------------------------------------
class Operand_t {
private:
    static Operand_t *none;

public:
    enum { OPND_NONE, OPND_REG, OPND_NUM } type;
    std::string name;
    int numWidth;

public:
    Operand_t(std::string n) { type = OPND_REG; name = n; numWidth = 0; }
    Operand_t(int w) { type = OPND_NUM; name = ""; numWidth = w; }
    Operand_t(void) { type = OPND_NONE; name = ""; numWidth = 0; }

    static Operand_t &None(void) { return *none; }
};


//
// -- This class holds the definition of an opcode within the table
//    -------------------------------------------------------------
class OpCode_t {
public:
    std::string mnenomic;
    Operand_t op1;
    Operand_t op2;
    Operand_t op3;

    std::string opcodeDef;
    std::string byteStream;
    std::string enumStr;

public:
    OpCode_t(const std::string &m, const std::string &s, bool addEnum = false);
    OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, bool addEnum = false);
    OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
            bool addEnum = false);
    OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
            const Operand_t &o3, bool addEnum = false);

    virtual ~OpCode_t() {}

public:
    std::string GetDef(void) const { return opcodeDef; }
};



//
// -- This is the list of OpCodes supported
//    -------------------------------------
class OpCodes {
    OpCodes(const OpCodes &) = delete;
    OpCodes &operator=(const OpCodes &) = delete;


private:
    static OpCodes *singleton;
    static std::map<std::string, OpCode_t *> opcodes;
    static std::map<int, std::string> lookup;


private:
    OpCodes(void) {}
    virtual ~OpCodes() {}


protected:
    static int hex2int(char h);
    static int ParseNumber(const std::string &s, uint64_t *val, int base);
    static OpCode_t *FindInstruction(const std::string &instr, const std::string &line, uint64_t *val);


protected:
    static inline int ParseDecimal(const std::string &s, uint64_t *v) { return ParseNumber(s, v, 10); }
    static inline int ParseBinary(const std::string &s, uint64_t *v) { return ParseNumber(std::string(s.c_str() + 2), v, 2) + 2; }
    static inline int ParseOctal(const std::string &s, uint64_t *v) { return ParseNumber(s, v, 8); }
    static inline int ParseHex(const std::string &s, uint64_t *v) { return ParseNumber(std::string(s.c_str() + 2), v, 16) + 2; }


public:
    static OpCodes *Get(void) { if (!singleton) singleton = new OpCodes; return singleton; }
    static void OutputEnum(void);

    static void Add(const std::string &m, const std::string &s, bool addEnum = false) {
        OpCode_t *op = new OpCode_t(m, s, addEnum);
        Get()->opcodes.emplace(op->GetDef(), op);
    }

    static void Add(const std::string &m, const std::string &s, const Operand_t &o1, bool addEnum = false) {
        OpCode_t *op = new OpCode_t(m, s, o1, addEnum);
        Get()->opcodes.emplace(op->GetDef(), op);
    }

    static void Add(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
            bool addEnum = false) {
        OpCode_t *op = new OpCode_t(m, s, o1, o2, addEnum);
        Get()->opcodes.emplace(op->GetDef(), op);
    }

    static void Add(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
            const Operand_t &o3, bool addEnum = false) {
        OpCode_t *op = new OpCode_t(m, s, o1, o2, o3, addEnum);
        Get()->opcodes.emplace(op->GetDef(), op);
    }

    static void Dump(void);
    static std::string NormalizeInstruction(const std::string &line);
    static void ProcessInstruction(const std::string &model, const std::string &line);
    static void AddLookup(int n, std::string op) { lookup[n] = op; }
    static std::string Lookup(int n) { return lookup[n]; }
};



