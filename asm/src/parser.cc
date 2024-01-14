//===================================================================================================================
// parser.cc -- main entry point and top-down parser (hand-coded)
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This file contains a hand-coded top-down parser for all assembly files.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//  2023-Jun-27  Initial  v0.0.7   Rename to C++ source
//===================================================================================================================


#include "asm.hh"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



//
// -- Some static members
//    -------------------
Parser_t *Parser_t::singleton = nullptr;
FILE *Parser_t::listingFile = nullptr;
int Parser_t::tok = 0;
char *Parser_t::sourceFile;


//
// -- scanner interface function
//    --------------------------
int yylex (void);



//
// -- Advance to the next token
//    -------------------------
#define ADVANCE_TOKEN do { tok = yylex(); /* printf("%d\n", tok); */ } while (0)

//
// -- The current token
//    -----------------
#define CURRENT_TOKEN tok


//
// -- Does the current token match what is considered (t)?
//    ----------------------------------------------------
#define MATCH(t) (tok == t)


//
// -- There is some unimplemented feature of the assembler which is causing a problem
//    -------------------------------------------------------------------------------
#define UNIMPLEMENTED   do {                                                                                \
    fprintf(stderr, "Unimplemented token in parser (%s:%d): %d\n", __FILE__, __LINE__, CURRENT_TOKEN);      \
    exit(EXIT_FAILURE);                                                                                     \
} while (0)


//
// -- If there is an error, perform recovery until we reach the end of the line and resume parsing
//    --------------------------------------------------------------------------------------------
#define RECOVERY        do {                                                                                \
        do {                                                                                                \
            ADVANCE_TOKEN;                                                                                  \
        } while (!MATCH(TOK_EOL));                                                                          \
        ADVANCE_TOKEN;                                                                                      \
    } while (0)



//
// -- Function to open a source file
//    ------------------------------
void OpenFile (const char *file);



