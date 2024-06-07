//===================================================================================================================
//  parser.cc -- This is the parser for the assembler
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  18-May-2024  Initial           Initial Version
//
//===================================================================================================================



#include "asm.hh"


//
// -- Some global variables used by the scanner and parser
//    ----------------------------------------------------
yylval_t yylval;
int Parser_t::tok;
int Parser_t::errors;
int Parser_t::warnings;
Parser_t::AssemblyTable_t *Parser_t::opcodeStart[26] = { nullptr };



//
// -- Initialize the parser structures
//    --------------------------------
void Parser_t::Initialize(void)
{
    AssemblyTable_t *entry;
    int i = 0;

    entry = &table[i];

    while (entry->mnemonic != "zzz") {
        char f = entry->mnemonic[0];

        if (opcodeStart[f - 'a'] == nullptr) opcodeStart[f - 'a'] = entry;

        entry = &table[++ i];
    }
}


//
// -- Start parsing a source file
//    ---------------------------
void Parser_t::Parse(void)
{
    Initialize();
    ADVANCE_TOKEN();        // -- fetch the first token

    while (true) {
        switch(CURRENT_TOKEN()) {
            case 0: return;

            case TOK_EOL: {
                Listing_t::EOL();
                ADVANCE_TOKEN();
                continue;
            }

            case TOK_ORG: {
                ParseOrgDirective();
                continue;
            }

            case TOK_INCBIN: {
                ParseIncBinDirective();
                continue;
            }

            case TOK_DATAWORD: {
                ParseDataWordDirective();
                continue;
            }

            case TOK_LABEL:
            case TOK_OPCODE: {
                ParseInstructionLine();
                continue;
            }

            case TOK_ERR: {
                std::cerr << "Error: On line " << yylineno << ": " << yylval.err << std::endl;
                errors ++;
                RECOVERY();
                continue;
            }

            case TOK_IDENT: {
                std::cerr << "Invalid opcode on line " << yylineno << ": `" << yylval.name << '`' << std::endl;
                errors ++;
                RECOVERY();
                continue;
            }

            default: {
                UNIMPLEMENTED();
                errors ++;
                RECOVERY();
                continue;
            }
        }
    }
}



