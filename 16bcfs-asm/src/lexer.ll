/*******************************************************************************************************************/
/*  lexer.ll -- This is the scanner (lexical analyzer) for the assembler                                           */
/*                                                                                                                 */
/*      Copyright (c) 2024-2025  - Adam Clark                                                                            */
/*      License: Beerware                                                                                          */
/*                                                                                                                 */
/*      Date     Tracker  Version  Description                                                                     */
/*  -----------  -------  -------  ------------------------------------------------------------------------------- */
/*  17-May-2024  Initial           Initial Version                                                                 */
/*                                                                                                                 */
/*******************************************************************************************************************/


%{
    #include "asm.hh"
%}

%option yylineno
%option noyywrap
%option stack


%x      incl


WS              [ \t]
COMMENT         ;.*
NL              (\n|\r|\n\r|\r\n)


LETTER          [a-z_]
DIGIT           [0-9]
ALPHA           ({LETTER}|{DIGIT})
HEX             [0-9a-f]
OCT             [0-7]
BIN             [01]


SUFFIX          (-al|-nv|-eq|-ne|-cs|-cc|-mi|-pl|-vs|-vc|-hi|-ls|-ge|-lt|-gt|-le)


%%

{WS}+           {}
{COMMENT}       { Listing_t::AddComment(yytext); }
{NL}            { return TOK_EOL; }


0{OCT}*         { yylval.number = MakeOctal(yytext); return TOK_NUM; }
0b{BIN}+        { yylval.number = MakeBinary(yytext); return TOK_NUM; }
0x{HEX}+        { yylval.number = MakeHex(yytext); return TOK_NUM; }
[1-9]{DIGIT}*   { yylval.number = MakeDecimal(yytext); return TOK_NUM; }


rz              { yylval.name = yytext; return TOK_REG; }
r1              { yylval.name = yytext; return TOK_REG; }
r2              { yylval.name = yytext; return TOK_REG; }
r3              { yylval.name = yytext; return TOK_REG; }
r4              { yylval.name = yytext; return TOK_REG; }
r5              { yylval.name = yytext; return TOK_REG; }
r6              { yylval.name = yytext; return TOK_REG; }
r7              { yylval.name = yytext; return TOK_REG; }
r8              { yylval.name = yytext; return TOK_REG; }
r9              { yylval.name = yytext; return TOK_REG; }
r10             { yylval.name = yytext; return TOK_REG; }
r11             { yylval.name = yytext; return TOK_REG; }
r12             { yylval.name = yytext; return TOK_REG; }
dev01           { yylval.name = yytext; return TOK_REG; }
ctl01           { yylval.name = yytext; return TOK_REG; }
dev02           { yylval.name = yytext; return TOK_REG; }
ctl02           { yylval.name = yytext; return TOK_REG; }
dev03           { yylval.name = yytext; return TOK_REG; }
ctl03           { yylval.name = yytext; return TOK_REG; }
dev04           { yylval.name = yytext; return TOK_REG; }
ctl04           { yylval.name = yytext; return TOK_REG; }
dev05           { yylval.name = yytext; return TOK_REG; }
ctl05           { yylval.name = yytext; return TOK_REG; }
dev06           { yylval.name = yytext; return TOK_REG; }
ctl06           { yylval.name = yytext; return TOK_REG; }
dev07           { yylval.name = yytext; return TOK_REG; }
ctl07           { yylval.name = yytext; return TOK_REG; }
dev08           { yylval.name = yytext; return TOK_REG; }
ctl08           { yylval.name = yytext; return TOK_REG; }
dev09           { yylval.name = yytext; return TOK_REG; }
ctl09           { yylval.name = yytext; return TOK_REG; }
dev10           { yylval.name = yytext; return TOK_REG; }
ctl10           { yylval.name = yytext; return TOK_REG; }
sp              { yylval.name = yytext; return TOK_REG; }
ra              { yylval.name = yytext; return TOK_REG; }
pc              { yylval.name = yytext; return TOK_REG; }
intsp           { yylval.name = yytext; return TOK_REG; }
intra           { yylval.name = yytext; return TOK_REG; }
intpc           { yylval.name = yytext; return TOK_REG; }


