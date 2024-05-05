//===================================================================================================================
//  control.cc -- Generate the control logic for the 16-Bit Computer From Scratch
//
//  This file will generate the control logic for the 16-Bit Computer From Scratch.  This is intended to be a
//  temporary solution for the breadboard incarnation.  When we get to moving this to PCB, a different solution
//  will be used (as in, not EEPROM).
//
//  -----------------------------------------------------------------------------------------------------------------
//
//  The following are the unused signal counts in case something needs to be added:
//
//  |  ROM  | # |
//  |:-----:|:--|
//  | CTRL0 | - |
//  | CTRL1 | - |
//  | CTRL2 | - |
//  | CTRL3 | - |
//  | CTRL4 | - |
//  | CTRL5 | 6 |
//  | CTRL6 | 2 |
//  | CTRL7 | - |
//  | CTRL8 | - |
//  | CTRL9 | - |
//  | CTRLA | - |
//  | CTRLB | - |
//  | CTRLC | - |
//  | CTRLD | - |
//  | CTRLE | - |
//  | CTRLF | - |
//  |=======|===|
//  | Total | 8 |
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
//  2024-Apr-28  #136     V-0098   ADCL  Reorganize all the control logic signals
//
//===================================================================================================================


#include <cstdint>
#include <stdio.h>
#include <cstring>



//
// -- Define the control word size
//    ----------------------------
typedef __uint128_t uint128_t;



