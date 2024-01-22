//===================================================================================================================
//  control.cc -- Generate the control logic for the 16-Bit Computer From Scratch
//
//  This file will generate the control logic for the 16-Bit Computer From Scratch.  This is intended to be a
//  temporary solution for the breadboard incarnation.  When we get to moving this to PCB, a different solution
//  will be used (as in, not EEPROM).
//
//  -----------------------------------------------------------------------------------------------------------------
//
//     Date      Tracker  Version  Pgmr  Description
//  -----------  -------  -------  ----  ---------------------------------------------------------------------------
//  2023-Feb-24  Initial  v0.0.1   ADCL  Initial version
//  2023-Mar-02  Initial  v0.0.2   ADCL  Add a NOP instruction
//  2023-Mar-11  Initial  v0.0.3   ADCL  Expand the Control Logic to 24 control signals & add R1 controls
//  2023-Mar-19  Initial  v0.0.4   ADCL  Add support for the `MOV R1,<imm16>` instruction
//  2023-Mar-21  Initial  v0.0.5   ADCL  Bug fixes
//  2023-Mar-29  Initial  v0.0.6   ADCL  Add support for the `JMP <imm16>` instruction
//  2023-May-13  Initial  v0.0.7   ADCL  Add support for the `CLC` and `STC` instructions
//  2023-May-22  Initial  v0.0.8   ADCL  Expand the instr to 12 bits; eliminate the direct-wired main bus assert
//
//===================================================================================================================


#include <cstdint>
#include <stdio.h>
#include <cstring>


typedef __uint128_t uint128_t;

//
// -- These are the different flags which will change what an instruction will actually do
//    ------------------------------------------------------------------------------------
enum {
    FLAG_CONDITION          = 0b100ul,        // The contition was not met
};


//
// -- These are the different control signals which can be enabled on the PROM
//    ------------------------------------------------------------------------
enum : uint128_t {
    //
    // == CTRL1
    //    =====

    // bits 7:6 -- assert to Address Bus 1
    ADDR_BUS_1_ASSERT_PC    = (((uint128_t)0b00ul)      << 6) << 0,
    ADDR_BUS_1_ASSERT_RA    = (((uint128_t)0b01ul)      << 6) << 0,
    ADDR_BUS_1_ASSERT_INTPC = (((uint128_t)0b10ul)      << 6) << 0,
    ADDR_BUS_1_ASSERT_INTRA = (((uint128_t)0b11ul)      << 6) << 0,

