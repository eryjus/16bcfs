//===================================================================================================================
//  binary.hh -- This is the output binary
//
//      Copyright (c) 2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  18-May-2024  Initial           Initial Version
//
//===================================================================================================================



#pragma once



//
// -- This class handled the resulting built binary
//    ---------------------------------------------
class Binary_t {
private:
    static uint16_t buffer[32*1024];
    static int loc;


public:
    static void SetOrg(int l) { loc = l; }
    static int GetLoc(void) { return loc; }

    static void Emit(uint16_t dw) { buffer[loc ++] = dw; }
    static void EmitBinary(FILE *fp);
    static void UpdateBinaryLocation(int loc, uint16_t val) { buffer[loc] = val; }
    static void OutputBinary(void);
};