//
// -- this macro will be used to assist in building the enum values
//    -------------------------------------------------------------
#define U128_ENUM(v,bit,byte) ((((uint128_t)v)<<bit)<<(byte*8))



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
    // == CTRL0 -- Control ROM 0
    //    ======================

    // bit 7 -- Clear Carry Flag
    CLC                                 = U128_ENUM(0b1ul, 7, 0),


    // bit 6 -- Set Carry Flag
    STC                                 = U128_ENUM(0b1ul, 6, 0),


    // bit 5 -- Latch Z Flag (Pgm)
    PGM_Z_LATCH                         = U128_ENUM(0b1ul, 5, 0),


    // bit 4 -- Latch Z Flag (Interrupt)
    INT_Z_LATCH                         = U128_ENUM(0b1ul, 4, 0),


    // bit 3 -- Latch C Flag (Pgm)
    PGM_C_LATCH                         = U128_ENUM(0b1ul, 3, 0),


    // bit 2 -- Latch C Flag (Interrupt)
    INT_C_LATCH                         = U128_ENUM(0b1ul, 2, 0),


    // bit 1:0 -- Address Bus 1 Assert
    ADDR_BUS_1_ASSERT_PGMPC             = U128_ENUM(0b00ul, 0, 0),
    ADDR_BUS_1_ASSERT_PGMRA             = U128_ENUM(0b01ul, 0, 0),
    ADDR_BUS_1_ASSERT_INTPC             = U128_ENUM(0b10ul, 0, 0),
    ADDR_BUS_1_ASSERT_INTRA             = U128_ENUM(0b11ul, 0, 0),



    //---------------------------------------------------



    //
    // == CTRL1 -- Control ROM 1
    //    ======================

    // bit 7 -- Latch N Flag (Pgm)
    PGM_N_LATCH                         = U128_ENUM(0b1ul, 7, 1),


    // bit 6 -- Latch N Flag (Interrupt)
    INT_N_LATCH                         = U128_ENUM(0b1ul, 6, 1),


    // bit 5 -- Latch V Flag (Pgm)
    PGM_V_LATCH                         = U128_ENUM(0b1ul, 5, 1),


    // bit 4 -- Latch V Flag (Interrupt)
    INT_V_LATCH                         = U128_ENUM(0b1ul, 4, 1),


    // bits 3:0 -- Address Bus 2 Assert
    ADDR_BUS_2_ASSERT_NONE               = U128_ENUM(0b0000ul, 0, 1),
    ADDR_BUS_2_ASSERT_R1                 = U128_ENUM(0b0001ul, 0, 1),
    ADDR_BUS_2_ASSERT_R2                 = U128_ENUM(0b0010ul, 0, 1),
    ADDR_BUS_2_ASSERT_R3                 = U128_ENUM(0b0011ul, 0, 1),
    ADDR_BUS_2_ASSERT_R4                 = U128_ENUM(0b0100ul, 0, 1),
    ADDR_BUS_2_ASSERT_R5                 = U128_ENUM(0b0101ul, 0, 1),
    ADDR_BUS_2_ASSERT_R6                 = U128_ENUM(0b0110ul, 0, 1),
    ADDR_BUS_2_ASSERT_R7                 = U128_ENUM(0b0111ul, 0, 1),
    ADDR_BUS_2_ASSERT_R8                 = U128_ENUM(0b1000ul, 0, 1),
    ADDR_BUS_2_ASSERT_R9                 = U128_ENUM(0b1001ul, 0, 1),
    ADDR_BUS_2_ASSERT_R10                = U128_ENUM(0b1010ul, 0, 1),
    ADDR_BUS_2_ASSERT_R11                = U128_ENUM(0b1011ul, 0, 1),
    ADDR_BUS_2_ASSERT_R12                = U128_ENUM(0b1100ul, 0, 1),
    ADDR_BUS_2_ASSERT_FETCH              = U128_ENUM(0b1101ul, 0, 1),
    ADDR_BUS_2_ASSERT_PGM_SP             = U128_ENUM(0b1110ul, 0, 1),
    ADDR_BUS_2_ASSERT_INT_SP             = U128_ENUM(0b1111ul, 0, 1),



    //---------------------------------------------------



    //
    // == CTRL2 -- Control ROM 2
    //    ======================

    // bit 7 -- Latch L Flag (Pgm)
    PGM_L_LATCH                         = U128_ENUM(0b1ul, 7, 2),


    // bit 6 -- Latch L Flag (Interrupt)
    INT_L_LATCH                         = U128_ENUM(0b1ul, 6, 2),


    // bit 5 -- Clear oVerflow Flag
    CLV                                 = U128_ENUM(0b1ul, 5, 0),


    // bit 4 -- Set oVerflow Flag
    STV                                 = U128_ENUM(0b1ul, 4, 0),


    // bit 3:0 -- ALU A Assert
    ALU_BUS_A_ASSERT_NONE                = U128_ENUM(0b0000ul, 0, 2),
    ALU_BUS_A_ASSERT_R1                  = U128_ENUM(0b0001ul, 0, 2),
    ALU_BUS_A_ASSERT_R2                  = U128_ENUM(0b0010ul, 0, 2),
    ALU_BUS_A_ASSERT_R3                  = U128_ENUM(0b0011ul, 0, 2),
    ALU_BUS_A_ASSERT_R4                  = U128_ENUM(0b0100ul, 0, 2),
    ALU_BUS_A_ASSERT_R5                  = U128_ENUM(0b0101ul, 0, 2),
    ALU_BUS_A_ASSERT_R6                  = U128_ENUM(0b0110ul, 0, 2),
    ALU_BUS_A_ASSERT_R7                  = U128_ENUM(0b0111ul, 0, 2),
    ALU_BUS_A_ASSERT_R8                  = U128_ENUM(0b1000ul, 0, 2),
    ALU_BUS_A_ASSERT_R9                  = U128_ENUM(0b1001ul, 0, 2),
    ALU_BUS_A_ASSERT_R10                 = U128_ENUM(0b1010ul, 0, 2),
    ALU_BUS_A_ASSERT_R11                 = U128_ENUM(0b1011ul, 0, 2),
    ALU_BUS_A_ASSERT_R12                 = U128_ENUM(0b1100ul, 0, 2),
    // -- unused                         = U128_ENUM(0b1101ul, 0, 2),
    ALU_BUS_A_ASSERT_PGM_SP              = U128_ENUM(0b1110ul, 0, 2),
    ALU_BUS_A_ASSERT_INT_SP              = U128_ENUM(0b1111ul, 0, 2),



    //---------------------------------------------------



    //
    // == CTRL3 -- Control ROM 3
    //    ======================

    // bit 7:4 -- ALU Logic Unit Results
    ALU_LOGIC_RESULT_FALSE              = U128_ENUM(0b0000ul, 4, 3),
    ALU_LOGIC_RESULT_AND                = U128_ENUM(0b0001ul, 4, 3),
    ALU_LOGIC_RESULT_OR                 = U128_ENUM(0b0111ul, 4, 3),
    ALU_LOGIC_RESULT_XOR                = U128_ENUM(0b0110ul, 4, 3),
    ALU_LOGIC_RESULT_NOR                = U128_ENUM(0b1000ul, 4, 3),
    ALU_LOGIC_RESULT_XNOR               = U128_ENUM(0b1001ul, 4, 3),
    ALU_LOGIC_RESULT_NOT_B              = U128_ENUM(0b1010ul, 4, 3),
    ALU_LOGIC_RESULT_NOT_A              = U128_ENUM(0b1100ul, 4, 3),
    ALU_LOGIC_RESULT_NAND               = U128_ENUM(0b1110ul, 4, 3),
    ALU_LOGIC_RESULT_TRUE               = U128_ENUM(0b1111ul, 4, 3),


    // bit 3:0 -- ALU B Assert
    ALU_BUS_B_ASSERT_NONE                = U128_ENUM(0b0000ul, 0, 3),
    ALU_BUS_B_ASSERT_R1                  = U128_ENUM(0b0001ul, 0, 3),
    ALU_BUS_B_ASSERT_R2                  = U128_ENUM(0b0010ul, 0, 3),
    ALU_BUS_B_ASSERT_R3                  = U128_ENUM(0b0011ul, 0, 3),
    ALU_BUS_B_ASSERT_R4                  = U128_ENUM(0b0100ul, 0, 3),
    ALU_BUS_B_ASSERT_R5                  = U128_ENUM(0b0101ul, 0, 3),
    ALU_BUS_B_ASSERT_R6                  = U128_ENUM(0b0110ul, 0, 3),
    ALU_BUS_B_ASSERT_R7                  = U128_ENUM(0b0111ul, 0, 3),
    ALU_BUS_B_ASSERT_R8                  = U128_ENUM(0b1000ul, 0, 3),
    ALU_BUS_B_ASSERT_R9                  = U128_ENUM(0b1001ul, 0, 3),
    ALU_BUS_B_ASSERT_R10                 = U128_ENUM(0b1010ul, 0, 3),
    ALU_BUS_B_ASSERT_R11                 = U128_ENUM(0b1011ul, 0, 3),
    ALU_BUS_B_ASSERT_R12                 = U128_ENUM(0b1100ul, 0, 3),
    ALU_BUS_B_ASSERT_PGM_SP              = U128_ENUM(0b1101ul, 0, 3),
    ALU_BUS_B_ASSERT_INT_SP              = U128_ENUM(0b1110ul, 0, 3),
    ALU_BUS_B_ASSERT_FETCH               = U128_ENUM(0b1111ul, 0, 3),



    //---------------------------------------------------



    //
    // == CTRL4 -- Control ROM 4
    //    ======================

    // bit 7 -- Instruction Suppress
    INSTRUCTION_SUPPRESS                = U128_ENUM(0b1ul, 7, 4),


    // bit 6:0 -- assert to Main Bus
    MAIN_BUS_ASSERT_NONE                 = U128_ENUM(0b0000000ul, 0, 4),
    MAIN_BUS_ASSERT_R1                   = U128_ENUM(0b0000001ul, 0, 4),
    MAIN_BUS_ASSERT_R2                   = U128_ENUM(0b0000010ul, 0, 4),
    MAIN_BUS_ASSERT_R3                   = U128_ENUM(0b0000011ul, 0, 4),
    MAIN_BUS_ASSERT_R4                   = U128_ENUM(0b0000100ul, 0, 4),
    MAIN_BUS_ASSERT_R5                   = U128_ENUM(0b0000101ul, 0, 4),
    MAIN_BUS_ASSERT_R6                   = U128_ENUM(0b0000110ul, 0, 4),
    MAIN_BUS_ASSERT_R7                   = U128_ENUM(0b0000111ul, 0, 4),
    MAIN_BUS_ASSERT_R8                   = U128_ENUM(0b0001000ul, 0, 4),
    MAIN_BUS_ASSERT_R9                   = U128_ENUM(0b0001001ul, 0, 4),
    MAIN_BUS_ASSERT_R10                  = U128_ENUM(0b0001010ul, 0, 4),
    MAIN_BUS_ASSERT_R11                  = U128_ENUM(0b0001011ul, 0, 4),
    MAIN_BUS_ASSERT_R12                  = U128_ENUM(0b0001100ul, 0, 4),
    MAIN_BUS_ASSERT_PGM_SP               = U128_ENUM(0b0001101ul, 0, 4),
    MAIN_BUS_ASSERT_PGM_RA               = U128_ENUM(0b0001110ul, 0, 4),
    MAIN_BUS_ASSERT_PGM_PC               = U128_ENUM(0b0001111ul, 0, 4),
    MAIN_BUS_ASSERT_INT_SP               = U128_ENUM(0b0010000ul, 0, 4),
    MAIN_BUS_ASSERT_INT_RA               = U128_ENUM(0b0010001ul, 0, 4),
    MAIN_BUS_ASSERT_INT_PC               = U128_ENUM(0b0010010ul, 0, 4),
    MAIN_BUS_ASSERT_FETCH                = U128_ENUM(0b0010011ul, 0, 4),
    MAIN_BUS_ASSERT_DEV1                 = U128_ENUM(0b0010100ul, 0, 4),
    MAIN_BUS_ASSERT_DEV2                 = U128_ENUM(0b0010101ul, 0, 4),
    MAIN_BUS_ASSERT_DEV3                 = U128_ENUM(0b0010110ul, 0, 4),
    MAIN_BUS_ASSERT_DEV4                 = U128_ENUM(0b0010111ul, 0, 4),
    MAIN_BUS_ASSERT_DEV5                 = U128_ENUM(0b0011000ul, 0, 4),
    MAIN_BUS_ASSERT_DEV6                 = U128_ENUM(0b0011001ul, 0, 4),
    MAIN_BUS_ASSERT_DEV7                 = U128_ENUM(0b0011010ul, 0, 4),
    MAIN_BUS_ASSERT_DEV8                 = U128_ENUM(0b0011011ul, 0, 4),
    MAIN_BUS_ASSERT_DEV9                 = U128_ENUM(0b0011100ul, 0, 4),
    MAIN_BUS_ASSERT_DEV10                = U128_ENUM(0b0011101ul, 0, 4),
    MAIN_BUS_ASSERT_ALU_RESULT           = U128_ENUM(0b0011110ul, 0, 4),
    MAIN_BUS_ASSERT_MEMORY               = U128_ENUM(0b0011111ul, 0, 4),
    // unused block
    MAIN_BUS_ASSERT_CTL1                 = U128_ENUM(0b0100100ul, 0, 4),
    MAIN_BUS_ASSERT_CTL2                 = U128_ENUM(0b0100101ul, 0, 4),
    MAIN_BUS_ASSERT_CTL3                 = U128_ENUM(0b0100110ul, 0, 4),
    MAIN_BUS_ASSERT_CTL4                 = U128_ENUM(0b0100111ul, 0, 4),
    MAIN_BUS_ASSERT_CTL5                 = U128_ENUM(0b0101000ul, 0, 4),
    MAIN_BUS_ASSERT_CTL6                 = U128_ENUM(0b0101001ul, 0, 4),
    MAIN_BUS_ASSERT_CTL7                 = U128_ENUM(0b0101010ul, 0, 4),
    MAIN_BUS_ASSERT_CTL8                 = U128_ENUM(0b0101011ul, 0, 4),
    MAIN_BUS_ASSERT_CTL9                 = U128_ENUM(0b0101100ul, 0, 4),
    MAIN_BUS_ASSERT_CTL10                = U128_ENUM(0b0101101ul, 0, 4),
    // unused block
    MAIN_BUS_ASSERT_SWAP_R1              = U128_ENUM(0b1000001ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R2              = U128_ENUM(0b1000010ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R3              = U128_ENUM(0b1000011ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R4              = U128_ENUM(0b1000100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R5              = U128_ENUM(0b1000101ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R6              = U128_ENUM(0b1000110ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R7              = U128_ENUM(0b1000111ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R8              = U128_ENUM(0b1001000ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R9              = U128_ENUM(0b1001001ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R10             = U128_ENUM(0b1001010ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R11             = U128_ENUM(0b1001011ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_R12             = U128_ENUM(0b1001100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_PGM_SP          = U128_ENUM(0b1001101ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_PGM_RA          = U128_ENUM(0b1001110ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_PGM_PC          = U128_ENUM(0b1001111ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_INT_SP          = U128_ENUM(0b1010000ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_INT_RA          = U128_ENUM(0b1010001ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_INT_PC          = U128_ENUM(0b1010010ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_FETCH           = U128_ENUM(0b1010011ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV1            = U128_ENUM(0b1010100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV2            = U128_ENUM(0b1010101ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV3            = U128_ENUM(0b1010110ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV4            = U128_ENUM(0b1010111ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV5            = U128_ENUM(0b1011000ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV6            = U128_ENUM(0b1011001ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV7            = U128_ENUM(0b1011010ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV8            = U128_ENUM(0b1011011ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV9            = U128_ENUM(0b1011100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_DEV10           = U128_ENUM(0b1011101ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_ALU_RESULT      = U128_ENUM(0b1011110ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_MEMORY          = U128_ENUM(0b1011111ul, 0, 4),
    // unused block
    MAIN_BUS_ASSERT_SWAP_CTL1            = U128_ENUM(0b1100100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL2            = U128_ENUM(0b1100101ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL3            = U128_ENUM(0b1100110ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL4            = U128_ENUM(0b1100111ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL5            = U128_ENUM(0b1101000ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL6            = U128_ENUM(0b1101001ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL7            = U128_ENUM(0b1101010ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL8            = U128_ENUM(0b1101011ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL9            = U128_ENUM(0b1101100ul, 0, 4),
    MAIN_BUS_ASSERT_SWAP_CTL10           = U128_ENUM(0b1101101ul, 0, 4),
    // unused block



    //---------------------------------------------------



    //
    // == CTRL5 -- Control ROM 5
    //    ======================

    // bit 7:2 -- unused


    // bit 1:0 -- Carry Select
    CARRY_SELECT_0                       = U128_ENUM(0b00ul, 0, 5),
    CARRY_SELECT_LAST                    = U128_ENUM(0b01ul, 0, 5),
    CARRY_SELECT_INVERTED                = U128_ENUM(0b10ul, 0, 5),
    CARRY_SELECT_1                       = U128_ENUM(0b11ul, 0, 5),



    //---------------------------------------------------



    //
    // == CTRL6 -- Control ROM 6
    //    ======================


    // bit 7:6 -- unused

    // bit 5 -- Break
    BREAK                               = U128_ENUM(0b1ul, 5, 6),

    // bit 4 -- Leave interrupt Context
    LEAVE_INT_CONTEXT                   = U128_ENUM(0b1ul, 4, 6),

    // bit 3 -- fetch suppress
    FETCH_SUPPRESS                      = U128_ENUM(0b1ul, 3, 6),

    // bit 2:0 -- shift and rotate signals
    ALU_SHIFT_NONE                      = U128_ENUM(0b000ul, 0, 6),
    ALU_ARITH_SHL                       = U128_ENUM(0b001ul, 0, 6),
    ALU_ARITH_SHR                       = U128_ENUM(0b010ul, 0, 6),
    ALU_LGL_SHR                         = U128_ENUM(0b011ul, 0, 6),
    ALU_ROL_CARRY                       = U128_ENUM(0b100ul, 0, 6),
    ALU_ROR_CARRY                       = U128_ENUM(0b101ul, 0, 6),
    ALU_ROL                             = U128_ENUM(0b110ul, 0, 6),
    ALU_ROR                             = U128_ENUM(0b111ul, 0, 6),



    //---------------------------------------------------



    //
    // == CTRL7 -- Control ROM 7
    //    ======================

    // bits 7 -- PGM PC Increment
    PGM_PC_INC                          = U128_ENUM(0b1ul, 7, 7),


    // bits 6 -- PGM PC Load
    PGM_PC_LOAD                         = U128_ENUM(0b1ul, 6, 7),


    // bits 5 -- PGM RA Increment
    PGM_RA_INC                          = U128_ENUM(0b1ul, 5, 7),


    // bits 4 -- PGM RA Load
    PGM_RA_LOAD                         = U128_ENUM(0b1ul, 4, 7),


    // bits 3 -- INT PC Increment
    INT_PC_INC                          = U128_ENUM(0b1ul, 3, 7),


    // bits 2 -- INT PC Load
    INT_PC_LOAD                         = U128_ENUM(0b1ul, 2, 7),


    // bits 1 -- INT RA Increment
    INT_RA_INC                          = U128_ENUM(0b1ul, 1, 7),


    // bits 0 -- INT RA Load
    INT_RA_LOAD                         = U128_ENUM(0b1ul, 0, 7),



    //---------------------------------------------------



    //
    // == CTRL8 -- Control ROM 8
    //    ======================

    // bit 7 -- PGM SP Load
    PGM_SP_LOAD                         = U128_ENUM(0b1ul, 7, 8),


    // bit 6 -- PGM SP Increment
    PGM_SP_INC                          = U128_ENUM(0b1ul, 6, 8),


    // bit 5 -- PGM SP Decrement
    PGM_SP_DEC                          = U128_ENUM(0b1ul, 5, 8),


    // bit 4 -- INT SP Load
    INT_SP_LOAD                         = U128_ENUM(0b1ul, 4, 8),


    // bit 3 -- INT SP Increment
    INT_SP_INC                          = U128_ENUM(0b1ul, 3, 8),


    // bit 2 -- INT SP Decrement
    INT_SP_DEC                          = U128_ENUM(0b1ul, 2, 8),


    // bit 1 -- Subtract
    ALU_SUBTRACT                        = U128_ENUM(0b1ul, 1, 8),


    // bit 0 -- Memory Write
    MEMORY_WRITE                        = U128_ENUM(0b1ul, 0, 8),



    //---------------------------------------------------



    //
    // == CTRL9 -- Control ROM 9
    //    ======================

    // bit 7 -- R1 Load
    R1_LOAD                             = U128_ENUM(0b1ul, 7, 9),


    // bit 6 -- R1 Inc
    R1_INC                              = U128_ENUM(0b1ul, 6, 9),


    // bit 5 -- R1 Dec
    R1_DEC                              = U128_ENUM(0b1ul, 5, 9),


    // bit 4 -- R2 Load
    R2_LOAD                             = U128_ENUM(0b1ul, 4, 9),


    // bit 3 -- R2 Inc
    R2_INC                              = U128_ENUM(0b1ul, 3, 9),


    // bit 2 -- R2 Dec
    R2_DEC                              = U128_ENUM(0b1ul, 2, 9),


    // bit 1 -- R3 Load
    R3_LOAD                             = U128_ENUM(0b1ul, 1, 9),


    // bit 0 -- R2 Inc
    R3_INC                              = U128_ENUM(0b1ul, 0, 9),



    //---------------------------------------------------



    //
    // == CTRLA -- Control ROM 10
    //    =======================

    // bit 7 -- R3 Dec
    R3_DEC                              = U128_ENUM(0b1ul, 7, 10),


    // bit 6 -- R4 Load
    R4_LOAD                             = U128_ENUM(0b1ul, 6, 10),


    // bit 5 -- R4 Inc
    R4_INC                              = U128_ENUM(0b1ul, 5, 10),


    // bit 4 -- R4 Dec
    R4_DEC                              = U128_ENUM(0b1ul, 4, 10),


    // bit 3 -- R5 Load
    R5_LOAD                             = U128_ENUM(0b1ul, 3, 10),


    // bit 2 -- R5 Inc
    R5_INC                              = U128_ENUM(0b1ul, 2, 10),


    // bit 1 -- R5 Dec
    R5_DEC                              = U128_ENUM(0b1ul, 1, 10),


    // bit 0 -- R6 Load
    R6_LOAD                             = U128_ENUM(0b1ul, 0, 10),



    //---------------------------------------------------



    //
    // == CTRLB -- Control ROM 11
    //    =======================

    // bit 7 -- R6 Inc
    R6_INC                              = U128_ENUM(0b1ul, 7, 11),


    // bit 6 -- R6 Dec
    R6_DEC                              = U128_ENUM(0b1ul, 6, 11),


    // bit 5 -- R7 Load
    R7_LOAD                             = U128_ENUM(0b1ul, 5, 11),


    // bit 4 -- R7 Inc
    R7_INC                              = U128_ENUM(0b1ul, 4, 11),


    // bit 3 -- R7 Dec
    R7_DEC                              = U128_ENUM(0b1ul, 3, 11),


    // bit 2 -- R8 Load
    R8_LOAD                             = U128_ENUM(0b1ul, 2, 11),


    // bit 1 -- R8 Inc
    R8_INC                              = U128_ENUM(0b1ul, 1, 11),


    // bit 0 -- R8 Dec
    R8_DEC                              = U128_ENUM(0b1ul, 0, 11),



    //---------------------------------------------------



    //
    // == CTRLC -- Control ROM 12
    //    =======================

    // bit 7 -- R9 Load
    R9_LOAD                             = U128_ENUM(0b1ul, 7, 12),


    // bit 6 -- R9 Inc
    R9_INC                              = U128_ENUM(0b1ul, 6, 12),


    // bit 5 -- R9 Dec
    R9_DEC                              = U128_ENUM(0b1ul, 5, 12),


    // bit 4 -- R10 Load
    R10_LOAD                            = U128_ENUM(0b1ul, 4, 12),


    // bit 3 -- R10 Inc
    R10_INC                             = U128_ENUM(0b1ul, 3, 12),


    // bit 2 -- R10 Dec
    R10_DEC                             = U128_ENUM(0b1ul, 2, 12),


    // bit 1 -- R11 Load
    R11_LOAD                            = U128_ENUM(0b1ul, 1, 12),


    // bit 0 -- R11 Inc
    R11_INC                             = U128_ENUM(0b1ul, 0, 12),



    //---------------------------------------------------



    //
    // == CTRLD -- Control ROM 13
    //    =======================

    // bit 7 -- R11 Dec
    R11_DEC                             = U128_ENUM(0b1ul, 7, 13),


    // bit 6 -- R12 Load
    R12_LOAD                            = U128_ENUM(0b1ul, 6, 13),


    // bit 5 -- R12 Inc
    R12_INC                             = U128_ENUM(0b1ul, 5, 13),


    // bit 4 -- R12 Dec
    R12_DEC                             = U128_ENUM(0b1ul, 4, 13),


    // bit 3 -- DEV01 Load
    DEV01_LOAD                          = U128_ENUM(0b1ul, 3, 13),


    // bit 2 -- CTL01 Load
    CTL01_LOAD                          = U128_ENUM(0b1ul, 2, 13),


    // bit 1 -- DEV02 Load
    DEV02_LOAD                          = U128_ENUM(0b1ul, 1, 13),


    // bit 0 -- CTL02 Load
    CTL02_LOAD                          = U128_ENUM(0b1ul, 0, 13),



    //---------------------------------------------------



    //
    // == CTRLE -- Control ROM 14
    //    =======================

    // bit 7 -- DEV03 Load
    DEV03_LOAD                          = U128_ENUM(0b1ul, 7, 14),


    // bit 6 -- CTL03 Load
    CTL03_LOAD                          = U128_ENUM(0b1ul, 6, 14),


    // bit 5 -- DEV04 Load
    DEV04_LOAD                          = U128_ENUM(0b1ul, 5, 14),


    // bit 4 -- CTL04 Load
    CTL04_LOAD                          = U128_ENUM(0b1ul, 4, 14),


    // bit 3 -- DEV05 Load
    DEV05_LOAD                          = U128_ENUM(0b1ul, 3, 14),


    // bit 2 -- CTL05 Load
    CTL05_LOAD                          = U128_ENUM(0b1ul, 2, 14),


    // bit 1 -- DEV06 Load
    DEV06_LOAD                          = U128_ENUM(0b1ul, 1, 14),


    // bit 0 -- CTL06 Load
    CTL06_LOAD                          = U128_ENUM(0b1ul, 0, 14),



    //---------------------------------------------------



    //
    // == CTRLF -- Control ROM 15
    //    =======================

    // bit 7 -- DEV07 Load
    DEV07_LOAD                          = U128_ENUM(0b1ul, 7, 15),


    // bit 6 -- CTL07 Load
    CTL07_LOAD                          = U128_ENUM(0b1ul, 6, 15),


    // bit 5 -- DEV08 Load
    DEV08_LOAD                          = U128_ENUM(0b1ul, 5, 15),


    // bit 4 -- CTL08 Load
    CTL08_LOAD                          = U128_ENUM(0b1ul, 4, 15),


    // bit 3 -- DEV09 Load
    DEV09_LOAD                          = U128_ENUM(0b1ul, 3, 15),


    // bit 2 -- CTL09 Load
    CTL09_LOAD                          = U128_ENUM(0b1ul, 2, 15),


    // bit 1 -- DEV10 Load
    DEV10_LOAD                          = U128_ENUM(0b1ul, 1, 15),


    // bit 0 -- CTL10 Load
    CTL10_LOAD                          = U128_ENUM(0b1ul, 0, 15),



    //---------------------------------------------------



    //
    // == Improve code readability
    //    ========================
    FETCH_ASSERT_MAIN       = MAIN_BUS_ASSERT_FETCH | INSTRUCTION_SUPPRESS,
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

    const uint128_t nop = ADDR_BUS_1_ASSERT_PGMPC |  PGM_PC_INC; // Note that `| INSTRUCTION_ASSERT` == `| 0`, ∴ omitted
    uint128_t out = ADDR_BUS_1_ASSERT_PGMPC | PGM_PC_INC;

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

        return out | MAIN_BUS_ASSERT_NONE | R1_LOAD;


    case OPCODE_MOV_R2_RZ:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_BUS_ASSERT_NONE | R2_LOAD;


    case OPCODE_MOV_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_BUS_ASSERT_R1 | R2_LOAD;

    case OPCODE_MOV_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | MAIN_BUS_ASSERT_R2 | R1_LOAD;

    case OPCODE_ADD_R1___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_FETCH | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADD_R2___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_FETCH | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADD_R1_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_R1 | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADD_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_R2 | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADD_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_R1 | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADD_R2_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_0 | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_R2 | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R1___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_FETCH | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R2___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_FETCH | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R1_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_R1 | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R1_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_R2 | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R2_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_R1 | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_ADC_R2_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_LAST | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_R2 | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_INC_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_1 | ALU_BUS_A_ASSERT_R1 | ALU_BUS_B_ASSERT_NONE | MAIN_BUS_ASSERT_ALU_RESULT |
                R1_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_INC_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return out | CARRY_SELECT_1 | ALU_BUS_A_ASSERT_R2 | ALU_BUS_B_ASSERT_NONE | MAIN_BUS_ASSERT_ALU_RESULT |
                R2_LOAD | PGM_Z_LATCH | PGM_C_LATCH | PGM_N_LATCH | PGM_V_LATCH | PGM_L_LATCH;

    case OPCODE_JMP___16_:
        //
        // -- If we do not meet the condition, we do nothing and skip the next
        //    word in the instruction stream since it is a constant value
        //    ----------------------------------------------------------------
        if (!CONDITION_MET(flags)) return nop | INSTRUCTION_SUPPRESS;

        return FETCH_ASSERT_MAIN | PGM_PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PGMPC;

    case OPCODE_JMP_R1:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return MAIN_BUS_ASSERT_R1 | PGM_PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PGMPC;

    case OPCODE_JMP_R2:
        //
        // -- If we do not meet the condition, we do nothing
        //    ----------------------------------------------
        if (!CONDITION_MET(flags)) return nop;

        return MAIN_BUS_ASSERT_R2 | PGM_PC_LOAD | INSTRUCTION_SUPPRESS | ADDR_BUS_1_ASSERT_PGMPC;

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

    case 0xfff:
        return ~((uint128_t)0);
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
    FILE *ofc;
    FILE *ofd;
    FILE *ofe;
    FILE *off;

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

    ofc = fopen("ctrlc.bin", "w");
    if (!ofc) perror("Unable to open ctrlc.bin");

    ofd = fopen("ctrld.bin", "w");
    if (!ofd) perror("Unable to open ctrld.bin");

    ofe = fopen("ctrle.bin", "w");
    if (!ofe) perror("Unable to open ctrle.bin");

    off = fopen("ctrlf.bin", "w");
    if (!off) perror("Unable to open ctrlf.bin");


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
        uint8_t bytec = (promBuffer[i] >> 96) & 0xff;
        uint8_t byted = (promBuffer[i] >> 104) & 0xff;
        uint8_t bytee = (promBuffer[i] >> 112) & 0xff;
        uint8_t bytef = (promBuffer[i] >> 120) & 0xff;

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
        fwrite(&bytec, 1, sizeof(uint8_t), ofc);
        fwrite(&byted, 1, sizeof(uint8_t), ofd);
        fwrite(&bytee, 1, sizeof(uint8_t), ofe);
        fwrite(&bytef, 1, sizeof(uint8_t), off);
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
    fflush(ofc);
    fflush(ofd);
    fflush(ofe);
    fflush(off);


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
    fclose(ofc);
    fclose(ofd);
    fclose(ofe);
    fclose(off);
}