    // bit 5:0 -- assert to Main Bus
    MAIN_NONE               = (((uint128_t)0b000000ul)  << 0) << 0,
    MAIN_R1                 = (((uint128_t)0b000001ul)  << 0) << 0,
    MAIN_R2                 = (((uint128_t)0b000010ul)  << 0) << 0,
    MAIN_R3                 = (((uint128_t)0b000011ul)  << 0) << 0,
    MAIN_R4                 = (((uint128_t)0b000100ul)  << 0) << 0,
    MAIN_R5                 = (((uint128_t)0b000101ul)  << 0) << 0,
    MAIN_R6                 = (((uint128_t)0b000110ul)  << 0) << 0,
    MAIN_R7                 = (((uint128_t)0b000111ul)  << 0) << 0,
    MAIN_R8                 = (((uint128_t)0b001000ul)  << 0) << 0,
    MAIN_R9                 = (((uint128_t)0b001001ul)  << 0) << 0,
    MAIN_R10                = (((uint128_t)0b001010ul)  << 0) << 0,
    MAIN_R11                = (((uint128_t)0b001011ul)  << 0) << 0,
    MAIN_R12                = (((uint128_t)0b001100ul)  << 0) << 0,
    MAIN_SP                 = (((uint128_t)0b001101ul)  << 0) << 0,
    MAIN_RA                 = (((uint128_t)0b001110ul)  << 0) << 0,
    MAIN_PC                 = (((uint128_t)0b001111ul)  << 0) << 0,
    MAIN_ISP                = (((uint128_t)0b010000ul)  << 0) << 0,
    MAIN_IRA                = (((uint128_t)0b010001ul)  << 0) << 0,
    MAIN_IPC                = (((uint128_t)0b010010ul)  << 0) << 0,
    MAIN_FETCH              = (((uint128_t)0b010011ul)  << 0) << 0,
    MAIN_DEV1               = (((uint128_t)0b010100ul)  << 0) << 0,
    MAIN_DEV2               = (((uint128_t)0b010101ul)  << 0) << 0,
    MAIN_DEV3               = (((uint128_t)0b010110ul)  << 0) << 0,
    MAIN_DEV4               = (((uint128_t)0b010111ul)  << 0) << 0,
    MAIN_DEV5               = (((uint128_t)0b011000ul)  << 0) << 0,
    MAIN_DEV6               = (((uint128_t)0b011001ul)  << 0) << 0,
    MAIN_DEV7               = (((uint128_t)0b011010ul)  << 0) << 0,
    MAIN_DEV8               = (((uint128_t)0b011011ul)  << 0) << 0,
    MAIN_DEV9               = (((uint128_t)0b011100ul)  << 0) << 0,
    MAIN_DEV10              = (((uint128_t)0b011101ul)  << 0) << 0,
    MAIN_ALU_RESULT         = (((uint128_t)0b011110ul)  << 0) << 0,
    MAIN_MEMORY             = (((uint128_t)0b011111ul)  << 0) << 0,

    MAIN_CTL1               = (((uint128_t)0b100100ul)  << 0) << 0,
    MAIN_CTL2               = (((uint128_t)0b100101ul)  << 0) << 0,
    MAIN_CTL3               = (((uint128_t)0b100110ul)  << 0) << 0,
    MAIN_CTL4               = (((uint128_t)0b100111ul)  << 0) << 0,
    MAIN_CTL5               = (((uint128_t)0b101000ul)  << 0) << 0,
    MAIN_CTL6               = (((uint128_t)0b101001ul)  << 0) << 0,
    MAIN_CTL7               = (((uint128_t)0b101010ul)  << 0) << 0,
    MAIN_CTL8               = (((uint128_t)0b101011ul)  << 0) << 0,
    MAIN_CTL9               = (((uint128_t)0b101100ul)  << 0) << 0,
    MAIN_CTL10              = (((uint128_t)0b101101ul)  << 0) << 0,


    //---------------------------------------------------

    //
    // == CTRL2
    //    =====

    // bits 7:6 -- PC Load/Inc/Dec
    PC_DO_NOTHING           = (((uint128_t)0b00ul)      << 6) << 8,
    PC_LOAD                 = (((uint128_t)0b01ul)      << 6) << 8,
    PC_INC                  = (((uint128_t)0b10ul)      << 6) << 8,
    PC_DEC                  = (((uint128_t)0b11ul)      << 6) << 8,

    // bits 5:4 -- RA Load/Inc/Dec
    RA_DO_NOTHING           = (((uint128_t)0b00ul)      << 4) << 8,
    RA_LOAD                 = (((uint128_t)0b01ul)      << 4) << 8,
    RA_INC                  = (((uint128_t)0b10ul)      << 4) << 8,
    RA_DEC                  = (((uint128_t)0b11ul)      << 4) << 8,

    // bits 3:2 -- SP Load/Inc/Dec
    SP_DO_NOTHING           = (((uint128_t)0b00ul)      << 2) << 8,
    SP_LOAD                 = (((uint128_t)0b01ul)      << 2) << 8,
    SP_INC                  = (((uint128_t)0b10ul)      << 2) << 8,
    SP_DEC                  = (((uint128_t)0b11ul)      << 2) << 8,

