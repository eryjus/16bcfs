//===================================================================================================================
//  listing.cc -- This class is responsible for collecting and printing the listing of the assembly file.
//
//      Copyright (c) 2024-2025  - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  21-May-2024  Initial           Initial Version
//
//===================================================================================================================



#include "asm.hh"



//
// -- Define some class static members
//    --------------------------------
Listing_t::ListingEntry_t *Listing_t::top = nullptr;
Listing_t::ListingEntry_t *Listing_t::last = nullptr;
Listing_t::ListingEntry_t *Listing_t::current = nullptr;
int Listing_t::lineNumber = 1;




//
// -- This is an end of a source line, which should terminate a complete thought for the listing
//    ------------------------------------------------------------------------------------------
void Listing_t::EOL(bool incLine)
{
    if (current == nullptr) current = new ListingEntry_t;

    if (top == nullptr) {
        top = last = current;
    } else {
        last->next = current;
        last = current;
    }

    current = nullptr;

    if (incLine) lineNumber ++;
}



//
// -- format and output the listing
//    -----------------------------
void Listing_t::Output(void)
{
    std::cout << std::endl << "                                Assembly Listing" << std::endl
            << "================================================================================"
            << std::endl << std::endl
            << "LineNo  Addr  Bin   OpCode                   Comment" << std::endl
            << "------  ----  ----  -----------------------  -----------------------------------"
            << std::endl;

    if (top == nullptr) {
        std::cout << "Empty source file" << std::endl;
        return;
    }

    ListingEntry_t *wrk = top;
    while (wrk != nullptr) {
        std::cout << std::setw(6) << std::right << wrk->lineNo;
        std::cout << "  ";

        if (wrk->label != "") {
            std::cout << wrk->label << ':' << std::endl;
        } else {
            std::cout << std::setw(4) << wrk->address;
            std::cout << "  ";
            std::cout << std::setw(4) << wrk->binary;
            std::cout << "  ";

            std::string fullOpCode = wrk->opcode + " " + wrk->op1 + (wrk->op2!=""?",":"") + wrk->op2 + (wrk->op3!=""?",":"") + wrk->op3;
            std::cout << std::setw(23) << std::left << fullOpCode;

            std::cout << "  ";
            std::cout << wrk->comment << std::endl;
        }

        wrk = wrk->next;
    }
}



//
// -- turn a number into a 4-digit hex string for printing
//    ----------------------------------------------------
std::string Listing_t::MakeHex(uint16_t bin)
{
    static char hex[] = "0123456789abcdef";
    std::string val = "";

    val += hex[(bin >> 12 ) & 0x0f];
    val += hex[(bin >>  8 ) & 0x0f];
    val += hex[(bin >>  4 ) & 0x0f];
    val += hex[(bin >>  0 ) & 0x0f];

    return val;
}



//
// -- Add a label to the current listing line
//    ---------------------------------------
void Listing_t::AddLabel(std::string lbl)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->address = MakeHex(Binary_t::GetLoc() - 1);
    current->label = lbl;
}



//
// -- Add a comment to the current listing line
//    -----------------------------------------
void Listing_t::AddComment(std::string cmt)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->comment = cmt;
}



//
// -- Add an opcode to the current listing line
//    -----------------------------------------
void Listing_t::AddOpCode(std::string op, bool addAddr)
{
    if (current == nullptr) current = new ListingEntry_t;

    if (addAddr) current->address = MakeHex(Binary_t::GetLoc() - 1);

    current->opcode.resize(op.size());
    std::transform(op.begin(), op.end(), current->opcode.begin(), ::tolower);
}



//
// -- Add operad1 to the current listing line
//    ---------------------------------------
void Listing_t::AddOp1(std::string op)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->op1 = op;
}



//
// -- Add operad2 to the current listing line
//    ---------------------------------------
void Listing_t::AddOp2(std::string op)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->op2 = op;
}



//
// -- Add operad3 to the current listing line
//    ---------------------------------------
void Listing_t::AddOp3(std::string op)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->op3 = op;
}



//
// -- Add a binary value to the current listing line
//    ----------------------------------------------
void Listing_t::AddBin(uint16_t bin)
{
    if (current == nullptr) current = new ListingEntry_t;

    current->address = MakeHex(Binary_t::GetLoc() - 1);
    current->binary = MakeHex(bin);
}


