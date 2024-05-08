//===================================================================================================================
// opcode.cc -- functions for managing the opcodes supported by the architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "asm.hh"

#include <algorithm>



//
// -- local members
//    -------------
std::map<std::string, OpCode_t *> OpCodes::opcodes;
std::map<int, std::string> OpCodes::lookup;
OpCodes *OpCodes::singleton = nullptr;
Operand_t *Operand_t::none = new Operand_t;



//
// -- Construct an opcode with no operands
//    ------------------------------------
OpCode_t::OpCode_t(const std::string &m, const std::string &s, bool addEnum)
        : mnenomic(m), op1(Operand_t::None()), op2(Operand_t::None()), op3(Operand_t::None()), byteStream(s)
{
    opcodeDef = m;
    enumStr = "OPCODE_";

    for (int i = 0; i < opcodeDef.length(); i ++) {
        if (opcodeDef[i] >= 'a' && opcodeDef[i] <= 'z') enumStr += std::toupper(opcodeDef[i]);
        else if ((opcodeDef[i] >= '0' && opcodeDef[i] <= '9') || opcodeDef[i] == '_') enumStr += opcodeDef[i];
        else if (enumStr[enumStr.length() - 1] != '_') enumStr += '_';
    }

    enumStr += " = 0x";
    int num = 0;

    for (int i = 0; i < byteStream.length(); i ++) {
        char ch = byteStream[i];

        if (ch == ' ' || ch == '\t') break;

        enumStr += ch;

        if (ch >= '0' && ch <= '9') num = (num * 16) + (ch - '0');
        else if (ch >= 'a' && ch <= 'f') num = (num * 16) + (ch - 'a' + 10);
    }

    OpCodes::AddLookup(num, opcodeDef);

    if (addEnum) enumStr += ",\n";
    else enumStr = "";
}


//
// -- Construct an opcode with 1 operand
//    ----------------------------------
OpCode_t::OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, bool addEnum)
        : mnenomic(m), op1(o1), op2(Operand_t::None()), op3(Operand_t::None()), byteStream(s)
{
    opcodeDef = m + ' ';

    if (op1.type == Operand_t::OPND_REG) opcodeDef += o1.name;
    else opcodeDef += '#';

    enumStr = "OPCODE_";

    for (int i = 0; i < opcodeDef.length(); i ++) {
        if (opcodeDef[i] >= 'a' && opcodeDef[i] <= 'z') enumStr += std::toupper(opcodeDef[i]);
        else if ((opcodeDef[i] >= '0' && opcodeDef[i] <= '9') || opcodeDef[i] == '_') enumStr += opcodeDef[i];
        else if (opcodeDef[i] == '#') enumStr += "IMM";
        else if (enumStr[enumStr.length() - 1] != '_') enumStr += '_';
    }

    enumStr += " = 0x";
    int num = 0;

    for (int i = 0; i < byteStream.length(); i ++) {
        char ch = byteStream[i];

        if (ch == ' ' || ch == '\t') break;

        enumStr += ch;

        if (ch >= '0' && ch <= '9') num = (num * 16) + (ch - '0');
        else if (ch >= 'a' && ch <= 'f') num = (num * 16) + (ch - 'a' + 10);
    }

    OpCodes::AddLookup(num, opcodeDef);

    if (addEnum) enumStr += ",\n";
    else enumStr = "";
}