    // bits 1:0 -- INT-PC Load/Inc/Dec
    INT_PC_DO_NOTHING       = (((uint128_t)0b00ul)      << 0) << 8,
    INT_PC_LOAD             = (((uint128_t)0b01ul)      << 0) << 8,
    INT_PC_INC              = (((uint128_t)0b10ul)      << 0) << 8,
    INT_PC_DEC              = (((uint128_t)0b11ul)      << 0) << 8,


    //---------------------------------------------------

    //
    // == CTRL3
    //    =====

    // bits 7:6 -- INT-RA Load/Inc/Dec
    INT_RA_DO_NOTHING       = (((uint128_t)0b00ul)      << 6) << 16,
    INT_RA_LOAD             = (((uint128_t)0b01ul)      << 6) << 16,
    INT_RA_INC              = (((uint128_t)0b10ul)      << 6) << 16,
    INT_RA_DEC              = (((uint128_t)0b11ul)      << 6) << 16,

    // bits 5:4 -- INT-SP Load/Inc/Dec
    INT_SP_DO_NOTHING       = (((uint128_t)0b00ul)      << 4) << 16,
    INT_SP_LOAD             = (((uint128_t)0b01ul)      << 4) << 16,
    INT_SP_INC              = (((uint128_t)0b10ul)      << 4) << 16,
    INT_SP_DEC              = (((uint128_t)0b11ul)      << 4) << 16,

    // bit 3 -- Memory Write
    MEMORY_NOTHING          = (((uint128_t)0b0ul)       << 3) << 16,
    MEMORY_WRITE            = (((uint128_t)0b1ul)       << 3) << 16,

    // bit 2 -- Fetch Assert to Instruction
    INSTRUCTION_ASSERT      = (((uint128_t)0b0ul)       << 2) << 16,
    INSTRUCTION_SUPPRESS    = (((uint128_t)0b1ul)       << 2) << 16,

    // bit 1 -- R1 Load
    R1_DO_NOTHING           = (((uint128_t)0b0ul)       << 1) << 16,
    R1_LOAD                 = (((uint128_t)0b1ul)       << 1) << 16,

    // bit 0 -- R2 Load
    R2_DO_NOTHING           = (((uint128_t)0b0ul)       << 0) << 16,
    R2_LOAD                 = (((uint128_t)0b1ul)       << 0) << 16,


    //---------------------------------------------------

    //
    // == CTRL4
    //    =====

    // bit 7 -- R3 Load
    R3_DO_NOTHING           = (((uint128_t)0b0ul)       << 7) << 24,
    R3_LOAD                 = (((uint128_t)0b1ul)       << 7) << 24,

    // bit 6 -- R4 Load
    R4_DO_NOTHING           = (((uint128_t)0b0ul)       << 6) << 24,
    R4_LOAD                 = (((uint128_t)0b1ul)       << 6) << 24,

    // bit 5 -- R5 Load
    R5_DO_NOTHING           = (((uint128_t)0b0ul)       << 5) << 24,
    R5_LOAD                 = (((uint128_t)0b1ul)       << 5) << 24,

    // bit 4 -- R6 Load
    R6_DO_NOTHING           = (((uint128_t)0b0ul)       << 4) << 24,
    R6_LOAD                 = (((uint128_t)0b1ul)       << 4) << 24,

    // bit 3 -- R7 Load
    R7_DO_NOTHING           = (((uint128_t)0b0ul)       << 3) << 24,
    R7_LOAD                 = (((uint128_t)0b1ul)       << 3) << 24,

    // bit 2 -- R8 Load
    R8_DO_NOTHING           = (((uint128_t)0b0ul)       << 2) << 24,
    R8_LOAD                 = (((uint128_t)0b1ul)       << 2) << 24,

    // bit 1 -- R9 Load
    R9_DO_NOTHING           = (((uint128_t)0b0ul)       << 1) << 24,
    R9_LOAD                 = (((uint128_t)0b1ul)       << 1) << 24,

