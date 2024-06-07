//===================================================================================================================
//  parser.hh -- This is the parser for the assembler
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  18-May-2024  Initial           Initial Version
//
//===================================================================================================================



#pragma once


//
// -- This is the hand-coded parser
//    -----------------------------
class Parser_t {
private:
    enum {
        ARG_COSNT = 0,
        ARG_REG = 1,
    };

    enum {
        BIN_STATIC = 0,
        BIN_CONST = 1,
    };

    typedef struct {
        std::string mnemonic;
        int argCount;
        int argMask;
        std::string a1Name;
        std::string a2Name;
        std::string a3Name;
        int binCount;
        int binMask;
        uint16_t b1Val;
        uint16_t b2Val;
        uint16_t b3Val;
        uint16_t b4Val;
        uint16_t b5Val;
        uint16_t b6Val;
        uint16_t b7Val;
        uint16_t b8Val;
    } AssemblyTable_t;


private:
    static int tok;
    static AssemblyTable_t table[];
    static int errors;
    static int warnings;
    static AssemblyTable_t *opcodeStart[26];


protected:
    static bool MATCH(int t) { return (tok == t); }
    static void ADVANCE_TOKEN(void) { tok = yylex(); }
    static int CURRENT_TOKEN(void) { return tok; }
    static void UNIMPLEMENTED(void) { std::cerr << "Invalid instruction on line " << yylineno << std::endl; }
    static void RECOVERY(void) { do { ADVANCE_TOKEN(); } while (!MATCH(TOK_EOL) && !MATCH(0)); }


protected:
    static void Initialize(void);
    static void ParseOrgDirective(void);
    static void ParseIncBinDirective(void);
    static void ParseDataWordDirective(void);
    static void ParseInstructionLine(void);
    static AssemblyTable_t *FindExactOpcode(std::string opcode, int opCnt, std::string op1, std::string op2, std::string op3, int constPos);



public:
    static void Parse(void);
    static int GetErrorCount(void) { return errors; }
    static int GetWarningCount(void) { return warnings; }
    static void IncErrors(void) { errors ++; }
    static void OutputHeader(void);
};