//
// -- Construct an opcode with 2 operands
//    -----------------------------------
OpCode_t::OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
        bool addEnum)
        : mnenomic(m), op1(o1), op2(o2), op3(Operand_t::None()), byteStream(s)
{
    opcodeDef = m + ' ' + op1.name + ',';

    if (op2.type == Operand_t::OPND_REG) opcodeDef += op2.name;
    else opcodeDef += '#';

    enumStr = "OPCODE_";

    for (int i = 0; i < opcodeDef.length(); i ++) {
        if (opcodeDef[i] >= 'a' && opcodeDef[i] <= 'z') enumStr += std::toupper(opcodeDef[i]);
        else if ((opcodeDef[i] >= '0' && opcodeDef[i] <= '9') || opcodeDef[i] == '_') enumStr += opcodeDef[i];
        else if (opcodeDef[i] == '#') enumStr += "IMM";
        else if (enumStr[enumStr.length() - 1] != '_') enumStr += '_';
    }

    enumStr += " = 0x";
    int num = 0;

    for (int i = 0; i < byteStream.length(); i ++) {
        char ch = byteStream[i];

        if (ch == ' ' || ch == '\t') break;

        enumStr += ch;

        if (ch >= '0' && ch <= '9') num = (num * 16) + (ch - '0');
        else if (ch >= 'a' && ch <= 'f') num = (num * 16) + (ch - 'a' + 10);
    }

    OpCodes::AddLookup(num, opcodeDef);

    if (addEnum) enumStr += ",\n";
    else enumStr = "";
}

//
// -- Construct an opcode with 3 operands
//    -----------------------------------

OpCode_t::OpCode_t(const std::string &m, const std::string &s, const Operand_t &o1, const Operand_t &o2,
        const Operand_t &o3, bool addEnum)
        : mnenomic(m), op1(o1), op2(o2), op3(o3), byteStream(s)
{
    opcodeDef = m + ' ' + op1.name + ',' + op2.name + ',';

    if (op3.type == Operand_t::OPND_REG) opcodeDef += op3.name;
    else opcodeDef += '#';

    enumStr = "OPCODE_";

    for (int i = 0; i < opcodeDef.length(); i ++) {
        if (opcodeDef[i] >= 'a' && opcodeDef[i] <= 'z') enumStr += std::toupper(opcodeDef[i]);
        else if ((opcodeDef[i] >= '0' && opcodeDef[i] <= '9') || opcodeDef[i] == '_') enumStr += opcodeDef[i];
        else if (opcodeDef[i] == '#') enumStr += "IMM";
        else if (enumStr[enumStr.length() - 1] != '_') enumStr += '_';
    }

    enumStr += " = 0x";
    int num = 0;

    for (int i = 0; i < byteStream.length(); i ++) {
        char ch = byteStream[i];

        if (ch == ' ' || ch == '\t') break;

        enumStr += ch;

        if (ch >= '0' && ch <= '9') num = (num * 16) + (ch - '0');
        else if (ch >= 'a' && ch <= 'f') num = (num * 16) + (ch - 'a' + 10);
    }

    OpCodes::AddLookup(num, opcodeDef);

    if (addEnum) enumStr += ",\n";
    else enumStr = "";
}



//
// -- Output the enum constants to the opcodes file
//    ---------------------------------------------
void OpCodes::OutputEnum(void)
{
    static std::string preamble =
        "//===================================================================================================================\n"
        "// opcode.h -- This file is generated by the assembler.  Do not modify!\n"
        "//===================================================================================================================\n"
        "\n"
        "\n"
        "enum {\n";

    FILE *fp = fopen("opcodes.h", "w");

    if (!fp) {
        Messaging::Fatal("Unable to open opcodes.h", 0, 0, 0, 0);
    }

    fprintf(fp, "%s", preamble.c_str());

    for (auto it = opcodes.begin(); it != opcodes.end(); it ++) {
        fprintf(fp, "%s", it->second->enumStr.c_str());
    }

    fprintf(fp, "};\n\n");
    fclose(fp);
}



//
// -- converts a hex character to an integer number
//    ---------------------------------------------
int OpCodes::hex2int(char h)
{
    if (h >= '0' && h <= '9') return h - '0';
    if (h >= 'A' && h <= 'F') return (h - 'A') + 10;
    if (h >= 'a' && h <= 'f') return (h - 'a') + 10;

    Messaging::Fatal("Incorrect HEX character: %c", __FILE__, __LINE__, "", 0, h);

    return -1;
}


