//===================================================================================================================
// binary.c -- functions to take care of the assembled binary
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of the assembled binary, including its output to a resulting
//  file.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================



#include "asm.hh"



//
// -- This table (indexed by the machine code) is used to keep track of the human readable instruction.
//    This table is used in outputting the program listing.
//    -------------------------------------------------------------------------------------------------
char *opTable16[MAX_OPCODES] = { 0 };



//
// -- Some static members
//    -------------------
Binary_t *Binary_t::singleton = nullptr;
uint8_t *Binary_t::bin = nullptr;
int Binary_t::endian = Binary_t::LITTLE;
bool Binary_t::endianSet = false;
bool Binary_t::isInitialized = false;
uint64_t Binary_t::location = 0;



//
// -- The abstract class to get the binary singleton object -- fatal if not initialized
//    ---------------------------------------------------------------------------------
Binary_t *Binary_t::Get(void)
{
    if (!singleton) Messaging::Fatal("Binary object not initialized!", Parser_t::GetSourceFile(), yylineno, "", 0);
    return singleton;
}



//
// -- set the endianness for the binary
//    ---------------------------------
void Binary_t::SetEndian(int e)
{
    if (endianSet) Messaging::Error("Endianness has already been established", "", 0, "", 0);
    endian = e;
    endianSet = true;
}



//
// -- Perform the initialization for the resulting binary and buffer
//    --------------------------------------------------------------
void Binary_t::BinaryInit(void)
{
    if (!IsAlloc()) Binary_8bit_t::Organization();

    bin = new uint8_t[(Memory::GetEndingMemory() - Memory::GetStartingMemory() + 1) * (GetOrganization() / 8)];
    if (!bin) Messaging::Fatal("Out of memory allocating binary storage", 0, 0, 0, 0);
    memset(bin, 0, ((Memory::GetEndingMemory() - Memory::GetStartingMemory() + 1) * (GetOrganization() / 8)));

    location = Memory::GetStartingMemory();
    isInitialized = true;
}



//
// -- When fixing up a forward-declared label, set the address of the location in the binary
//    --------------------------------------------------------------------------------------
void Binary_t::SetAddressAt(uint64_t where, uint64_t address)
{
    // -- TODO: get an address width and check the number of bytes to update
    // -- TODO: determine the endian-ness of the architecture and handle it here

    // -- for now, assume 16-bit addresses and little endian for all
    uint8_t lo = address & 0xff;
    uint8_t hi = (address >> 8) & 0xff;

    bin[where] = lo;
    bin[where + 1] = hi;
}



//
// -- Set the position (as in the org directive) in the binary
//    --------------------------------------------------------
void Binary_t::SetPosition(uint64_t p)
{
    if (p < GetLocation()) {
        Messaging::Error("Cannot move backward in the output binary; location is already %lx\n",
                Parser_t::GetSourceFile(),
                yylineno,
                0,
                0,
                GetLocation());
    } else if ((p < Memory::GetStartingMemory() && Memory::GetStartingMemory() != ~0) ||
            (p > Memory::GetEndingMemory() && Memory::GetEndingMemory() != 0)) {
        Messaging::Error("`.org` location %lx is is outside memory range of %lx to %lx\n",
                Parser_t::GetSourceFile(),
                yylineno,
                0,
                0,
                p,
                Memory::GetStartingMemory(),
                Memory::GetEndingMemory());
    } else {
        SetLocation(p);
    }
}



//
// -- Emit a byte to the assembled binary buffer -- even multi-byte assemblers will emit 1 byte at a time
//    ---------------------------------------------------------------------------------------------------
void Binary_t::EmitByte(uint8_t b)
{
    uint64_t loc = GetLocation();

    bin[loc] = b;
    SetLocation(loc + 1);
}



//
// -- Handle emitting a single instruction into the assembled binary
//    --------------------------------------------------------------
void Binary_t::EmitInstruction(const char *stream, uint64_t optionalArg)
{
    if (!IsInitialized()) {
        BinaryInit();
    }

    for (int i = 0; i < strlen(stream); i ++) {
        if (stream[i] == ' ') continue;

        if (stream[i] == '$' || stream[i + 1] == '(') {
            // -- handle the optional argument here
            i += 2;

            if (stream[i] == '8' && stream[i + 1] == ')') {
                if (GetOrganization() == 16) {
                    EmitByte(0);
                    EmitByte((uint8_t)(optionalArg & 0xff));
                } else {
                    EmitByte((uint8_t)(optionalArg & 0xff));
                }

                i += 2;
                continue;
            }

            if (stream[i] == '1' && stream[i + 1] == '6' && stream[i + 2] == ')') {
                if (GetOrganization() == 16) {
                    EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                    EmitByte((uint8_t)(optionalArg & 0xff));
                } else if (GetOrganization() == 8) {
                    if (GetEndian() == LITTLE) {
                        EmitByte((uint8_t)(optionalArg & 0xff));
                        EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                    } else {
                        EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                        EmitByte((uint8_t)(optionalArg & 0xff));
                    }
                }

                i += 3;
                continue;
            }

            Messaging::Error("Malformed instruction byte stream constant", 0, 0, 0, 0);
            return;
        }

        if (stream[i] == '%') {
            if (GetOrganization() == 8) {
                EmitByte((uint8_t)(optionalArg & 0xff));
            } else if (GetOrganization() == 16) {
                EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                EmitByte((uint8_t)(optionalArg & 0xff));
            }

            i ++;
            continue;
        }


        uint8_t b = 0;
        int j;

        for (j = 0; j < (GetOrganization() / 4); j ++) {
            if (!((stream[i + j] >= 'a' && stream[i + j] <= 'f') || (stream[i + j] >= '0' && stream[i + j] <= '9'))) {
                Messaging::Error("Incorrect hex characcter in byte stream output", 0, 0, 0, 0);
            }

            if (stream[i + j] >= 'a' && stream[i + j] <= 'f') {
                b = (b * 16) + stream[i + j] - 'a' + 10;
            }

            if (stream[i + j] >= '0' && stream[i + j] <= '9') {
                b = (b * 16) + stream[i + j] - '0';
            }

            if (j & 1) {
                EmitByte(b);
                b = 0;
            }
        }

        i += j;

        if (stream[i] != ' ' && stream[i] != 0) {
            Messaging::Error("Byte Stream too long for organization size", 0, 0, 0, 0);
        }
    }
}