//
// -- Perform a register definition:
//    .register reg-name bits-wide\n
//    ------------------------------
void Parser_t::ParseRegisterDef(void)
{
    if (!MATCH(TOK_ARCH_NAME)) {
        Messaging::Error("Expected a register name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *regName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("Specify the number of bits wide the register is", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    Registers_t::Add(regName, yylval.number);

    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Messaging::Error("Too many tokens on line after register bit width: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


//
// -- Perform a memory definition:
//    .memory from-address to-address\n
//    ---------------------------------
void Parser_t::ParseMemoryDef(void)
{
    const char *n = "";

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("Expected memory address range starting address", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t fromAddr = yylval.number;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("Expected memory address range ending address", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t toAddr = yylval.number;

    if (!MATCH(TOK_EOL)) {
        ADVANCE_TOKEN;
    }

    if (MATCH(TOK_ARCH_NAME)) {
        n = yylval.name;
    }

    Memory::AddRange(fromAddr, toAddr, n);
    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Messaging::Error("Too many tokens on line after memory address range: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


//
// -- Perform some standardization of an opcode definition, making it easier to match
//    -------------------------------------------------------------------------------
char *Parser_t::StandardizeOpcodeDef(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = new char[len];
    char *ch = yylval.name;
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
// -- The byte stream for an opcode also needs to be standardized to make it easier to output
//    ---------------------------------------------------------------------------------------
char *Parser_t::StandardizeByteStream(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = new char[len];
    char *ch = yylval.name;
    char *out = rv;

    memset(rv, 0, len);

    // -- start by skipping all leading blanks
    while (*ch == ' ' || *ch == '\t') ch ++;

    // -- now move in each byte with 1 space in between
    while (*ch) {
        while (*ch != ' ' && *ch != '\t') {
            if (*ch == 0) goto exit;
            *out ++ = *ch ++;
        }

        // -- if we are inserting a constant, wrap it up and return
        if (*ch == '$' || *ch == '%') {
            while (*ch && *ch != ' ' && *ch != '\t') {
                *out ++ = *ch ++;
            }

            goto exit;
        }

        while (*ch == ' ' || *ch == '\t') ch ++;

        *out ++ = ' ';
    }

    // -- back out the final space
    *(-- out) = 0;

exit:
    return rv;
}


//
// -- Perform an opcode definition:
//    -----------------------------
void Parser_t::ParseOpcodeDef(void)
{
    char *defn = StandardizeOpcodeDef();
    ADVANCE_TOKEN;

    if(!MATCH(TOK_OPCODE_MC) && !MATCH(TOK_OPCODE_DB)) {
        Messaging::Error("Opcode definition requires a byte stream to emit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (MATCH(TOK_OPCODE_DB)) {
        Messaging::Warning(".db in .opcode definition is deprecated.  .mc is assumed and .db will be removed in the future.", sourceFile, yylineno, 0, 0);
    }

    ADVANCE_TOKEN;

    char *bytes = StandardizeByteStream();

    OpCodes::AddEnum(defn, bytes);

    if (Conditionals::IsAlloc()) cond().AddOpcodes(defn, bytes);
    else OpCodes::Add(defn, bytes);

    ADVANCE_TOKEN;
}


//
// -- Set the Organization
//    --------------------
void Parser_t::ParseOrganization(void)
{
    if (!MATCH(TOK_ARCH_NUMBER))
    {
        Messaging::Error("Expected a number in `.organization` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (yylval.number != 8 && yylval.number != 16) {
        Messaging::Error("`.organization` directive must specify either 8 or 16 bits", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (yylval.number == 8) Binary_8bit_t::Organization();
    else if (yylval.number == 16) Binary_16bit_t::Organization();

    ADVANCE_TOKEN;
}


//
// -- Parse data
//    ----------
void Parser_t::ParseData(void)
{
    if (!MATCH(TOK_NUMBER)) {
        Messaging::Error("Expected a data element in `.db` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (yylval.number < 0 || yylval.number > 255) {
        Messaging::Error("Number out of range in `.db` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    Binary_t::EmitByte((uint8_t)yylval.number);
    ADVANCE_TOKEN;
}


//
// -- set the new location in the output binary
//    -----------------------------------------
void Parser_t::GetNewLocation(void)
{
    if (!MATCH(TOK_NUMBER)) {
        Messaging::Error("`.org` location expected", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    Binary_t::SetPosition(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the number of bits a conditional takes up
//    -----------------------------------------------
void Parser_t::ParseCondBits(void)
{
    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("`.cond-bits` requires the number of bits needed to represent condition", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().Bits(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the default condition
//    ---------------------------
void Parser_t::ParseCondDefault(void)
{
    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("`.cond-default` requires the value of the default condition", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().Default(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the a condition added as a prefix to the opcode
//    -----------------------------------------------------
void Parser_t::ParseCondPrefix(void)
{
    if (!(MATCH(TOK_ARCH_NAME) || MATCH(TOK_ARCH_NAME_PREFIX))) {
        Messaging::Error("`.cond-prefix` requires a prefix name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *prefixName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("Specify the number for the prefix", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t prefix = yylval.number;

    if (prefix >> cond().GetBits() != 0) {
        Messaging::Error("The prefix does not fit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().AddPrefix(prefixName, prefix);
    ADVANCE_TOKEN;
}


//
// -- Parse the a condition added as a suffix to the opcode
//    -----------------------------------------------------
void Parser_t::ParseCondSuffix(void)
{
    if (!(MATCH(TOK_ARCH_NAME) || MATCH(TOK_ARCH_NAME_SUFFIX))) {
        Messaging::Error("`.cond-suffix` requires a suffix name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *suffixName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Messaging::Error("Specify the number for the suffix", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t suffix = yylval.number;

    if (suffix >> cond().GetBits() != 0) {
        Messaging::Error("The suffix does not fit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().AddSuffix(suffixName, suffix);
    ADVANCE_TOKEN;
}


//
// -- Top level file parser
//    ---------------------
void Parser_t::ParseFile(void)
{
    ADVANCE_TOKEN;

    while (CURRENT_TOKEN != 0) {
        switch (CURRENT_TOKEN) {
        case TOK_EOL:                   // -- handle an empty line (or one with just a comment)
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_REGISTER:
            ADVANCE_TOKEN;
            ParseRegisterDef();
            break;

        case TOK_ARCH_MEMORY:
            ADVANCE_TOKEN;
            ParseMemoryDef();
            break;

        case TOK_ARCH_OPCODE:
            ADVANCE_TOKEN;
            ParseOpcodeDef();
            break;

        case TOK_ERROR:
            Messaging::Error(yylval.errorMsg, sourceFile, yylineno, 0, 0);
            RECOVERY;
            break;

        case TOK_ORG:
            if (!Binary_t::IsInitialized()) Binary_t::BinaryInit();

            ADVANCE_TOKEN;
            GetNewLocation();
            break;

        case TOK_LABEL:
            if (!Binary_t::IsInitialized()) {
                Binary_t::BinaryInit();
            }

            NoteLabelLocation();
            ADVANCE_TOKEN;
            break;

        case TOK_DB:
            if (!Binary_t::IsInitialized()) {
                Binary_t::BinaryInit();
            }

            ADVANCE_TOKEN;
            ParseData();
            break;

        case TOK_INSTRUCTION:
            if (!Binary_t::IsInitialized()) {
                Binary_t::BinaryInit();
            }

            OpCodes::ParseInstruction(OpCodes::NormalizeInstruction(yylval.name));
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_ORGANIZATION:
            if (Binary_t::IsAlloc()) {
                Messaging::Warning("Organization is initialized already; extra specification ignored\n"
                        "This could have been an implicit initialization as `.organization` needs to be first\n",
                        __FILE__, __LINE__, "", 0);

                break;
            }

            ADVANCE_TOKEN;
            ParseOrganization();

            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_BIG_ENDIAN:
            if (Binary_t::EndianInitialized()) {
                Messaging::Warning("Endianness has already been initialized; extra specification ignored\n",
                        __FILE__, __LINE__, "", 0);
            } else {
                Binary_t::SetEndian(Binary_t::BIG);
            }

            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_LITTLE_ENDIAN:
            if (Binary_t::EndianInitialized()) {
                Messaging::Warning("Endianness has already been initialized; extra specification ignored\n",
                        __FILE__, __LINE__, "", 0);
            } else {
                Binary_t::SetEndian(Binary_t::LITTLE);
            }

            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_COND_BITS:
            ADVANCE_TOKEN;
            ParseCondBits();
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_COND_DEFAULT:
            ADVANCE_TOKEN;
            ParseCondDefault();
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_COND_PREFIX:
            ADVANCE_TOKEN;
            ParseCondPrefix();
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_COND_SUFFIX:
            ADVANCE_TOKEN;
            ParseCondSuffix();
            ADVANCE_TOKEN;
            break;

        default:
            UNIMPLEMENTED;
            RECOVERY;
        }
    }
}


