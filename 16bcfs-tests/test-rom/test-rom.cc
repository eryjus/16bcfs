//===================================================================================================================
//  test-rom.cc -- Construct a test ROM for Control ROM tests
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  This program builds a test ROM which can be used for testing different signal combinations on the Control
//  ROMs on the Control Logic Mid-Plane
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Sep-09  Initial  v0.0.1   Initial version
//===================================================================================================================


#include <cstdint>
#include <cstdio>
#include <cstdlib>


const int SIZE = 32 * 1024;

uint8_t buffer[SIZE];


int main(void)
{
    for (int i = 0; i < SIZE; i ++) buffer[i] = (i % 256);

    buffer[0] = 0xaa;
    buffer[SIZE - 1] = 0x55;

    FILE *fp = fopen("rom.bin", "wb");
    if (!fp) {
        fprintf(stderr, "ERROR opening 'rom.bin'\n");
        return EXIT_FAILURE;
    }

    fwrite(buffer, SIZE, 1, fp);
    fclose(fp);

    return EXIT_SUCCESS;
}




