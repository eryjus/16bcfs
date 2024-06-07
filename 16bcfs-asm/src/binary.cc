//===================================================================================================================
//  binary.cc -- This is the output binary
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
// -- some static class members
//    -------------------------
int Binary_t::loc = 0;
uint16_t Binary_t::buffer[32*1024] = { 0 };



//
// -- Copy a binary into the output
//    -----------------------------
void Binary_t::EmitBinary(FILE *fp)
{
    uint16_t word;

    while (fread(&word, sizeof(uint16_t), 1, fp) == sizeof(uint16_t)) {
        buffer[loc ++] = word;
    }
}



//
// -- Output the results of the build
//    -------------------------------
void Binary_t::OutputBinary(void)
{
    FILE *msb = fopen("msb.bin", "w");
    FILE *lsb = fopen("lsb.bin", "w");

    if (msb == nullptr || lsb == nullptr) {
        std::cerr << "Error: Unable to open output files";
        return;
    }

    for (int i = 0; i < 32 * 1024; i ++) {
        uint8_t lsByte = (uint8_t)(buffer[i] & 0xff);
        uint8_t msByte = (uint8_t)((buffer[i] >> 8) & 0xff);

        fwrite(&msByte, sizeof(uint8_t), 1, msb);
        fwrite(&lsByte, sizeof(uint8_t), 1, lsb);
    }

    fclose(msb);
    fclose(lsb);
}
