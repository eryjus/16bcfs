/*=================================================================================================================*/
/* lexer.ll -- This is the scanner for the assembler                                                               */
/*                                                                                                                 */
/*  This flex file defines the scanner which will be used to break the input into tokens for the assembler.  I     */
/*  used flex to simplify the tokenization and whitespace removal.  This scanner is broken up into several         */
/*  sub-scanners as follows:                                                                                       */
/*  * arch -- used to define the architecture for which to assemble                                                */
/*  * incl -- include any additional source members into the file to be assemble                                   */
/*  * path -- add a search path into list of paths used to search for includes or architectures                    */
/*  * opcode -- within the architecture definition, add an opcode to the architecture                              */
/*  * db -- within an opcode definition, these are the bytes to be output to the resulting binary file             */
/*  * data -- data bytes to be included in the resulting binary                                                    */
/*  * architecture -- when defining an architecture, several keywords are enabled (like register or memory)        */
/*                                                                                                                 */
/*      Date     Tracker  Version  Description                                                                     */
/*  -----------  -------  -------  ------------------------------------------------------------------------------- */
/*  2023-Feb-28  Initial  v0.0.1   Initial Version                                                                 */
/*  2023-Jun-26  Initial  v0.0.7   Add support for implied conditional execution opcodes                           */
/*                                                                                                                 */
/*=================================================================================================================*/


%{
    #include "asm.h"

    #define MAX_NESTED_INCLUDE  10
    YY_BUFFER_STATE includeStack[MAX_NESTED_INCLUDE];
    int lineNumberStack[MAX_NESTED_INCLUDE];
    char *sourceFileStack[MAX_NESTED_INCLUDE];
    char *currentSourceFile = "";

    int inclStackPtr = 0;

    int archDefined = 0;

    uint64_t ConvertNumber(int base, char *_s);

    #define HexNumber(yytext) ConvertNumber(16, yytext)
    #define OctNumber(yytext) ConvertNumber( 8, yytext)
    #define BinNumber(yytext) ConvertNumber( 2, yytext)
    #define DecNumber(yytext) ConvertNumber(10, yytext)
%}

%option yylineno
%option noyywrap
%option stack


%x              arch
%x              incl
%x              path

%x              opcode
%x              db
%x              data

%x              architecture


WS              [ \t]

COMMENT         \;.*
NL              (\n|\r|\n\r|\r\n)

LETTER          [a-z_]
DIGIT           [0-9]
ALPHA           ({LETTER}|{DIGIT})
HEX             [0-9a-f]
OCT             [0-7]
BIN             [01]


%%

<arch>{WS}                          {}
<arch>{COMMENT}                     {}
<arch>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<arch>[^ \t\n]+                     {
                                        if (inclStackPtr >= MAX_NESTED_INCLUDE) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Too many nested include files.\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno);

                                            exit(EXIT_FAILURE);
                                        }

                                        includeStack[inclStackPtr] = YY_CURRENT_BUFFER;
                                        lineNumberStack[inclStackPtr] = yylineno;
                                        sourceFileStack[inclStackPtr] = currentSourceFile;
                                        inclStackPtr ++;

                                        yyin = searchPath(yytext);

                                        if (!yyin) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Unable to locate include file \"%s\"\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno,
                                                    yytext);

                                            exit(EXIT_FAILURE);
                                        }

                                        currentSourceFile = strdup(yytext);
                                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                                        yylineno = 1;

                                        BEGIN(architecture);
                                    }



