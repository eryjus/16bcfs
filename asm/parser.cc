//===================================================================================================================
// parser.cc -- main entry point and top-down parser (hand-coded)
//
//  This file contains a hand-coded top-down parser for all assembly files.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//  2023-Jun-27  Initial  v0.0.7   Rename to C++ source
//===================================================================================================================


#include "asm.h"
#include "asm.hh"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


//
// -- scanner interface function
//    --------------------------
extern "C" int yylex (void);


//
// -- This is the current token -- lookahead of 1
//    -------------------------------------------
int tok;


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
extern "C" void OpenFile (const char *file);

//
// -- The name of the source file
//    ---------------------------
char *sourceFile;


//
// -- The file handle where the output listing will be placed (if not 0)
//    ------------------------------------------------------------------
FILE *listingFile = 0;



//
// -- Perform a register definition:
//    .register reg-name bits-wide\n
//    ------------------------------
void ParseRegisterDef(void) {
    if (!MATCH(TOK_ARCH_NAME)) {
        Error("Expected a register name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *regName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Specify the number of bits wide the register is", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    AddRegister(regName, yylval.number);

    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Error("Too many tokens on line after register bit width: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


//
// -- Perform a memory definition:
//    .memory from-address to-address\n
//    ---------------------------------
void ParseMemoryDef(void) {
    const char *n = "";

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Expected memory address range starting address", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t fromAddr = yylval.number;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Expected memory address range ending address", sourceFile, yylineno, 0, 0);
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

    AddMemoryRange(fromAddr, toAddr, n);
    ADVANCE_TOKEN;

    if (!MATCH(TOK_EOL)) {
        Error("Too many tokens on line after memory address range: %d", sourceFile, yylineno, 0, 0, CURRENT_TOKEN);
        RECOVERY;
        return;
    }

    ADVANCE_TOKEN;
}


//
// -- Perform some standardization of an opcode definition, making it easier to match
//    -------------------------------------------------------------------------------
char *StandardizeOpcodeDef(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = (char *)malloc(len);
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
char *StandardizeByteStream(void)
{
    int len = strlen(yylval.name) + 1;
    char *rv = (char *)malloc(len);
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
void ParseOpcodeDef(void)
{
    char *defn = StandardizeOpcodeDef();
    ADVANCE_TOKEN;

    if(!MATCH(TOK_OPCODE_MC) && !MATCH(TOK_OPCODE_DB)) {
        Error("Opcode definition requires a byte stream to emit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (MATCH(TOK_OPCODE_DB)) {
        Warning(".db in .opcode definition is deprecated.  .mc is assumed and .db will be removed in the future.", sourceFile, yylineno, 0, 0);
    }

    ADVANCE_TOKEN;

    char *bytes = StandardizeByteStream();

    AddToEnum(defn, bytes);

    if (Conditionals::IsInit()) cond().AddOpcodes(defn, bytes);
    else AddOpcode(defn, bytes);

    ADVANCE_TOKEN;
}


//
// -- Set the Organization
//    --------------------
void ParseOrganization(void)
{
    if (!MATCH(TOK_ARCH_NUMBER))
    {
        Error("Expected a number in `.organization` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (yylval.number != 8 && yylval.number != 16) {
        Error("`.organization` directive must specify either 8 or 16 bits", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    organization = yylval.number;
    ADVANCE_TOKEN;
}


//
// -- Parse data
//    ----------
void ParseData(void)
{
    if (!MATCH(TOK_NUMBER)) {
        Error("Expected a data element in `.db` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    if (yylval.number < 0 || yylval.number > 255) {
        Error("Number out of range in `.db` directive", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    EmitByte((uint8_t)yylval.number);
    ADVANCE_TOKEN;
}


//
// -- set the new location in the output binary
//    -----------------------------------------
void GetNewLocation(void)
{
    if (!MATCH(TOK_NUMBER)) {
        Error("`.org` location expected", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    SetPosition(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the number of bits a conditional takes up
//    -----------------------------------------------
void ParseCondBits(void)
{
    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("`.cond-bits` requires the number of bits needed to represent condition", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().Bits(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the default condition
//    ---------------------------
void ParseCondDefault(void)
{
    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("`.cond-default` requires the value of the default condition", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().Default(yylval.number);
    ADVANCE_TOKEN;
}


//
// -- Parse the a condition added as a prefix to the opcode
//    -----------------------------------------------------
void ParseCondPrefix(void)
{
    if (!(MATCH(TOK_ARCH_NAME) || MATCH(TOK_ARCH_NAME_PREFIX))) {
        Error("`.cond-prefix` requires a prefix name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *prefixName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Specify the number for the prefix", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t prefix = yylval.number;

    if (prefix >> cond().GetBits() != 0) {
        Error("The prefix does not fit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().AddPrefix(prefixName, prefix);
    ADVANCE_TOKEN;
}


//
// -- Parse the a condition added as a suffix to the opcode
//    -----------------------------------------------------
void ParseCondSuffix(void)
{
    if (!(MATCH(TOK_ARCH_NAME) || MATCH(TOK_ARCH_NAME_SUFFIX))) {
        Error("`.cond-suffix` requires a suffix name", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    char *suffixName = yylval.name;
    ADVANCE_TOKEN;

    if (!MATCH(TOK_ARCH_NUMBER)) {
        Error("Specify the number for the suffix", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    uint64_t suffix = yylval.number;

    if (suffix >> cond().GetBits() != 0) {
        Error("The suffix does not fit", sourceFile, yylineno, 0, 0);
        RECOVERY;
        return;
    }

    cond().AddSuffix(suffixName, suffix);
    ADVANCE_TOKEN;
}


//
// -- Top level file parser
//    ---------------------
void ParseFile(void)
{
    extern int binaryInitialized;
    extern int organizationInitialized;
    extern int endianInitialized;

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
            Error(yylval.errorMsg, sourceFile, yylineno, 0, 0);
            RECOVERY;
            break;

        case TOK_ORG:
            if (!binaryInitialized) {
                BinaryInit();
                binaryInitialized = 1;
            }

            ADVANCE_TOKEN;
            GetNewLocation();
            break;

        case TOK_LABEL:
            if (!binaryInitialized) {
                BinaryInit();
                binaryInitialized = 1;
            }

            NoteLabelLocation();
            ADVANCE_TOKEN;
            break;

        case TOK_DB:
            if (!binaryInitialized) {
                BinaryInit();
                binaryInitialized = 1;
            }

            ADVANCE_TOKEN;
            ParseData();
            break;

        case TOK_INSTRUCTION:
            if (!binaryInitialized) {
                BinaryInit();
                binaryInitialized = 1;
            }

            ParseInstruction(NormalizeInstruction(yylval.name));
            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_ORGANIZATION:
            if(!organizationInitialized) {
                ADVANCE_TOKEN;
                ParseOrganization();
                organizationInitialized = 1;
            }

            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_BIG_ENDIAN:
            if (!endianInitialized) {
                SetBigEndian();
                endianInitialized = 1;
            }

            ADVANCE_TOKEN;
            break;

        case TOK_ARCH_LITTLE_ENDIAN:
            if (!endianInitialized) {
                SetLittleEndian();
                endianInitialized = 1;
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


//
// -- simple hack to divide the EEPROM -- hard coded!!
//    ------------------------------------------------
void SplitOutput(void)
{
    FILE *in = fopen("output.bin", "r");
    FILE *out1 = fopen("eeprom-lsb.bin", "w");
    FILE *out2 = fopen("eeprom-msb.bin", "w");

    if (!in || !out1 || !out2) {
        fprintf(stderr, "Unable to open files for splitting\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 32 * 1024; i ++) {
        uint8_t lsb;
        uint8_t msb;

        fread(&msb, 1, 1, in);      // -- this is in little endian order
        fread(&lsb, 1, 1, in);

        fwrite(&lsb, 1, 1, out1);
        fwrite(&msb, 1, 1, out2);
    }

    fflush(out1);
    fflush(out2);

    fclose(in);
    fclose(out1);
    fclose(out2);
}


//
// -- main entry point
//    ----------------
int main(int argc, char *argv[])
{
    char dftSearch[256];

    sprintf(dftSearch, "%s/%s/", getenv("HOME"), "bin/arch");
    searchList[0] = dftSearch;

    for (int i = 1; i < argc; i ++) {
        if (strcmp(argv[i], "-I") == 0) {
            i ++;
            for (int j = 0; j < MAX_SEARCH_PATH; j ++) {
                if (searchList[j] == 0) {
                    searchList[j] = argv[i];
                    break;
                }
            }

            fprintf(stderr, "Too many search paths\n");
            exit(EXIT_FAILURE);
        } else if (strncmp(argv[i], "-l=-", 4) == 0) {
            listingFile = stdout;
        } else if (strncmp(argv[i], "-l=", 3) == 0) {
            listingFile = fopen(argv[i] + 3, "w");
            if (!listingFile) {
                Fatal("Unable to open listing output file", __FILE__, __LINE__, "", 0);
            }
        } else if (strncmp(argv[i], "-l", 2) == 0) {
            listingFile = fopen("output.txt", "w");
            if (!listingFile) {
                Fatal("Unable to open listing output file", __FILE__, __LINE__, "", 0);
            }
        } else {
            sourceFile = argv[i];
        }
    }

    OpenFile(sourceFile);
    ParseFile();
    OutputListing();
    OutputResults();

    if (listingFile) {
        fflush(listingFile);
        if (listingFile != stdout) fclose(listingFile);
    }

    SplitOutput();
    OutputEnum();

    return EXIT_SUCCESS;
}

