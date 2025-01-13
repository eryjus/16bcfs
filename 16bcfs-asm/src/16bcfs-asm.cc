//===================================================================================================================
//  16bcfs-asm.cc -- This source contains the main entry point and several support functions
//
//      Copyright (c) 2024-2025  - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  20-May-2024  Initial           Initial Version
//
//===================================================================================================================



#include "asm.hh"



//
// -- Print the usage and exit
//    ------------------------
void Usage(void)
{
    std::cout << "Usages: 16bcfs-asm [option]" << std::endl;
    std::cout << "        16bcfs-asm <source-file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --help               display this help and exit" << std::endl;
    std::cout << "  -v                   display version information and exit" << std::endl;
    std::cout << "  -h                   create a control-logic header file and exit" << std::endl;

    exit(EXIT_SUCCESS);
}



//
// -- main entry point for the assembler
//    ----------------------------------
int main(int argc, char *argv[])
{
    std::cout << "Welcome to the 16bcfs assembler" << std::endl;

    if (argc != 2) {
        std::cerr << "Assembler requires exactly 1 argument" << std::endl;
        return EXIT_FAILURE;
    }

    std::string option = argv[1];
    if (option == "--help") Usage();
    else if (option == "-v") return EXIT_SUCCESS;
    else if (option == "-h") Parser_t::OutputHeader();


    yyin = fopen(argv[1], "r");
    if (yyin == nullptr) {
        std::cerr << "Error: Unable to open source file " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    Parser_t::Parse();
    Labels_t::CheckUnresolvedLabels();

    if (Parser_t::GetErrorCount() > 0) {
        std::cerr << "Assembly failed: " << Parser_t::GetErrorCount() << " errors; "
                << Parser_t::GetWarningCount() << " warnings" << std::endl;

        return EXIT_FAILURE;
    }

    Binary_t::OutputBinary();
    Listing_t::Output();

    return EXIT_SUCCESS;
}

