//===================================================================================================================
// parser.hh -- main entry point and top-down parser (hand-coded)
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This file contains a hand-coded top-down parser for all assembly files.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12    #55    v0.0.1   Initial Version -- This file started from asm.hh
//===================================================================================================================



#pragma once



class Parser_t {
    Parser_t(const Parser_t &) = delete;
    Parser_t &operator=(const Parser_t &) = delete;


private:
    static Parser_t *singleton;
    static FILE *listingFile;
    static int tok;
    static char *sourceFile;


private:
    Parser_t(void) {}
    ~Parser_t() {}


public:
    static Parser_t *Get(void) { if (singleton == nullptr) singleton = new Parser_t; return singleton; }
    static void ParseFile(void);

    static FILE *GetListingFile(void) { return listingFile; }
    static void SetListingFile(FILE *f) { listingFile = f; }
    static char *GetSourceFile(void) { return sourceFile; }
    static void SetSourceFile(char *f) { sourceFile = f; }


protected:
    static void ParseCondSuffix(void);
    static void ParseCondPrefix(void);
    static void ParseCondDefault(void);
    static void ParseCondBits(void);
    static void GetNewLocation(void);
    static void ParseData(void);
    static void ParseOrganization(void);
    static void ParseOpcodeDef(void);
    static char *StandardizeByteStream(void);
    static char *StandardizeOpcodeDef(void);
    static void ParseMemoryDef(void);
    static void ParseRegisterDef(void);
    static void ParseInstruction(const std::string opcode);
};


