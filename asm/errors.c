//===================================================================================================================
// errors.c -- functions for reporting errors, warnings, debugging information and other messages
//
//  These functions and variables are used to take care of the messaging output.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Feb-28  Initial  v0.0.1   Initial Version
//===================================================================================================================


#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//
// -- current number of errors and warnings.  At some point, some threshold will be exceeded
//    --------------------------------------------------------------------------------------
int errorCount = 0;
int warningCount = 0;

//
// -- the max number of errors before stopping (default is 1)
//    -------------------------------------------------------
int maxErrors = 1;


//
// -- Treat warnings as errors?
//    -------------------------
int warningsAreErrors = 0;



//
// -- This is a fatal error.  Print the message and exit immediately.  No recovery is possible.
//    -----------------------------------------------------------------------------------------
void Fatal(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "FATAL");

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

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    fflush(stderr);

    exit(EXIT_FAILURE);
}



//
// -- This is an error in assembling the binary.  Recovery is possible to check for more errors, but
//    no binary output file will be created.
//    ----------------------------------------------------------------------------------------------
void Error(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "ERROR");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    errorCount ++;

    if (errorCount >= maxErrors) {
        fprintf(stderr, "FATAL: Too many errors\n");
        exit(EXIT_FAILURE);
    }

    fflush(stderr);
}


//
// -- This is a warning that something might not be quite right.  Recovery is guaranteed and a binary
//    will be produced.  However, there is no guarantee that the output faithfully represents the intent.
//    ---------------------------------------------------------------------------------------------------
void Warning(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "WARNING");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    if (warningsAreErrors) {
        errorCount ++;

        if (errorCount >= maxErrors) {
            fprintf(stderr, "FATAL: Too many errors\n");
        }
    } else warningCount ++;

    fflush(stderr);
}


//
// -- This is informaiton only.
//    -------------------------
void Info(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "INFO");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    fflush(stderr);
}


//
// -- This is debugging information and is far more technical or detailed for the average user.
//    -----------------------------------------------------------------------------------------
void Debug(const char *msg,
            const char *file,
            int lineNum,
            const char *codeLine,
            int pos,
            ...)
{
    fprintf(stderr, "DEBUG");

    if (file) {
        fprintf(stderr, " (%s", file);

        if (lineNum > 0) {
            fprintf(stderr, ":%d", lineNum);

            if (pos > 0) {
                fprintf(stderr, ":%d", pos);
            }
        }

        fprintf(stderr, "):\n");
    } else fprintf(stderr, ":\n");

    va_list _list;
    va_start(_list, pos);
    vfprintf(stderr, msg, _list);

    if (msg[strlen(msg) - 1] != '\n') {
        fprintf(stderr, "\n");
    }
    va_end(_list);

    if (codeLine) {
        fprintf(stderr, "%s\n", codeLine);
    }

    fflush(stderr);
}


//
// -- This is a simple output message.  Different than informational messages, Msg() does not reference the
//    source in any way.  These are general messages.
//    -----------------------------------------------------------------------------------------------------
void Msg(const char *msg, ...)
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