    // bit 0 -- R10 Load
    R10_DO_NOTHING          = (((uint128_t)0b0ul)       << 0) << 24,
    R10_LOAD                = (((uint128_t)0b1ul)       << 0) << 24,


    //---------------------------------------------------

    //
    // == CTRL5
    //    =====

    // bit 7 -- R11 Load
    R11_DO_NOTHING          = (((uint128_t)0b0ul)       << 7) << 32,
    R11_LOAD                = (((uint128_t)0b1ul)       << 7) << 32,

    // bit 6 -- R12 Load
    R12_DO_NOTHING          = (((uint128_t)0b0ul)       << 6) << 32,
    R12_LOAD                = (((uint128_t)0b1ul)       << 6) << 32,

    // bit 5 -- DEV01 Load
    DEV01_DO_NOTHING        = (((uint128_t)0b0ul)       << 5) << 32,
    DEV01_LOAD              = (((uint128_t)0b1ul)       << 5) << 32,

    // bit 4 -- CTL01 Load
    CTL01_DO_NOTHING        = (((uint128_t)0b0ul)       << 4) << 32,
    CTL01_LOAD              = (((uint128_t)0b1ul)       << 4) << 32,

    // bit 3 -- DEV02 Load
    DEV02_DO_NOTHING        = (((uint128_t)0b0ul)       << 3) << 32,
    DEV02_LOAD              = (((uint128_t)0b1ul)       << 3) << 32,

    // bit 2 -- CTL02 Load
    CTL02_DO_NOTHING        = (((uint128_t)0b0ul)       << 2) << 32,
    CTL02_LOAD              = (((uint128_t)0b1ul)       << 2) << 32,

    // bit 1 -- DEV03 Load
    DEV03_DO_NOTHING        = (((uint128_t)0b0ul)       << 1) << 32,
    DEV03_LOAD              = (((uint128_t)0b1ul)       << 1) << 32,

    // bit 0 -- CTL03 Load
    CTL03_DO_NOTHING        = (((uint128_t)0b0ul)       << 0) << 32,
    CTL03_LOAD              = (((uint128_t)0b1ul)       << 0) << 32,


    //---------------------------------------------------

    //
    // == CTRL6
    //    =====

    // bit 7 -- DEV04 Load
    DEV04_DO_NOTHING        = (((uint128_t)0b0ul)       << 7) << 40,
    DEV04_LOAD              = (((uint128_t)0b1ul)       << 7) << 40,

    // bit 6 -- CTL04 Load
    CTL04_DO_NOTHING        = (((uint128_t)0b0ul)       << 6) << 40,
    CTL04_LOAD              = (((uint128_t)0b1ul)       << 6) << 40,

    // bit 5 -- DEV05 Load
    DEV05_DO_NOTHING        = (((uint128_t)0b0ul)       << 5) << 40,
    DEV05_LOAD              = (((uint128_t)0b1ul)       << 5) << 40,

    // bit 4 -- CTL05 Load
    CTL05_DO_NOTHING        = (((uint128_t)0b0ul)       << 4) << 40,
    CTL05_LOAD              = (((uint128_t)0b1ul)       << 4) << 40,

    // bit 3 -- DEV06 Load
    DEV06_DO_NOTHING        = (((uint128_t)0b0ul)       << 3) << 40,
    DEV06_LOAD              = (((uint128_t)0b1ul)       << 3) << 40,

    // bit 2 -- CTL06 Load
    CTL06_DO_NOTHING        = (((uint128_t)0b0ul)       << 2) << 40,
    CTL06_LOAD              = (((uint128_t)0b1ul)       << 2) << 40,

    // bit 1 -- DEV07 Load
    DEV07_DO_NOTHING        = (((uint128_t)0b0ul)       << 1) << 40,
    DEV07_LOAD              = (((uint128_t)0b1ul)       << 1) << 40,

