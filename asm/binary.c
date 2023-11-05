//===================================================================================================================
// binary.c -- functions to take care of the assembled binary
//
//  These functions and variables are used to take care of the assembled binary, including its output to a resulting
//  file.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.h"
#include <endian.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>


//
// -- Set the limit on how many bits wide this assembler will support (eventually, only 16-bits for now)
//    --------------------------------------------------------------------------------------------------
#define MAX_BIT_COUNT 64


//
// -- This table (indexed by the machine code) is used to keep track of the human readable instruction.
//    This table is used in outputting the program listing.
//    -------------------------------------------------------------------------------------------------
char *opTable16[MAX_OPCODES] = { 0 };


//
// -- When multiple bytes are used, are they organized in big or little endian
//    ------------------------------------------------------------------------
enum {
    LITTLE = 0,
    BIG = 1,
};


//
// -- Some global variables for managing the resulting assembled binary
//    -----------------------------------------------------------------
uint64_t location = 0;
int organization = 8;
int endian = LITTLE;

int binaryInitialized = 0;
int endianInitialized = 0;
int organizationInitialized = 0;


//
// -- buffer for handling the assembled binary
//    ----------------------------------------
uint8_t *bin;


//
// -- When called, sets multiple bytes to be in big endian order
//    ----------------------------------------------------------
void SetBigEndian(void)
{
    endian = BIG;
}


//
// -- When called, sets multiple bytes to be in little endian order
//    -------------------------------------------------------------
void SetLittleEndian(void)
{
    endian = LITTLE;
}


//
// -- Perform the initialization for the resulting binary and buffer
//    --------------------------------------------------------------
void BinaryInit(void)
{
    bin = malloc((GetEndingMemory() - GetStartingMemory() + 1) * (organization / 8));
    if (!bin) Fatal("Out of memory allocating binary storage", 0, 0, 0, 0);
    memset(bin, 0, ((GetEndingMemory() - GetStartingMemory() + 1) * (organization / 8)));

    location = GetStartingMemory();
}


//
// -- Set the position (as in the org directive) in the binary
//    --------------------------------------------------------
void SetPosition(uint64_t p)
{
    if (p < location) {
        Error("Cannot move backward in the output binary; location is already %lx\n",
                sourceFile,
                yylineno,
                0,
                0,
                location);
    } else if ((p < GetStartingMemory() && GetStartingMemory() != ~0) ||
            (p > GetEndingMemory() && GetEndingMemory() != 0)) {
        Error("`.org` location %lx is is outside memory range of %lx to %lx\n",
                sourceFile,
                yylineno,
                0,
                0,
                p,
                GetStartingMemory(),
                GetEndingMemory());
    } else {
        location = p;
    }
}


//
// -- Get the current location where we are outputting the current binary
//    -------------------------------------------------------------------
uint64_t GetLocation(void)
{
    return location;
}


//
// -- When fixing up a forward-declared label, set the address of the location in the binary
//    --------------------------------------------------------------------------------------
void SetAddressAt(uint64_t where, uint64_t address)
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
// -- Emit a byte to the assembled binary buffer -- even multi-byte assemblers will emit 1 byte at a time
//    ---------------------------------------------------------------------------------------------------
void EmitByte(uint8_t b)
{
    bin[location ++] = b;
}


//
// -- Handle emitting a single instruction into the assembled binary
//    --------------------------------------------------------------
void EmitInstruction(char *stream, uint64_t optionalArg)
{
    if (!binaryInitialized) {
        BinaryInit();
        binaryInitialized = 1;
    }

    for (int i = 0; i < strlen(stream); i ++) {
        if (stream[i] == ' ') continue;

        if (stream[i] == '$' || stream[i + 1] == '(') {
            // -- handle the optional argument here
            i += 2;

            if (stream[i] == '8' && stream[i + 1] == ')') {
                if (organization == 16) {
                    EmitByte(0);
                    EmitByte((uint8_t)(optionalArg & 0xff));
                } else {
                    EmitByte((uint8_t)(optionalArg & 0xff));
                }
    
                i += 2;
                continue;
            }

            if (stream[i] == '1' && stream[i + 1] == '6' && stream[i + 2] == ')') {
                if (organization == 16) {
                    EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                    EmitByte((uint8_t)(optionalArg & 0xff));
                } else if (organization == 8) {
                    if (endian == LITTLE) {
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

            Error("Malformed instruction byte stream constant", 0, 0, 0, 0);
            return;
        }

        if (stream[i] == '%') {
            if (organization == 8) {
                EmitByte((uint8_t)(optionalArg & 0xff));
            } else if (organization == 16) {
                EmitByte((uint8_t)((optionalArg >> 8) & 0xff));
                EmitByte((uint8_t)(optionalArg & 0xff));
            }

            i ++;
            continue;
        }


        uint8_t b = 0;
        int j;

        for (j = 0; j < (organization / 4); j ++) {
            if (!((stream[i + j] >= 'a' && stream[i + j] <= 'f') || (stream[i + j] >= '0' && stream[i + j] <= '9'))) {
                Error("Incorrect hex characcter in byte stream output", 0, 0, 0, 0);
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
            Error("Byte Stream too long for organization size", 0, 0, 0, 0);
        }
    }
}


//
// -- Dump the resulting binary
//    -------------------------
void DumpBinary(uint64_t from, uint64_t to)
{
    Msg("       x0 x1 x2 x3 x4 x5 x6 x7   x8 x9 xa xb xc xd xe xf");
    Msg("       -- -- -- -- -- -- -- --   -- -- -- -- -- -- -- --");
    for (uint64_t i = from; i <= to; i += 16) {
        Msg("%04xx: %02x %02x %02x %02x %02x %02x %02x %02x %c %02x %02x %02x %02x %02x %02x %02x %02x",
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
// -- Output the full binary to a file
//    --------------------------------
void OutputResults(void)
{
    // TODO: need to set output address limits and break into byte-sized files
    FILE *fp = fopen("output.bin", "wb");
    fwrite(bin, GetEndingMemory() - GetStartingMemory() + 1, organization / 8, fp);
    fclose(fp);
}


//
// -- Output a 16-bit word in binary
//    ------------------------------
char *Binary16Bit(uint8_t byte1, uint8_t byte2) {
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
// -- Given a 16-bit machine instruction, go find its human readable form
//    -------------------------------------------------------------------
char *GetOpCode16(uint8_t byte1, uint8_t byte2) {
    char *rv;
    
    if (organization == 8) {
        return "";
    } else if (organization == 16) {
        rv = opTable16[(byte1 << 8) | byte2];
        return (rv?rv:"");
    }
    return "";
}


//
// -- Output a listing of the program
//    -------------------------------
void OutputListing(void)
{
    extern FILE *listingFile;
    int disasmSuppress = 0;

    if (!listingFile) return;

    for (int i = 0; i < location; i += (organization / 8)) {
        if (organization == 8) {

        } else if (organization == 16) {
            PrintLocationLabels(listingFile, i / 2);
            fprintf(listingFile, "%04.4x: %02.2x%02.2x (%-19.19s)", i, bin[i], bin[i + 1],
                        Binary16Bit(bin[i], bin[i + 1]));
            
            if (disasmSuppress) {
                fprintf(listingFile, "\n");
                disasmSuppress = 0;
            } else {
                char *disasm = GetOpCode16(bin[i], bin[i + 1]);
                fprintf(listingFile, ":   %s\n", disasm);
                disasmSuppress = (strchr(disasm, '$') != 0);
            }
        }
    }
}