<incl>{WS}                          {}
<incl>{COMMENT}                     {}
<incl>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<incl>[^ \t\n]+                     {
                                        if (inclStackPtr >= MAX_NESTED_INCLUDE) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Too many nested include files.\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno);

                                            exit(EXIT_FAILURE);
                                        }

                                        includeStack[inclStackPtr] = YY_CURRENT_BUFFER;
                                        lineNumberStack[inclStackPtr] = yylineno;
                                        sourceFileStack[inclStackPtr] = currentSourceFile;
                                        inclStackPtr ++;

                                        yyin = searchPath(yytext);

                                        if (!yyin) {
                                            fprintf(stderr,
                                                    "In file %s on line %d: Unable to locate include file \"%s\"\n",
                                                    (currentSourceFile?currentSourceFile:""),
                                                    yylineno,
                                                    yytext);

                                            exit(EXIT_FAILURE);
                                        }

                                        currentSourceFile = strdup(yytext);
                                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                                        yylineno = 1;
                                    }


<path>{WS}                          {}
<path>{COMMENT}                     {}
<path>{NL}                          { BEGIN(INITIAL); return TOK_EOL; }
<path>[^ \t\n]+                     {
                                        char *buf = malloc(strlen(yytext) + 5);

                                        strcpy(buf, yytext);
                                        if (buf[strlen(buf) - 1] != '/') {
                                            int pos = strlen(buf);
                                            buf[pos] = '/';
                                            buf[pos + 1] = 0;
                                        }

                                        for (int i = 0; i < MAX_SEARCH_PATH; i ++) {
                                            if (!searchList[i]) {
                                                searchList[i] = buf;
                                                break;
                                            }
                                        }
                                    }




<architecture>{NL}                  { return TOK_EOL; }
<architecture>{WS}                  {}
<architecture>{COMMENT}             {}
<architecture>{LETTER}{ALPHA}*      {
                                        yylval.name = strdup(yytext);
                                        return TOK_ARCH_NAME;
                                    }
<architecture>{LETTER}{ALPHA}*-     {
                                        yylval.name = strdup(yytext);
                                        return TOK_ARCH_NAME_PREFIX;
                                    }
<architecture>-{LETTER}{ALPHA}*     {
                                        yylval.name = strdup(yytext);
                                        return TOK_ARCH_NAME_SUFFIX;
                                    }