//
// -- Dump all known opcodes
//    ----------------------
void OpCodes::Dump(void)
{
    int i = 0;

    for (auto it = opcodes.begin(); it != opcodes.end(); it ++) {
        Messaging::Msg("%04.4x: Key: `%s`: Opcode definition `%s` outputs bytes `%s`",
                ++ i,
                it->first.c_str(),
                it->second->opcodeDef.c_str(),
                it->second->byteStream.c_str());
    }
}


//
// -- Normalize an opcode definition to have a consistent number of spaces, trimming the front and back
//    -------------------------------------------------------------------------------------------------
std::string OpCodes::NormalizeInstruction(const std::string &line)
{
    std::string rv = "";
    int ch = 0;

    // -- start by skipping all leading blanks
    while (line[ch] == ' ' || line[ch] == '\t') ch ++;

    // -- now move in the mnemonic with a separating space
    while (line[ch] != ' ' && line[ch] != '\t') {
        if (line[ch] == 0) return rv;
        rv += line[ch ++];
    }

    rv += ' ';

    // -- the rest just needs to have any blanks dropped until the end of the input line
    while (line[ch]) {
        if (line[ch] != ' ' && line[ch] != '\t') {
            rv += line[ch];
        }

        ch ++;
    }

    if (rv[rv.length() - 1] == ' ') rv[rv.length() - 1] = 0;

    return rv;
}


//
// -- parse a number string into an integer and return its value
//    ----------------------------------------------------------
int OpCodes::ParseNumber(const std::string &s, uint64_t *val, int base)
{
    // -- TODO: add some protections into this function
    int p = 0;
    char ch;
    *val = 0;

    while (s[p]) {
        ch = tolower(s[p ++]);

        int d;

        if (ch >= '0' && ch <= '9') {
            d = ch - '0';
        } else if (ch >= 'a' && ch <= 'f') {
            d = ch - 'a' + 10;
        } else if (ch >= 'A' && ch <= 'F') {
            d = ch - 'A' + 10;
        } else {
            break;
        }

        *val = *val * base + d;
    }

    return p;
}



//
// -- Look up an opcode in the table and perform any substitution required
//    --------------------------------------------------------------------
OpCode_t *OpCodes::FindInstruction(const std::string &instr, const std::string &line, uint64_t *val)
{
    auto it = opcodes.find(instr);

    if (it == opcodes.end()) return nullptr;
    else {
        // -- do we have an optional numeric argument (or label)?
        int pos = it->second->opcodeDef.find('#');
        *val = 0;

        if (pos == std::string::npos) return it->second;

        // -- we do; substring the instruction to find the value or the label name
        std::string num = line.substr(pos);

        if (num[0] == '.' || isalpha(num[0])) {
            Label_t *lbl = Labels::Find(num);

            *val = lbl->location / 2;
        } else {
            // -- well, we know we have a number to convert; what kind is it?
            int numDigits;

            if (num[0] != '0') {
                numDigits = ParseDecimal(num, val);
            } else if (num[1] == 'b') {
                numDigits = ParseBinary(num, val);
            } else if (instr[1] == 'x') {
                numDigits = ParseHex(num, val);
            } else {
                numDigits = ParseOctal(num, val);
            }
        }

        return it->second;
    }
}


//
// -- Parse an instruction from the source code and emit its binary result
//    --------------------------------------------------------------------
void OpCodes::ProcessInstruction(const std::string &model, const std::string &line)
{
    uint64_t val = 0;

    OpCode_t *instr = FindInstruction(model, line, &val);

    if (!instr) {
        Messaging::Error("Unknown instruction", Parser_t::GetSourceFile(), yylineno, line.c_str(), 0);
        return;
    }

    Binary_t::EmitInstruction(instr->byteStream.c_str(), val);
}