    // bit 0 -- CTL07 Load
    CTL07_DO_NOTHING        = (((uint128_t)0b0ul)       << 0) << 40,
    CTL07_LOAD              = (((uint128_t)0b1ul)       << 0) << 40,


    //---------------------------------------------------

    //
    // == CTRL7
    //    =====

    // bit 7 -- DEV08 Load
    DEV08_DO_NOTHING        = (((uint128_t)0b0ul)       << 7) << 48,
    DEV08_LOAD              = (((uint128_t)0b1ul)       << 7) << 48,

    // bit 6 -- CTL08 Load
    CTL08_DO_NOTHING        = (((uint128_t)0b0ul)       << 6) << 48,
    CTL08_LOAD              = (((uint128_t)0b1ul)       << 6) << 48,

    // bit 5 -- DEV09 Load
    DEV09_DO_NOTHING        = (((uint128_t)0b0ul)       << 5) << 48,
    DEV09_LOAD              = (((uint128_t)0b1ul)       << 5) << 48,

    // bit 4 -- CTL09 Load
    CTL09_DO_NOTHING        = (((uint128_t)0b0ul)       << 4) << 48,
    CTL09_LOAD              = (((uint128_t)0b1ul)       << 4) << 48,

    // bit 3 -- DEV10 Load
    DEV10_DO_NOTHING        = (((uint128_t)0b0ul)       << 3) << 48,
    DEV10_LOAD              = (((uint128_t)0b1ul)       << 3) << 48,

    // bit 2 -- CTL10 Load
    CTL10_DO_NOTHING        = (((uint128_t)0b0ul)       << 2) << 48,
    CTL10_LOAD              = (((uint128_t)0b1ul)       << 2) << 48,

    // bits 1:0 -- Unused for now


    //---------------------------------------------------

    //
    // == CTRL8
    //    =====

    // bit 7 -- Clear Carry Flag
    CLC                     = (((uint128_t)0b1ul)       << 7) << 56,

    // bit 6 -- Set Carry Flag
    STC                     = (((uint128_t)0b1ul)       << 6) << 56,

    // bit 5 -- Latch Z Flag (Pgm)
    PGM_Z_LATCH             = (((uint128_t)0b1ul)       << 5) << 56,

    // bit 4 -- Latch C Flag (Pgm)
    PGM_C_LATCH             = (((uint128_t)0b1ul)       << 4) << 56,

    // bit 3 -- Latch N Flag (Pgm)
    PGM_N_LATCH             = (((uint128_t)0b1ul)       << 3) << 56,

    // bit 2 -- Latch V Flag (Pgm)
    PGM_V_LATCH             = (((uint128_t)0b1ul)       << 2) << 56,

    // bit 1 -- Latch L Flag (Pgm)
    PGM_L_LATCH             = (((uint128_t)0b1ul)       << 1) << 56,

    // bit 0 -- Unused for now


    //---------------------------------------------------

    //
    // == CTRL9
    //    =====

    // bit 7:6 -- Carry Select
    CARRY_0                 = (((uint128_t)0b00ul)      << 6) << 64,
    CARRY_LAST              = (((uint128_t)0b01ul)      << 6) << 64,
    CARRY_INVERTED          = (((uint128_t)0b10ul)      << 6) << 64,
    CARRY_1                 = (((uint128_t)0b11ul)      << 6) << 64,

    // bit 5 -- Latch Z Flag (Pgm)
    INT_Z_LATCH             = (((uint128_t)0b1ul)       << 5) << 64,

    // bit 4 -- Latch C Flag (Pgm)
    INT_C_LATCH             = (((uint128_t)0b1ul)       << 4) << 64,

    // bit 3 -- Latch N Flag (Pgm)
    INT_N_LATCH             = (((uint128_t)0b1ul)       << 3) << 64,

    // bit 2 -- Latch V Flag (Pgm)
    INT_V_LATCH             = (((uint128_t)0b1ul)       << 2) << 64,