//
// -- Output a 16-bit word in binary
//    ------------------------------
char *Binary_t::Binary16Bit(uint8_t byte1, uint8_t byte2)
{
    static char buf[MAX_BIT_COUNT + (MAX_BIT_COUNT / 4) + 1];
    char *pos = buf;

    memset(buf, 0, sizeof(buf));

    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        *(pos ++) = (byte1 & mask ? '1' : '0');
        if (mask == 0x10) *(pos ++) = '.';
    }

    *(pos ++) = '.';

    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        *(pos ++) = (byte2 & mask ? '1' : '0');
        if (mask == 0x10) *(pos ++) = '.';
    }

    return buf;
}



//
// -- Output the full binary to a file
//    --------------------------------
void Binary_t::OutputResults(void)
{
    // TODO: need to set output address limits and break into byte-sized files
    FILE *fp = fopen("output.bin", "wb");
    fwrite(bin, Memory::GetEndingMemory() - Memory::GetStartingMemory() + 1, GetOrganization() / 8, fp);
    fclose(fp);
}



//
// -- Dump the resulting binary
//    -------------------------
void Binary_t::DumpBinary(uint64_t from, uint64_t to)
{
    Messaging::Msg("       x0 x1 x2 x3 x4 x5 x6 x7   x8 x9 xa xb xc xd xe xf");
    Messaging::Msg("       -- -- -- -- -- -- -- --   -- -- -- -- -- -- -- --");
    for (uint64_t i = from; i <= to; i += 16) {
        Messaging::Msg("%04xx: %02x %02x %02x %02x %02x %02x %02x %02x %c %02x %02x %02x %02x %02x %02x %02x %02x",
            i >> 4,
            bin[i +  0],
            bin[i +  1],
            bin[i +  2],
            bin[i +  3],
            bin[i +  4],
            bin[i +  5],
            bin[i +  6],
            bin[i +  7],
            ' ',
            bin[i +  8],
            bin[i +  9],
            bin[i + 10],
            bin[i + 11],
            bin[i + 12],
            bin[i + 13],
            bin[i + 14],
            bin[i + 15]);
    }
}



//
// -- Given a 16-bit machine instruction, go find its human readable form
//    -------------------------------------------------------------------
const char *Binary_t::GetOpCode16(uint8_t byte1, uint8_t byte2)
{
    char *rv;

    if (GetOrganization() == 8) {
        return "";
    } else if (GetOrganization() == 16) {
        rv = opTable16[(byte1 << 8) | byte2];
        return (rv?rv:"");
    }
    return "";
}



//
// -- Output a listing of the program
//    -------------------------------
void Binary_t::OutputListing(void)
{
    int disasmSuppress = 0;

    if (!Parser_t::GetListingFile()) return;

    for (int i = 0; i < GetLocation(); i += (GetOrganization() / 8)) {
        if (GetOrganization() == 8) {

        } else if (GetOrganization() == 16) {
            PrintLocationLabels(Parser_t::GetListingFile(), i / 2);
            fprintf(Parser_t::GetListingFile(), "%04.4x: %02.2x%02.2x (%-19.19s)", i, bin[i], bin[i + 1],
                        Binary16Bit(bin[i], bin[i + 1]));

            if (disasmSuppress) {
                fprintf(Parser_t::GetListingFile(), "\n");
                disasmSuppress = 0;
            } else {
                const char *disasm = GetOpCode16(bin[i], bin[i + 1]);
                fprintf(Parser_t::GetListingFile(), ":   %s\n", disasm);
                disasmSuppress = (strchr(disasm, '$') != 0);
            }
        }
    }
}



//
// -- simple hack to divide the EEPROM -- hard coded!!
//    ------------------------------------------------
void Binary_t::SplitOutput(void)
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



//===================================================================================================================



//
// -- Set the organization for the binary and initialize the singleton
//    ----------------------------------------------------------------
Binary_t *Binary_8bit_t::Organization(void)
{
    if (singleton) Messaging::Fatal("Unable to reinitialize binary :organization", Parser_t::GetSourceFile(), yylineno, "", 0);
    singleton = new Binary_8bit_t;
    return singleton;
}



//===================================================================================================================



//
// -- Set the organization for the binary and initialize the singleton
//    ----------------------------------------------------------------
Binary_t *Binary_16bit_t::Organization(void) {
    if (singleton) Messaging::Fatal("Unable to reinitialize binary :organization", Parser_t::GetSourceFile(), yylineno, "", 0);
    singleton = new Binary_16bit_t;
    return singleton;
}




