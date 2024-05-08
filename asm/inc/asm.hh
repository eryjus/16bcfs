//===================================================================================================================
// asm.hh -- C++ Header file for all modules
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This file will eventually replace the `asm.h` header file as components are converted to C++.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Jun-26  Initial  v0.0.7   Initial Version
//
//===================================================================================================================


#pragma once


#include <map>
#include <string>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>


//
// -- Some definitions which set the limits of the assembler
//    ------------------------------------------------------
#define MAX_REGISTERS   1024
#define MAX_SEARCH_PATH 50
#define MAX_OPERANDS    3


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
    const char *errorMsg;
    uint64_t number;
    char *name;
} yylval_t;


extern yylval_t yylval;


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
    TOK_NAME = 281,
};


//
// -- Used by the scanner to keep track of the current location (See parser.c and lexer.ll)
//    -------------------------------------------------------------------------------------
extern char *sourceFile;
extern int yylineno;



#include "errors.hh"
#include "cond.hh"
#include "memory.hh"
#include "register.hh"
#include "opcode.hh"
#include "binary.hh"
#include "labels.hh"
#include "parser.hh"