    // bit 1 -- Latch L Flag (Pgm)
    INT_L_LATCH             = (((uint128_t)0b1ul)       << 1) << 64,

    // bit 0 -- ALU Result Latch
    ALU_RESULT_LATCH        = (((uint128_t)0b1ul)       << 0) << 64,


    //---------------------------------------------------



    //
    // == CTRL10
    //    ======

    // bit 7:4 -- ALU A Assert
    ALUA_NONE               = (((uint128_t)0b0000ul)    << 4) << 72,
    ALUA_R1                 = (((uint128_t)0b0001ul)    << 4) << 72,
    ALUA_R2                 = (((uint128_t)0b0010ul)    << 4) << 72,
    ALUA_R3                 = (((uint128_t)0b0011ul)    << 4) << 72,
    ALUA_R4                 = (((uint128_t)0b0100ul)    << 4) << 72,
    ALUA_R5                 = (((uint128_t)0b0101ul)    << 4) << 72,
    ALUA_R6                 = (((uint128_t)0b0110ul)    << 4) << 72,
    ALUA_R7                 = (((uint128_t)0b0111ul)    << 4) << 72,
    ALUA_R8                 = (((uint128_t)0b1000ul)    << 4) << 72,
    ALUA_R9                 = (((uint128_t)0b1001ul)    << 4) << 72,
    ALUA_R10                = (((uint128_t)0b1010ul)    << 4) << 72,
    ALUA_R11                = (((uint128_t)0b1011ul)    << 4) << 72,
    ALUA_R12                = (((uint128_t)0b1100ul)    << 4) << 72,
    ALUA_PGM_SP             = (((uint128_t)0b1101ul)    << 4) << 72,
    ALUA_INT_SP             = (((uint128_t)0b1110ul)    << 4) << 72,

    // bit 3:0 -- ALU B Assert
    ALUB_NONE               = (((uint128_t)0b0000ul)    << 0) << 72,
    ALUB_R1                 = (((uint128_t)0b0001ul)    << 0) << 72,
    ALUB_R2                 = (((uint128_t)0b0010ul)    << 0) << 72,
    ALUB_R3                 = (((uint128_t)0b0011ul)    << 0) << 72,
    ALUB_R4                 = (((uint128_t)0b0100ul)    << 0) << 72,
    ALUB_R5                 = (((uint128_t)0b0101ul)    << 0) << 72,
    ALUB_R6                 = (((uint128_t)0b0110ul)    << 0) << 72,
    ALUB_R7                 = (((uint128_t)0b0111ul)    << 0) << 72,
    ALUB_R8                 = (((uint128_t)0b1000ul)    << 0) << 72,
    ALUB_R9                 = (((uint128_t)0b1001ul)    << 0) << 72,
    ALUB_R10                = (((uint128_t)0b1010ul)    << 0) << 72,
    ALUB_R11                = (((uint128_t)0b1011ul)    << 0) << 72,
    ALUB_R12                = (((uint128_t)0b1100ul)    << 0) << 72,
    ALUB_FETCH              = (((uint128_t)0b1101ul)    << 0) << 72,
    ALUB_MEM                = (((uint128_t)0b1110ul)    << 0) << 72,


    //---------------------------------------------------


    //
    // == Improve code readability
    //    ========================
    FETCH_ASSERT_MAIN       = MAIN_FETCH | INSTRUCTION_SUPPRESS,
};


//
// -- This #define should help readability in the code
//    since a '1' on that flag means that the condition was not met
//    -------------------------------------------------------------
#define CONDITION_MET(x) (((x) & FLAG_CONDITION) == 0)



//
// -- These are the instructions which will be encoded
//
//    Recall that the instruction word has the following format:
//
//              CCCC IIII IIII IIII
//
//    Where:
//    - CCCC are control flags, used to condition the instruction
//    - IIII IIII IIII is the instruction, encoded in the enum below
//    --------------------------------------------------------------
#include "opcodes.h"


