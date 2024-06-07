# *16bcfs* Assembler

This note describes my thoughts for simplifying the 16-Bit Computer From Scratch (*16bcfs*) assembler.


## Why?

Fair question: Adam, you always start things over.  Why?

The genuine answer to this question is that I do not always start out with a complete plan and tend to make it up as I go.  Well, more to the point, I have a plan in my head but I do not take time to document it and think it all the way through.

Now that I have a failure (read that in air-quotes) under my belt, I have a foundation to look at to see where I can improve.


## Target

So, this version of the assembler is directly targeted to the *16bcfs* home-brew computer.  This system is a 16-bit computer with all operations happening on a 16-bit word.  This includes the memory organization where any access to memory is 16-bits wide and only on the 16-bit boundary.

This simple rule allows me to create a computer which can address 128K.


## Tools

The original assembler used `flex` as the scanner, but a recursive-descent parser which was hand-coded.  I honestly do not see a reason to change the approach.  The key difference is that I no longer will need to parse an architecture file to determine what is supported and can be hard-coded into the scanner and parser.

This will also mean that the assembly tables will be hard-coded into the assembler.  But, with a little effort, I might even be able to support a disassembler without too much trouble -- a stretch goal to be sure.


## Assembly Tables

This section will lay out the structure of the assembly tables.  These may be over-engineered for the *16bcfs*, as I do not have ambitions to go back and add a column to the table if it is not absolutely necessary.

This structure is considered a living entity:

```
struct {
    std::string mnemonic;
    int argCount;       // must be 3 or less
    enum argMask;       // 1 bit per arg, with 2 possible value (Const: 0b0, Reg: 0b1); msb first
    std::string a1Name; // populated when argMask == reg
    std::string a2Name;
    std::string a3Name;
    int binCount;       // # 16-bit words machine code
    enum binMask;       // 1 bit per word of output (static: 0b0, or constant value: 0b1); msb first
    uint16_t b1Val;     // populated when binMask bit == static
    uint16_t b2Val;
    uint16_t b3Val;
    uint16_t b4Val;
    uint16_t b5Val;
    uint16_t b6Val;
    uint16_t b7Val;
    uint16_t b8Val;
};
```

Such a record might look like this:

```
{"nop", 0, 0b00000000, "", "", "", 1, 0b00000000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{"jmp", 1, 0b00000000, "", "", "", 2, 0b01000000, 0x00f0, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{"mov", 2, 0b11000000, "r1", "r2", "", 1, 0b00000000, 0x0031, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
{"mov", 2, 0b10000001, "r1", "", "", 2, 0b01000000, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
```


### Some Notes:
1. While the longest instruction I expect to have is 2 words, I want to be able to support macros which may be several instructions grouped together.  This may require several 16-bit words more than just 2.


## Scanning (Lexical Analysis)

Since this assembler is now going to be specifically directed to the *16bcfs*, it is possible to get very granular in what the scanner is able to identify.  No longer will the parser have the requirement to determine if an identifier is a register or a label, the scanner will be able to take that responsiblity since the number of register will be finite and predetermined.

Such registers will be:

* RZ
* R1
* R2
* R3
* R4
* R5
* R6
* R7
* R8
* R9
* R10
* R11
* R12
* DEV01
* CTL01
* DEV02
* CTL02
* DEV03
* CTL03
* DEV04
* CTL04
* DEV05
* CTL05
* DEV06
* CTL06
* DEV07
* CTL07
* DEV08
* CTL08
* DEV09
* CTL09
* DEV10
* CTL10
* SP
* RA
* PC
* INTSP
* INTPC
* INTRA


Any identifier outside of these will be considered labels and will need to be resolved before the source is accepted.


## Parsing

A line of assembly code can be represented with the following rules:
* A line is termined with an EOL
* A line may be blank
* A line may contain a label followed by a colon
  * Start in the left-most column
  * Start with a letter or dot (`'.'`) character
  * May contain letters/digits to follow
  * A dot (`'.'`) label is an intermediate label, which is appended to the last proper label seen
* A line may contain an opcode mnemonic, which starts with at least 1 preceeding whitespace character
* A line with an opcode may contain 1 or more operands to the mnemonic, each operand separated by commas (`','`)
* A line may be contain a comment which is started with 2 slash characters (`'//'`) and continues to EOL

So, the rule might be:

```
[ [ ^<label>: ] [ <opcode> [ <operand1> [, <operand2> [, <operand3> ] ] ] ] [// <comment> ] ] <EOL>
```

With that, a match to the instruction table must include the following attributes:
* mnemonic
* number of parameters
* type of each parameter (numeric literal, register name, or (if neither of the previous 2) label -- which will be treated as a numeric literal when defined)
* register names for operands 1, 2, and 3


## Assembler Directives

Assemblers are expected to be able to be controlled through directives in the source.  The directives I am expecting to be able to support are:
* `%include` -- include another source file into the assembly as text in the current assembly file; this directive must start in column 1
* `%incbin` -- inlcude a binary file into the assembly as binary data; this directive must start in column 1
* `.org` -- set the memory location for the following code
* `.dw` -- Treat the following 16-bit word as data


## Output

This assembler is currently going to be a single-source assembler, meaning that everything is expected to appear in a single source file.  The use of the `%include` and `%incbin` directives will help modularize the source.

However, there is no facility for building intermediate object files (`*.o`) and linking them later.  This may be created at a later time, but it is not to be included in the initial versions of this assembler.  A self-hosted assembler may change this direction.

That said, the output will be a binary file only.  It will not support common executable formats such as ELF.  The entry point will be at address 0 in all cases.


## Endianness of the Output Binary

This assembler is targeted for everything to be represented in 16-bits.  However, the binary output will need to be split into 2 8-bit binaries and decorated with `msb` and `lsb` names.

With that, the only thing that matters is that when splitting the binary the endianness of the output is kept straight -- internal representation of the binary inside the assembler is less consequential.

See [This Web Page](https://stackoverflow.com/questions/2100331/macro-definition-to-determine-big-endian-or-little-endian-machine) for solutions on how to determine endianness at runtime.

