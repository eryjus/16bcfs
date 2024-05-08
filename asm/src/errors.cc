//===================================================================================================================
// errors.c -- functions for reporting errors, warnings, debugging information and other messages
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of the messaging output.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include "asm.hh"



//
// -- current number of errors and warnings.  At some point, some threshold will be exceeded
//    --------------------------------------------------------------------------------------
int Messaging::errorCount = 0;
int Messaging::warningCount = 0;

//
// -- the max number of errors before stopping (default is 1)
//    -------------------------------------------------------
int Messaging::maxErrors = 1;


//
// -- Treat warnings as errors?
//    -------------------------
bool Messaging::warningsAreErrors = false;



//
// -- Output a message to stdout
//    --------------------------
void Messaging::Output(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, va_list args)
{
    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    }

    vfprintf(stderr, msg, args);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    fflush(stderr);
}



//
// -- This is a fatal error.  Print the message and exit immediately.  No recovery is possible.
//    -----------------------------------------------------------------------------------------
void Messaging::Fatal(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...)
{
    fprintf(stderr, "FATAL ");

    va_list _list;
    va_start(_list, pos);
    Output(msg, file, lineNum, codeLine, pos, _list);
    va_end(_list);

    exit(EXIT_FAILURE);
}



//
// -- This is an error in assembling the binary.  Recovery is possible to check for more errors, but
//    no binary output file will be created.
//    ----------------------------------------------------------------------------------------------
void Messaging::Error(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...)
{
    fprintf(stderr, "ERROR ");

    va_list _list;
    va_start(_list, pos);
    Output(msg, file, lineNum, codeLine, pos, _list);
    va_end(_list);

    if (++ errorCount >= maxErrors) Fatal("Too many errors", file, lineNum, codeLine, pos);

    fflush(stderr);
}


//
// -- This is a warning that something might not be quite right.  Recovery is guaranteed and a binary
//    will be produced.  However, there is no guarantee that the output faithfully represents the intent.
//    ---------------------------------------------------------------------------------------------------
void Messaging::Warning(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...)
{
    fprintf(stderr, "WARNING ");

    va_list _list;
    va_start(_list, pos);
    Output(msg, file, lineNum, codeLine, pos, _list);
    va_end(_list);

    if (warningsAreErrors) {
        if (++ errorCount >= maxErrors) Fatal("Too many errors", file, lineNum, codeLine, pos);
    } else warningCount ++;

    fflush(stderr);
}


//
// -- This is informaiton only.
//    -------------------------
void Messaging::Info(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...)
{
    fprintf(stderr, "INFO ");

    va_list _list;
    va_start(_list, pos);
    Output(msg, file, lineNum, codeLine, pos, _list);
    va_end(_list);
}


//
// -- This is debugging information and is far more technical or detailed for the average user.
//    -----------------------------------------------------------------------------------------
void Messaging::Debug(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...)
{
    fprintf(stderr, "DEBUG ");

    va_list _list;
    va_start(_list, pos);
    Output(msg, file, lineNum, codeLine, pos, _list);
    va_end(_list);
}


//
// -- This is a simple output message.  Different than informational messages, Msg() does not reference the
//    source in any way.  These are general messages.
//    -----------------------------------------------------------------------------------------------------
void Messaging::Msg(const char *msg, ...)
{
    va_list _list;
    va_start(_list, msg);
    vfprintf(stdout, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stdout, "\n");
    }
    va_end(_list);

    fflush(stdout);
}


