//===================================================================================================================
// binary.hh -- functions to take care of the assembled binary
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of the assembled binary, including its output to a resulting
//  file.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12    #55    v0.0.1   Initial Version
//===================================================================================================================



#pragma once



//
// -- Set the limit on how many bits wide this assembler will support (eventually, only 16-bits for now)
//    --------------------------------------------------------------------------------------------------
#define MAX_BIT_COUNT 64



//
// -- This is an abstract class to deal with a binary
//    -----------------------------------------------
class Binary_t {
    Binary_t(const Binary_t &) = delete;
    Binary_t &operator=(const Binary_t &) = delete;


public:
    enum {
        LITTLE = 0,
        BIG = 1,
    };


protected:
    static Binary_t *singleton;


private:
    static int endian;
    static bool endianSet;
    static bool isInitialized;
    static uint64_t location;
    static uint8_t *bin;


protected:
    Binary_t(void) {}
    ~Binary_t() {}


public:
    static Binary_t *Get(void);
    static void BinaryInit(void);
    static void SetAddressAt(uint64_t where, uint64_t address);
    static void EmitByte(uint8_t b);
    static void DumpBinary(uint64_t from, uint64_t to);
    static void OutputResults(void);
    static void OutputListing(void);
    static char *Binary16Bit(uint8_t byte1, uint8_t byte2);
    static const std::string GetOpCode16(uint8_t byte1, uint8_t byte2);
    static void SetPosition(uint64_t p);
    static void EmitInstruction(const char *stream, uint64_t optionalArg);
    static void SplitOutput(void);

public:
    static bool IsAlloc(void) { return singleton != nullptr; }
    static bool IsInitialized(void) { return IsAlloc() && isInitialized; }
    static uint64_t GetLocation(void) { return location; }
    static void SetLocation(uint64_t l) { location = l; }


public:
    static void SetEndian(int e);


private:
    virtual int _GetOrganization(void) const = 0;
    virtual int _GetEndian(void) const { return endian; }


public:
    static int GetOrganization(void) { return singleton->_GetOrganization(); }
    static int GetEndian(void) { return singleton->_GetEndian(); }
    static bool EndianInitialized(void) { return endianSet; }


public:
};



//
// -- This will be an 8-bit binary
//    ----------------------------
class Binary_8bit_t : public Binary_t {
    Binary_8bit_t(const Binary_8bit_t &) = delete;
    Binary_8bit_t &operator=(const Binary_8bit_t &) = delete;


private:
    Binary_8bit_t(void) {}
    ~Binary_8bit_t() {}


public:
    static Binary_t *Organization(void);


private:
    virtual int _GetOrganization(void) const { return 8; };
};



//
// -- This will be a 16-bit binary
//    ----------------------------
class Binary_16bit_t : public Binary_t {
    Binary_16bit_t(const Binary_16bit_t &) = delete;
    Binary_16bit_t &operator=(const Binary_16bit_t &) = delete;


private:
    Binary_16bit_t(void) {}
    ~Binary_16bit_t() {}


public:
    static Binary_t *Organization(void);


private:
    virtual int _GetOrganization(void) const { return 16; };
};