adc{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
add{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
clc{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
decr{SUFFIX}?   { yylval.name = yytext; return TOK_OPCODE; }
inc{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
incr{SUFFIX}?   { yylval.name = yytext; return TOK_OPCODE; }
jmp{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
mov{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }
nop             { yylval.name = yytext; return TOK_OPCODE; }
stc{SUFFIX}?    { yylval.name = yytext; return TOK_OPCODE; }



^{LETTER}{ALPHA}*\:                 { yylval.name = yytext; return TOK_LABEL; }
^{LETTER}\.{DIGIT}+\:               { yylval.name = yytext; return TOK_LABEL; }
^{LETTER}{ALPHA}*\.{DIGIT}+\:       { yylval.name = yytext; return TOK_LABEL; }
^\.{DIGIT}+\:                       { yylval.name = yytext; return TOK_LABEL; }


{LETTER}{ALPHA}*\:                  { yylval.err = "Label must start in the first column"; return TOK_ERR; }
{LETTER}\.{DIGIT}+\:                { yylval.err = "Label must start in the first column"; return TOK_ERR; }
{LETTER}{ALPHA}*\.{DIGIT}+\:        { yylval.err = "Label must start in the first column"; return TOK_ERR; }
\.{DIGIT}+\:                        { yylval.err = "Label must start in the first column"; return TOK_ERR; }


{LETTER}{ALPHA}*                    { yylval.name = yytext; return TOK_IDENT; }
{LETTER}\.{DIGIT}+                  { yylval.name = yytext; return TOK_IDENT; }
{LETTER}{ALPHA}*\.{DIGIT}+          { yylval.name = yytext; return TOK_IDENT; }
\.{DIGIT}+                          { yylval.name = yytext; return TOK_IDENT; }


,                                   { return ','; }
'([^\n\r']|\\')+'                   { yylval.str = yytext; return TOK_STRING; }
'([^\n\r']|\\')*                    { yylval.err = "Unterminated string"; return TOK_ERR; }


^%include       { BEGIN(incl); }
^%incbin        { return TOK_INCBIN; }
\.org           { return TOK_ORG; }
\.dw            { return TOK_DATAWORD; }


<incl>{WS}*         {}
<incl>[^ \t\n\r]    {
        if (includePtr >= MAX_INCLUDE) {
            yylval.err = "Include depth too deep";
            return TOK_ERR;
        }

        includeStack[includePtr ++] = YY_CURRENT_BUFFER;

        yyin = fopen(yytext, "r");
        if (!yyin) {
            includePtr --;
            yylval.err = "Unable to open include file";
            return TOK_ERR;
        }

        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

        BEGIN(INITIAL);
    }

<<EOF>> {
        if (-- includePtr < 0) yyterminate();
        else {
            yy_delete_buffer(YY_CURRENT_BUFFER);
            yy_switch_to_buffer(includeStack[includePtr]);
        }
    }


.       { yylval.err = "Invalid character in source file"; return TOK_ERR; }


%%


//
// -- Global variables
//    ----------------
int includePtr = 0;
YY_BUFFER_STATE includeStack[MAX_INCLUDE];


//
// -- convert various string number formats to uint16_t
//    -------------------------------------------------
inline uint16_t MakeBinary(std::string n) { return (uint16_t)std::stol(n.substr(2), nullptr, 2); }
inline uint16_t MakeOctal(std::string n) { return (uint16_t)std::stol(n, nullptr, 8); }
inline uint16_t MakeDecimal(std::string n) { return (uint16_t)std::stol(n, nullptr, 10); }
inline uint16_t MakeHex(std::string n) { return (uint16_t)std::stol(n.substr(2), nullptr, 16); }