//
// -- the size of the eeprom
//    ----------------------
const int PROM_SIZE = 1024 * 32;         // we are using 32KB EEPROM


//
// -- this eeprom buffer(s)
//    ---------------------
uint128_t promBuffer [PROM_SIZE];


//
// -- Break the prom location down to the flags and instruction portions
//    and determine the control lines for each possible combination
//    ------------------------------------------------------------------
uint128_t GenerateControlSignals(int loc)
{
    int flags = (loc >> 12) & 0x7;           // top 3 bits of the memory address; flags for augmenting the control signals
    int instr = (loc >>  0) & 0xfff;         // bottom 12 bits for the memory address of the instruction

    const uint128_t nop = ADDR_BUS_1_ASSERT_PC |  PC_INC; // Note that `| INSTRUCTION_ASSERT` == `| 0`, ∴ omitted
    uint128_t out = ADDR_BUS_1_ASSERT_PC | PC_INC;

    switch (instr) {
    default:
    case OPCODE_NOP:
        return nop;

    case OPCODE_MOV_R1___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | FETCH_ASSERT_MAIN | R1_LOAD;

    case OPCODE_MOV_R2___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | FETCH_ASSERT_MAIN | R2_LOAD;

    case OPCODE_MOV_R1_RZ:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_NONE | R1_LOAD;


    case OPCODE_MOV_R2_RZ:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_NONE | R2_LOAD;


    case OPCODE_MOV_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_R1 | R2_LOAD;

    case OPCODE_MOV_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_R2 | R1_LOAD;

    case OPCODE_ADD_R1___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_0 | ALUA_R1 | ALUB_FETCH | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADD_R2___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_0 | ALUA_R2 | ALUB_FETCH | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADD_R1_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_0 | ALUA_R1 | ALUB_R1 | MAIN_ALU_RESULT  | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADD_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_0 | ALUA_R1 | ALUB_R2 | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADD_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_0 | ALUA_R2 | ALUB_R1 | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADD_R2_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_0 | ALUA_R2 | ALUB_R2 | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R1___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_LAST | ALUA_R1 | ALUB_FETCH | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R2___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_LAST | ALUA_R2 | ALUB_FETCH | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R1_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_LAST | ALUA_R1 | ALUB_R1 | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_LAST | ALUA_R1 | ALUB_R2 | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_LAST | ALUA_R2 | ALUB_R1 | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_ADC_R2_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_LAST | ALUA_R2 | ALUB_R2 | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_INC_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_1 | ALUA_R1 | ALUB_NONE | MAIN_ALU_RESULT | R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_INC_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_1 | ALUA_R2 | ALUB_NONE | MAIN_ALU_RESULT | R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH |
                PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH | ALU_RESULT_LATCH;

    case OPCODE_JMP___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return FETCH_ASSERT_MAIN | PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PC;

    case OPCODE_JMP_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return MAIN_R1 | PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PC;

    case OPCODE_JMP_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return MAIN_R2 | PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PC;

    case OPCODE_CLC:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CLC;

    case OPCODE_STC:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | STC;
    }
}