//
// -- Parse an Org Directive
//
//     .org 0x100 <eol>
//    ----------------------
void Parser_t::ParseOrgDirective(void)
{
    ADVANCE_TOKEN();            // '.org' matched already

    if (!MATCH(TOK_NUM)) {
        std::cerr << "Error: `.org` directive requires a number to follow the directive on line "
                << yylineno << std::endl;

        errors ++;

        RECOVERY();
        return;
    }

    int l = yylval.number;
    ADVANCE_TOKEN();

    if (!MATCH(TOK_EOL)) {
        std::cerr << "Error: `.org` directive contains too many tokens on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    Binary_t::SetOrg(l);
    Listing_t::AddOpCode(".org", false);
    Listing_t::AddOp1(Listing_t::MakeHex(l));

    Listing_t::EOL();
    ADVANCE_TOKEN();

    if (l < Binary_t::GetLoc()) {
        std::cerr << "Warning: `.org` directive moves backwards in assembled binary on line "
                << yylineno << std::endl;
        warnings ++;
    }

}



//
// -- Parse a line with a data word directive on it
//
//     .dw 0x100 (, 0x101)* <eol>
//    ---------------------------------------------
void Parser_t::ParseDataWordDirective(void)
{
    ADVANCE_TOKEN();

    if (!MATCH(TOK_NUM)) {
        std::cerr << "Error: `.dw` directive requires at least 1 word on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    Binary_t::Emit(yylval.number);
    ADVANCE_TOKEN();

    while (!MATCH(TOK_EOL)) {
        switch (CURRENT_TOKEN()) {
            case ',': {
                ADVANCE_TOKEN();
                continue;
            }

            case TOK_NUM: {
                Binary_t::Emit(yylval.number);
                ADVANCE_TOKEN();
                continue;
            }

            case TOK_IDENT: {
                std::cerr << "Error: identifiers in `.dw.` directives is not yet implemented on line "
                        << yylineno << std::endl;
                errors ++;

                RECOVERY();
                return;
            }

            default: {
                std::cerr << "Error: Unknown token in `.dw` directive on line " << yylineno << std::endl;
                errors ++;

                RECOVERY();
                return;
            }
        }
    }
}



//
// -- Include a binary file directly into the output
//
//     %incbin file.ext <eol>
//    ----------------------------------------------
void Parser_t::ParseIncBinDirective(void)
{
    ADVANCE_TOKEN();

    if (!MATCH(TOK_IDENT)) {
        std::cerr << "Error: `%incbin` directive requires a file name on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    FILE *fp = fopen(yylval.name.c_str(), "rb");
    if (fp == nullptr) {
        std::cerr << "Error: Unable to open file name " << yylval.name << " for `%incbin` directive on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    ADVANCE_TOKEN();

    if (!MATCH(TOK_EOL)) {
        std::cerr << "Error: `%incbin` directive requires only 1 file name on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        fclose(fp);
        return;
    }

    Binary_t::EmitBinary(fp);
    fclose(fp);
    ADVANCE_TOKEN();
}



//
// -- Parse an instruction line
//
//    [ <label>: ] <opcode> [ <arg>  [ , <arg> ] ]
//    --------------------------------------------
void Parser_t::ParseInstructionLine(void)
{
    bool isLabel = false;

    if (MATCH(TOK_LABEL)) {
        Labels_t::AddNewLabel(yylval.name, Binary_t::GetLoc());
        ADVANCE_TOKEN();

        if (MATCH(TOK_EOL)) {
            Listing_t::EOL();
            ADVANCE_TOKEN();
            return;
        }
    }

    if (!MATCH(TOK_OPCODE)) {
        std::cerr << "Error: An opcode is required on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    std::string opcode = yylval.name;
    int operandCnt = 0;
    std::string operands[3] = {""};
    uint16_t constant;
    int constPos = -1;
    Listing_t::AddOpCode(opcode);

    ADVANCE_TOKEN();            // -- advance past the opcode


    // -- now er need to parse any optional operands until we reach the EOL
    while (!MATCH(TOK_EOL) && !MATCH(0)) {
        switch (CURRENT_TOKEN()) {
            case TOK_NUM: {
                if (constPos != -1) {
                    std::cerr << "Error: Only one constant is allowed for an opcode on line " << yylineno << std::endl;
                    errors ++;

                    RECOVERY();
                    return;
                }

                constPos = operandCnt;
                constant = yylval.number;
                operands[operandCnt] = Listing_t::MakeHex(constant);
                isLabel = false;

                ADVANCE_TOKEN();
                operandCnt ++;

                break;
            }

            case TOK_REG: {
                operands[operandCnt].resize(yylval.name.size());
                std::transform(yylval.name.begin(), yylval.name.end(), operands[operandCnt].begin(), ::tolower);
                ADVANCE_TOKEN();
                operandCnt ++;

                break;
            }

            case TOK_IDENT: {
                if (constPos != -1) {
                    std::cerr << "Error: Only one constant is allowed for an opcode on line " << yylineno << std::endl;
                    errors ++;

                    RECOVERY();
                    return;
                }

                constPos = operandCnt;

                if (yylval.name[0] == '.') {
                    operands[operandCnt] = Labels_t::GetMainLabel() + yylval.name;
                } else {
                    operands[operandCnt] = yylval.name;
                }

                constant = Labels_t::GetLabelLocation(yylval.name, Binary_t::GetLoc() + 1);
                isLabel = true;

                ADVANCE_TOKEN();
                operandCnt ++;

                break;
            }

            case 0: {
                std::cerr << "Error: Unexpected EOF on line " << yylineno << " (did you forget a final linefeed?)" << std::endl;
                errors ++;

                return;
            }

            TOK_ERR: {
                std::cerr << yylval.err << std::endl;
            }

            default: {
                std::cerr << "Error: Invalid argument for opcode " << opcode << " on line " << yylineno << std::endl;
                errors ++;

                RECOVERY();
                return;
            }
        }

        if (MATCH(',')) ADVANCE_TOKEN();
        else break;
    }


    if (operandCnt > 3) {
        std::cerr << "Error: Too many operands on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }

    if (!MATCH(TOK_EOL) && !MATCH(0)) {
        std::cerr << "Error: Too many tokens on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }


    // -- add the operands to the listing
    Listing_t::AddOp1(operands[0]);
    Listing_t::AddOp2(operands[1]);
    Listing_t::AddOp3(operands[2]);



    // -- need to determine which version of the opcode to find
    AssemblyTable_t *entry;
    entry = Parser_t::FindExactOpcode(opcode, operandCnt, operands[0], operands[1], operands[2], constPos + 1);

    if (entry == nullptr) {
        std::cerr << "Error: Unknown instruction " << opcode << " on line " << yylineno << std::endl;
        errors ++;

        RECOVERY();
        return;
    }


    for (int i = 0; i < entry->binCount; i ++) {
        if (((entry->binMask >> (7 - i)) & BIN_CONST) != 0) {
            Binary_t::Emit(constant);
            Listing_t::AddBin(constant);
        } else {
            switch (i) {
                case 0: {
                    Binary_t::Emit(entry->b1Val);
                    Listing_t::AddBin(entry->b1Val);
                    break;
                }

                case 1: {
                    Binary_t::Emit(entry->b2Val);
                    Listing_t::AddBin(entry->b2Val);
                    break;
                }

                case 2: {
                    Binary_t::Emit(entry->b3Val);
                    Listing_t::AddBin(entry->b3Val);
                    break;
                }

                case 3: {
                    Binary_t::Emit(entry->b4Val);
                    Listing_t::AddBin(entry->b4Val);
                    break;
                }

                case 4: {
                    Binary_t::Emit(entry->b5Val);
                    Listing_t::AddBin(entry->b5Val);
                    break;
                }

                case 5: {
                    Binary_t::Emit(entry->b6Val);
                    Listing_t::AddBin(entry->b6Val);
                    break;
                }

                case 6: {
                    Binary_t::Emit(entry->b7Val);
                    Listing_t::AddBin(entry->b7Val);
                    break;
                }

                case 7: {
                    Binary_t::Emit(entry->b8Val);
                    Listing_t::AddBin(entry->b8Val);
                    break;
                }
            }
        }

        Listing_t::EOL(false);
    }

    // -- move past the EOL
    if (!MATCH(0)) ADVANCE_TOKEN();
    Listing_t::IncLine();
}



//
// -- Search the asspmbly tables for a match for the opcode scanned
//    -------------------------------------------------------------
Parser_t::AssemblyTable_t *Parser_t::FindExactOpcode(std::string _opcode,
        int opCnt, std::string op1, std::string op2, std::string op3, int constPos)
{
    AssemblyTable_t *wrk = nullptr;
    std::string opcode;
    opcode.resize(_opcode.size());

    std::transform(_opcode.begin(), _opcode.end(), opcode.begin(), ::tolower);
    wrk = opcodeStart[opcode[0] - 'a'];

    if (wrk == nullptr) return nullptr;

    while (wrk != nullptr && wrk->mnemonic[0] == opcode[0] && wrk->mnemonic != "zzz") {
        // -- do we have a matching mnemonic and argument count?
        if (opcode == wrk->mnemonic && opCnt == wrk->argCount) {
            // -- if no arguments, we have a match
            if (opCnt == 0) return wrk;

            // -- check operand1
            if (opCnt >= 1) {
                if ((wrk->argMask & 0x80) != 0 && wrk->a1Name == op1) {
                    // -- do nothing: good so far!
                } else if ((wrk->argMask & 0x80) == 0 && constPos == 1) {
                    // -- do nothing; good so far!
                } else {
                    // -- nope, not this one
                    wrk ++;
                    continue;
                }
            }

            // -- check operand2
            if (opCnt >= 2) {
                if ((wrk->argMask & 0x40) != 0 && wrk->a2Name == op2) {
                    // -- do nothing: good so far!
                } else if ((wrk->argMask & 0x40) == 0 && constPos == 2) {
                    // -- do nothing; good so far!
                } else {
                    // -- nope, not this one
                    wrk ++;
                    continue;
                }
            }

            // -- check operand3
            if (opCnt >= 3) {
                if ((wrk->argMask & 0x20) != 0 && wrk->a3Name == op3) {
                    // -- do nothing: good so far!
                } else if ((wrk->argMask & 0x20) == 0 && constPos == 3) {
                    // -- do nothing; good so far!
                } else {
                    // -- nope, not this one
                    wrk ++;
                    continue;
                }
            }

            // -- if we make it here, we have a good match!
            return wrk;
        }

        // -- should never make it here, but be complete
        wrk ++;
    }


    return nullptr;
}



//
// -- Output the header (enum) for the control logic
//    ----------------------------------------------
void Parser_t::OutputHeader(void)
{
    static std::string preamble =
        "//===================================================================================================================\n"
        "// opcode.h -- This file is generated by the assembler.  Do not modify!\n"
        "//===================================================================================================================\n"
        "\n"
        "\n"
        "enum {\n";

    FILE *fp = fopen("opcodes.h", "w");

    if (!fp) {
        std::cerr << "Unable to open opcodes.h" << std::endl;
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s", preamble.c_str());

    for (AssemblyTable_t *entry = table; entry->mnemonic != "zzz"; entry ++) {
        if (entry->mnemonic.find('-') == std::string::npos) {
            std::string opcode;
            std::string op1;
            std::string op2;
            std::string op3;
            std::string val = "OPCODE_";

            opcode.resize(entry->mnemonic.size());
            op1.resize(entry->a1Name.size());
            op2.resize(entry->a2Name.size());
            op3.resize(entry->a3Name.size());

            std::transform(entry->mnemonic.begin(), entry->mnemonic.end(), opcode.begin(), ::toupper);

            val += opcode;

            if (entry->argCount >= 1) {
                std::transform(entry->a1Name.begin(), entry->a1Name.end(), op1.begin(), ::toupper);
                val += '_';

                if ((entry->argMask & 0x80) != 0) {
                    val += op1;
                } else {
                    val += "IMM";
                }
            }

            if (entry->argCount >= 2) {
                std::transform(entry->a2Name.begin(), entry->a2Name.end(), op2.begin(), ::toupper);
                val += '_';

                if ((entry->argMask & 0x40) != 0) {
                    val += op2;
                } else {
                    val += "IMM";
                }
            }

            if (entry->argCount >= 3) {
                std::transform(entry->a3Name.begin(), entry->a3Name.end(), op3.begin(), ::toupper);
                val += '_';

                if ((entry->argMask & 0x20) != 0) {
                    val += op3;
                } else {
                    val += "IMM";
                }
            }

            val += " = 0x";
            val += Listing_t::MakeHex(entry->b1Val);
            val += ",\n";

            fprintf(fp, "\t%s", val.c_str());
        }
    }

    fprintf(fp, "};\n\n");
    fclose(fp);

    exit(EXIT_SUCCESS);
}