//===================================================================================================================
// asm.h -- global header file for all source modules
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// -- Some definitions which set the limits of the assembler
//    ------------------------------------------------------
#define MAX_REGISTERS   1024
#define MAX_SEARCH_PATH 50
#define MAX_OPERANDS    2
#define MAX_OPCODES     65536
#define MAX_LABELS      4096


//
// -- Found in asm.c
//    --------------
extern char *searchList[MAX_SEARCH_PATH];


//
// -- Found in asm.c
//    --------------
FILE *searchPath(char *filename);


//
// -- This is the token definition, which will be returned by the scanner
//    -------------------------------------------------------------------
typedef union yylval_t {
    char *errorMsg;
    uint64_t number;
    char *name;
} yylval_t;


extern yylval_t yylval;
extern char *opTable16[];


//
// -- These are all the tokens recognized by the scanner
//    --------------------------------------------------
enum {
    TOK_EOL = 256,
    TOK_ERROR = 257,
    TOK_ARCH_REGISTER = 258,
    TOK_ARCH_NAME = 259,
    TOK_ARCH_NUMBER = 260,
    TOK_ARCH_MEMORY = 261,
    TOK_ARCH_OPCODE = 262,
    TOK_OPCODE_DB = 263,
    TOK_OPCODE_DEF = 264,
    TOK_BYTE_STREAM_DEF = 265,
    TOK_ORG = 266,
    TOK_NUMBER = 267,
    TOK_LABEL = 268,
    TOK_INSTRUCTION = 269,
    TOK_DB = 270,
    TOK_ARCH_ORGANIZATION = 271,
    TOK_ARCH_BIG_ENDIAN = 272,
    TOK_ARCH_LITTLE_ENDIAN = 273,
    TOK_OPCODE_MC = 274,
    TOK_ARCH_COND_BITS = 275,
    TOK_ARCH_COND_DEFAULT = 276,
    TOK_ARCH_COND_SUFFIX = 277,
    TOK_ARCH_COND_PREFIX = 278,
    TOK_ARCH_NAME_PREFIX = 279,
    TOK_ARCH_NAME_SUFFIX = 280,
};


//
// -- Used by the scanner to keep track of the current location (See parser.c and lexer.ll)
//    -------------------------------------------------------------------------------------
extern char *sourceFile;
extern int yylineno;


//
// -- Some error and debugging output helper functions (see errors,c)
//    ---------------------------------------------------------------
void Fatal(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Error(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Warning(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Info(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Debug(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...);
void Msg(const char *msg, ...);


//
// -- Keep track of all the register definitions (See register.c)
//    -----------------------------------------------------------
struct reg_t *AddRegister(char *r, int b);
void DumpRegisters(void);


//
// -- Keep track of all the memory ranges (See memory.c)
//    --------------------------------------------------
void AddMemoryRange(uint64_t from, uint64_t to, const char *name);
uint64_t GetStartingMemory(void);
uint64_t GetEndingMemory(void);
void DumpMemory(void);


//
// -- Keep track of all the opcodes available (see opcode.c)
//    ------------------------------------------------------
void AddOpcode(char *def, char *bytes);
char *NormalizeInstruction(char *line);
void ParseInstruction(char *line);
void DumpOpcodes(void);
void AddToEnum(char *def, char *bytes);
void OutputEnum(void);


//
// -- Keep track of the assembled binary (See binary.c)
//    -------------------------------------------------
extern int organization;

void BinaryInit(void);
void SetBigEndian(void);
void SetLittleEndian(void);
void SetPosition(uint64_t p);
uint64_t GetLocation(void);
void SetAddressAt(uint64_t where, uint64_t address);
void EmitByte(uint8_t b);
void EmitInstruction(char *stream, uint64_t optionalArg);
void DumpBinary(uint64_t from, uint64_t to);
void OutputListing(void);
void OutputResults(void);


//
// -- Keep track of labels, forward and backward (see labels.c)
//    ---------------------------------------------------------
void PrintLocationLabels(FILE *fp, uint64_t location);
void NoteLabelLocation(void);
void DumpLabelLocations(void);

#ifdef __cplusplus
}
#endif