//
// -- Main entry point
//    ----------------
int main(void)
{
//    printf("CARRY_1 is %16.16lx%16.16lx\n", (uint64_t)(CARRY_1>>64), (uint64_t)CARRY_1);

    for (int i = 0; i < PROM_SIZE; i ++) {
        promBuffer[i] = GenerateControlSignals(i);
    }

    FILE *of0;
    FILE *of1;
    FILE *of2;
    FILE *of3;
    FILE *of4;
    FILE *of5;
    FILE *of6;
    FILE *of7;
    FILE *of8;
    FILE *of9;
    FILE *ofa;
    FILE *ofb;

    // -- Open each output file in turn
    of0 = fopen("ctrl0.bin", "w");
    if (!of0) perror("Unable to open ctrl0.bin");

    of1 = fopen("ctrl1.bin", "w");
    if (!of1) perror("Unable to open ctrl1.bin");

    of2 = fopen("ctrl2.bin", "w");
    if (!of2) perror("Unable to open ctrl2.bin");

    of3 = fopen("ctrl3.bin", "w");
    if (!of3) perror("Unable to open ctrl3.bin");

    of4 = fopen("ctrl4.bin", "w");
    if (!of4) perror("Unable to open ctrl4.bin");

    of5 = fopen("ctrl5.bin", "w");
    if (!of5) perror("Unable to open ctrl5.bin");

    of6 = fopen("ctrl6.bin", "w");
    if (!of6) perror("Unable to open ctrl6.bin");

    of7 = fopen("ctrl7.bin", "w");
    if (!of7) perror("Unable to open ctrl7.bin");

    of8 = fopen("ctrl8.bin", "w");
    if (!of8) perror("Unable to open ctrl8.bin");

    of9 = fopen("ctrl9.bin", "w");
    if (!of9) perror("Unable to open ctrl9.bin");

    ofa = fopen("ctrla.bin", "w");
    if (!ofa) perror("Unable to open ctrla.bin");

    ofb = fopen("ctrlb.bin", "w");
    if (!ofb) perror("Unable to open ctrlb.bin");


    // -- write each EEPROM
    for (int i = 0; i < PROM_SIZE; i ++) {
        uint8_t byte0 = (promBuffer[i] >>  0) & 0xff;
        uint8_t byte1 = (promBuffer[i] >>  8) & 0xff;
        uint8_t byte2 = (promBuffer[i] >> 16) & 0xff;
        uint8_t byte3 = (promBuffer[i] >> 24) & 0xff;
        uint8_t byte4 = (promBuffer[i] >> 32) & 0xff;
        uint8_t byte5 = (promBuffer[i] >> 40) & 0xff;
        uint8_t byte6 = (promBuffer[i] >> 48) & 0xff;
        uint8_t byte7 = (promBuffer[i] >> 56) & 0xff;
        uint8_t byte8 = (promBuffer[i] >> 64) & 0xff;
        uint8_t byte9 = (promBuffer[i] >> 72) & 0xff;
        uint8_t bytea = (promBuffer[i] >> 80) & 0xff;
        uint8_t byteb = (promBuffer[i] >> 88) & 0xff;

        fwrite(&byte0, 1, sizeof(uint8_t), of0);
        fwrite(&byte1, 1, sizeof(uint8_t), of1);
        fwrite(&byte2, 1, sizeof(uint8_t), of2);
        fwrite(&byte3, 1, sizeof(uint8_t), of3);
        fwrite(&byte4, 1, sizeof(uint8_t), of4);
        fwrite(&byte5, 1, sizeof(uint8_t), of5);
        fwrite(&byte6, 1, sizeof(uint8_t), of6);
        fwrite(&byte7, 1, sizeof(uint8_t), of7);
        fwrite(&byte8, 1, sizeof(uint8_t), of8);
        fwrite(&byte9, 1, sizeof(uint8_t), of9);
        fwrite(&bytea, 1, sizeof(uint8_t), ofa);
        fwrite(&byteb, 1, sizeof(uint8_t), ofb);
    }

    // -- Flush the buffers -- just to be sure
    fflush(of0);
    fflush(of1);
    fflush(of2);
    fflush(of3);
    fflush(of4);
    fflush(of5);
    fflush(of6);
    fflush(of7);
    fflush(of8);
    fflush(of9);
    fflush(ofa);
    fflush(ofb);


    // -- close the files
    fclose(of0);
    fclose(of1);
    fclose(of2);
    fclose(of3);
    fclose(of4);
    fclose(of5);
    fclose(of6);
    fclose(of7);
    fclose(of8);
    fclose(of9);
    fclose(ofa);
    fclose(ofb);
}



