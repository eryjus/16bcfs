//===================================================================================================================
// opcode.c -- functions for managing the opcodes supported by the architecture
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


//
// -- An opcode definition
struct OpCode_t {
    char *opcodeDef;
    char *byteStream;
};


//
// -- The list of opcodes that are known about the architecture
//    ---------------------------------------------------------
struct OpCode_t opcodeList[MAX_OPCODES] = { { 0 } };


//
// -- This string is an enumeration of all the core opcodes
//    -----------------------------------------------------
static char enumString[4096 * 16] = {0};        // This is a 16-page long string for holding the opcodes



//
// -- Add a new opcode to the enum string
//    -----------------------------------
void AddToEnum(char *def, char *bytes)
{
    char opcode[64] = {0};
    for (int i = 0; i < strlen(def); i ++) {
        opcode[i] = toupper(def[i]);
        if (opcode[i] < '0' || (opcode[i] > '9' && opcode[i] < 'A') || opcode[i] > 'Z') opcode[i] = '_';
    }

    strcat(enumString, "    OPCODE_");
    strcat(enumString, opcode);
    strcat(enumString, " = 0x");

    for (int i = 0; i < strlen(bytes); i ++) {
        if (bytes[i] == ' ' || bytes[i] == '\t') break;

        int l = strlen(enumString);
        enumString[l] = bytes[i];
    }

    strcat(enumString, ",\n");
};


//
// -- Output the enum constants to the opcodes file
//    ---------------------------------------------
void OutputEnum(void)
{
    static char *preamble =
        "//===================================================================================================================\n"
        "// opcode.h -- This file is generated by the assembler.  Do not modify!\n"
        "//===================================================================================================================\n"
        "\n"
        "\n"
        "enum {\n";

    FILE *fp = fopen("opcodes.h", "w");

    if (!fp) {
        Fatal("Unable to open opcodes.h", 0, 0, 0, 0);
    }

    fprintf(fp, "%s", preamble);
    fprintf(fp, "%s", enumString);
    fprintf(fp, "};\n\n");
    fclose(fp);
}



//
// -- converts a hex character to an integer number
//    ---------------------------------------------
int hex2int(char h)
{
    if (h >= '0' && h <= '9') return h - '0';
    if (h >= 'A' && h <= 'F') return (h - 'A') + 10;
    if (h >= 'a' && h <= 'f') return (h - 'a') + 10;

    Fatal("Incorrect HEX character: %c", __FILE__, __LINE__, "", 0, h);

    return -1;
}


//
// -- add an opcode to the list.  opcodes can be duplicated, but later definitions will be unreachable.
//    -------------------------------------------------------------------------------------------------
void AddOpcode(char *def, char *bytes)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (opcodeList[i].opcodeDef == 0 && opcodeList[i].byteStream == 0) {
            opcodeList[i].opcodeDef = def;
            opcodeList[i].byteStream = bytes;

            int idx = 0;
            int j;

            for (j = 0; j < strlen(bytes); j ++) {
                if (bytes[j] == ' ') break;
                idx = (idx << 4) + hex2int(bytes[j]);
            }

            if (j != organization / 4) {
                Error("Machine Code does not match organization size", sourceFile, yylineno, 0, 0);
            }

            if (opTable16[idx] == 0) opTable16[idx] = def;

            return;
        }
    }

    Fatal("Too many opcode definitions", sourceFile, yylineno, 0, 0);
}


//
// -- Dump all known opcodes
//    ----------------------
void DumpOpcodes(void)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (opcodeList[i].opcodeDef == 0 || opcodeList[i].byteStream == 0) continue;
        Msg("%04.4x: Opcode definition `%s` outputs bytes `%s`", i, opcodeList[i].opcodeDef, opcodeList[i].byteStream);
    }
}


