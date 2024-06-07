//===================================================================================================================
//  listing.hh -- This class is responsible for collecting and printing the listing of the assembly file.
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  21-May-2024  Initial           Initial Version
//
//===================================================================================================================



#pragma once



class Listing_t {
public:
    typedef struct ListingEntry_t {
        int lineNo;
        std::string address;
        std::string binary;
        std::string label;
        std::string opcode;
        std::string op1;
        std::string op2;
        std::string op3;
        std::string comment;
        struct ListingEntry_t *next;

        ListingEntry_t(void) : lineNo(lineNumber), address(""), binary(""), label(""), opcode(""),
                    op1(""), op2(""), op3(""), comment(""), next(nullptr) {}
    } ListingEntry_t;


private:
    static int lineNumber;
    static ListingEntry_t *top;
    static ListingEntry_t *last;
    static ListingEntry_t *current;


public:
    static void AddLabel(std::string lbl);
    static void AddComment(std::string cmt);
    static void AddOpCode(std::string op, bool addAddr = true);
    static void AddOp1(std::string op);
    static void AddOp2(std::string op);
    static void AddOp3(std::string op);
    static void EOL(bool incLine = true);
    static void IncLine(void) { lineNumber ++; }
    static void Output(void);
    static void AddBin(uint16_t bin);
    static std::string MakeHex(uint16_t bin);
    static ListingEntry_t *GetCurrent(void) { if (current == nullptr) current = new ListingEntry_t; return current; }
};



