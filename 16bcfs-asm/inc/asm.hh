//===================================================================================================================
//  asm.hh -- Definitions for the whole of the assembler
//
//      Copyright (c) 2024-2025 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  17-May-2024  Initial           Initial Version
//
//===================================================================================================================



#pragma once


#include <string>
#include <cstdint>
#include <unistd.h>
#include <map>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>



//
// -- Define the max include file depth
//    ---------------------------------
#define MAX_INCLUDE     25



//
// -- copy the definition from the lexer.cc output file
//    -------------------------------------------------
#ifndef YY_TYPEDEF_YY_BUFFER_STATE
#define YY_TYPEDEF_YY_BUFFER_STATE
typedef struct yy_buffer_state *YY_BUFFER_STATE;
#endif

extern FILE *yyin;


//
// -- some global variable declarations
//    ---------------------------------
extern int includePtr;
extern YY_BUFFER_STATE includeStack[MAX_INCLUDE];



//
// -- This is the lexical token value, returned to the parser
//    -------------------------------------------------------
typedef struct yylval_t {
public:
    yylval_t(void) {}
    ~yylval_t() {}

    std::string name;
    std::string err;
    std::string str;
    uint16_t number;
} yylval_t;



//
// -- declare the token strcture itself
//    ---------------------------------
extern yylval_t yylval;
extern int yylineno;
extern int yylex(void);


//
// -- Here are some token values which might be returned by the scanner
//    -----------------------------------------------------------------
enum {
    TOK_ERR = 256,
    TOK_REG = 257,
    TOK_OPCODE = 258,
    TOK_LABEL = 259,
    TOK_IDENT = 260,
    TOK_STRING = 261,
    TOK_INCBIN = 262,
    TOK_ORG = 263,
    TOK_DATAWORD = 264,
    TOK_NUM = 265,
    TOK_EOL = 266,
};



//
// -- functions in lexer.ll
//    ---------------------
uint16_t MakeBinary(std::string n);
uint16_t MakeOctal(std::string n);
uint16_t MakeDecimal(std::string n);
uint16_t MakeHex(std::string n);



#include "listing.hh"
#include "parser.hh"
#include "binary.hh"
#include "labels.hh"