//
// -- Normalize an opcode definition to have a consistent number of spaces, trimming the front and back
//    -------------------------------------------------------------------------------------------------
char *NormalizeInstruction(char *line)
{
    int len = strlen(line) + 1;
    char *rv = malloc(len);
    char *ch = line;
    char *out = rv;

    memset(rv, 0, len);

    // -- start by skipping all leading blanks
    while (*ch == ' ' || *ch == '\t') ch ++;

    // -- now move in the mnemonic with a separating space
    while (*ch != ' ' && *ch != '\t') {
        if (*ch == 0) return rv;
        *out ++ = *ch ++;
    }

    *out ++ = ' ';

    // -- the rest just needs to have any blanks dropped until the end of the input line
    while (*ch) {
        if (*ch != ' ' && *ch != '\t') {
            *out ++ = *ch;
        }

        ch ++;
    }

    if (*(-- out) == ' ') *out = 0;

    return rv;
}


//
// -- parse a number string into an integer and return its value
//    ----------------------------------------------------------
int ParseNumber(char *_s, uint64_t *val, int base)
{
    // -- TODO: add some protections into this function
    char *s = _s;
    *val = 0;

    while (*s) {
        *s = tolower(*s);

        int d;

        if (*s >= '0' && *s <= '9') {
            d = *s - '0';
        } else if (*s >= 'a' && *s <= 'f') {
            d = *s - 'a' + 10;
        } else if (*s >= 'A' && *s <= 'F') {
            d = *s - 'A' + 10;
        } else {
            break;
        }

        *val = *val * base + d;
        s ++;
    }

    return s - _s;
}


//
// -- some quick functions which will parse the fundamental base numbers (2, 8, 10, 16)
//    ---------------------------------------------------------------------------------
int ParseDecimal(char *s, uint64_t *v) { return ParseNumber(s, v, 10); }        // starts with a digit other than 0
int ParseBinary(char *s, uint64_t *v) { return ParseNumber(s + 2, v, 2) + 2; }  // starts with '0b'
int ParseOctal(char *s, uint64_t *v) { return ParseNumber(s, v, 8); }           // starts with a '0'
int ParseHex(char *s, uint64_t *v) { return ParseNumber(s + 2, v, 16) + 2; }    // starts with '0x'


//
// -- Look up an opcode in the table and perform any substitution required
//    --------------------------------------------------------------------
struct OpCode_t *FindInstruction(char *instr, uint64_t *val)
{
    for (int i = 0; i < MAX_OPCODES; i ++) {
        if (!opcodeList[i].opcodeDef) break;

        char *mLoc = strstr(opcodeList[i].opcodeDef, "$(");


        if (mLoc == 0) {
            if (strcmp(instr, opcodeList[i].opcodeDef) == 0) {
                return &opcodeList[i];
            }
        } else {
            // -- get the position of the possible number in the instruction
            size_t len = mLoc - opcodeList[i].opcodeDef;
            if (strncmp(instr, opcodeList[i].opcodeDef, len) != 0) continue;

            // -- a number will start with, well, a digit; anything else is not a number
            if (instr[len] < '0' || instr[len] > '9') {
                continue;
            }

            // -- well, we know we have a number to convert; what kind is it?
            int numDigits;

            if (instr[len] != '0') {
                numDigits = ParseDecimal(instr + len, val);
            } else if (instr[len + 1] == 'b') {
                numDigits = ParseBinary(instr + len, val);
            } else if (instr[len + 1] == 'x') {
                numDigits = ParseHex(instr + len, val);
            } else {
                numDigits = ParseOctal(instr + len, val);
            }

            if (strncmp(mLoc, "$(8)", 4) == 0) {
                if (strcmp(instr + len + numDigits, opcodeList[i].opcodeDef + len + 4) == 0) {
                    if ((*val & 0xff) == *val) {
                        return &opcodeList[i];
                    }
                } else continue;
            }

            if (strncmp(mLoc, "$(16)", 5) == 0) {
                if (strcmp(instr + len + numDigits, opcodeList[i].opcodeDef + len + 5) == 0) {
                    if ((*val & 0xffff) == *val) {
                        return &opcodeList[i];
                    }
                } else continue;
            }
        }
    }

    Error("Unknown instruction: %s", sourceFile, yylineno, 0, 0, instr);
    return 0;
}


//
// -- Parse an instruction from the source code and emit its binary result
//    --------------------------------------------------------------------
void ParseInstruction(char *line)
{
    uint64_t val = 0;

    struct OpCode_t *instr = FindInstruction(line, &val);
    EmitInstruction(instr->byteStream, val);
}