<architecture>0[Xx]{HEX}+           {
                                        yylval.number = HexNumber(yytext + 2);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>0b{BIN}+              {
                                        yylval.number = BinNumber(yytext + 2);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>0{OCT}*               {
                                        yylval.number = OctNumber(yytext);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>{DIGIT}+              {
                                        yylval.number = DecNumber(yytext);
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>8bits                 {
                                        yylval.number = 8;
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>16bits                {
                                        yylval.number = 16;
                                        return TOK_ARCH_NUMBER;
                                    }
<architecture>,                     { return ','; }
<architecture>\(                    { return '('; }
<architecture>\)                    { return ')'; }
<architecture>\+                    { return '+'; }
<architecture>\-                    { return '-'; }

<architecture>^\.register           { return TOK_ARCH_REGISTER; }
<architecture>^\.memory             { return TOK_ARCH_MEMORY; }
<architecture>^\.opcode             { yy_push_state(opcode); return TOK_ARCH_OPCODE; }
<architecture>^\.organization       { return TOK_ARCH_ORGANIZATION; }
<architecture>^\.big-endian         { return TOK_ARCH_BIG_ENDIAN; }
<architecture>^\.little-endian      { return TOK_ARCH_LITTLE_ENDIAN; }
<architecture>^\.cond-bits          { return TOK_ARCH_COND_BITS; }
<architecture>^\.cond-default       { return TOK_ARCH_COND_DEFAULT; }
<architecture>^\.cond-prefix        { return TOK_ARCH_COND_PREFIX; }
<architecture>^\.cond-suffix        { return TOK_ARCH_COND_SUFFIX; }


<architecture>\.db                  { yy_push_state(db); return TOK_OPCODE_DB; }
<architecture>\.mc                  { yy_push_state(db); return TOK_OPCODE_MC; }

<architecture>.                     {
                                        yylval.errorMsg = "Unexpected character in .architecture definition file";
                                        return TOK_ERROR;
                                    }


<opcode>{WS}                        { }
<opcode>[^.\n]+                     {
                                        yylval.name = strdup(yytext);
                                        yy_pop_state();
                                        return TOK_OPCODE_DEF;
                                    }
<opcode>{NL}                        {
                                        yylval.errorMsg = "Unexpected newline in .opcode definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }
<opcode>.                           {
                                        yylval.errorMsg = "Unexpected character in .opcode definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }



<db>{COMMENT}                       {}
<db>[^;\n]*                         {
                                        yylval.name = strdup(yytext);
                                        yy_pop_state();
                                        return TOK_BYTE_STREAM_DEF;
                                    }
<db>{NL}                            {
                                        yylval.errorMsg = "Unexpected newline in .db byte code definition";
                                        yy_pop_state();
                                        return TOK_ERROR;
                                    }


<data>{WS}                          {}
<data>{COMMENT}                     {}
<data>{NL}                          { BEGIN(INITIAL); }
<data>{HEX}+                        {
                                        yylval.number = HexNumber(yytext);
                                        return TOK_NUMBER;
                                    }
<data>.                             {
                                        Error("Unexpected character: %c\n",
                                                currentSourceFile,
                                                yylineno,
                                                0,
                                                0,
                                                yytext[0]);

                                        yylval.errorMsg = "Unexpected character";
                                        return TOK_ERROR;
                                    }


{NL}                                { return TOK_EOL; }
{WS}                                {}
{COMMENT}.*                         {}

^\.include                          { BEGIN(incl); }
^\.path                             { BEGIN(path); }
^\.arch                             { BEGIN(arch); }

^{LETTER}{ALPHA}+\:                 {
                                        yylval.name = strdup(yytext);
                                        return TOK_LABEL;
                                    }
{LETTER}[^;\n]*                     {
                                        yylval.name = strdup(yytext);
                                        return TOK_INSTRUCTION;
                                    }

\.org                               { return TOK_ORG; }

0[x]{HEX}+                          {
                                        yylval.number = HexNumber(yytext + 2);
                                        return TOK_NUMBER;
                                    }

\.db                                { BEGIN(data); return TOK_DB; }

<<EOF>>                             {
                                        -- inclStackPtr;

                                        if (inclStackPtr < 0) {
                                            yyterminate();
                                        } else {
                                            yy_delete_buffer(YY_CURRENT_BUFFER);
                                            yy_switch_to_buffer(includeStack[inclStackPtr]);
                                            yylineno = lineNumberStack[inclStackPtr];
                                            currentSourceFile = sourceFileStack[inclStackPtr];

                                            if (sourceFileStack[inclStackPtr]) {
                                                // TODO: should the pointer be freed?
                                                sourceFileStack[inclStackPtr] = 0;
                                            }
                                        }

                                        BEGIN(INITIAL);
                                    }

.                                   {
                                        Error("Unexpected character: %c\n",
                                                currentSourceFile,
                                                yylineno,
                                                0,
                                                0,
                                                yytext[0]);

                                        yylval.errorMsg = "Unexpected character";
                                        return TOK_ERROR;
                                    }



%%

void OpenFile (const char *file)
{
    extern FILE *yyin;

    yyin = fopen(file, "r");

    if (!yyin) {
        fprintf(stderr, "Unable to open source file\n");
        exit(EXIT_FAILURE);
    }

    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    yylineno = 1;
}


uint64_t ConvertNumber(int base, char *_s)
{
    char *s = _s;
    uint64_t rv = 0;

    while (*s) {
        *s = tolower(*s);

        int d;

        if (*s >= '0' && *s <= '9') {
            d = *s - '0';
        } else if (*s >= 'a' && *s <= 'f') {
            d = *s - 'a' + 10;
        } else if (*s >= 'A' && *s <= 'F') {
            d = *s - 'A' + 10;
        } else {
            fprintf(stderr, "There was an error in the scanner parsing a number\n");
            exit(EXIT_FAILURE);
        }

        rv = rv * base + d;
        s ++;
    }

    return rv;
}
