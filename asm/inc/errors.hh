//===================================================================================================================
// errors.hh -- functions for reporting errors, warnings, debugging information and other messages
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//  These functions and variables are used to take care of the messaging output.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-11  Initial  v0.0.1   Initial Version
//===================================================================================================================


#pragma once

#ifdef __cplusplus


//
// -- this is a static class which will be used to output diagnotic messaging
//    -----------------------------------------------------------------------
class Messaging {
    Messaging() = delete;
    Messaging(const Messaging &) = delete;

private:
    static int errorCount;
    static int warningCount;
    static int maxErrors;
    static bool warningsAreErrors;


public:
    static void SetMaxErrors(int m) { maxErrors = m; }
    static void SetWarningsAreErrors(bool w) { warningsAreErrors = w; }


private:
    static void Output(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, va_list args);


public:
    static __attribute__((noreturn)) void Fatal(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...);
    static void Error(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...);
    static void Warning(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...);
    static void Info(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...);
    static void Debug(const char *msg, const char *file, int lineNum, const char *codeLine, int pos, ...);
    static void Msg(const char *msg, ...);
};

#endif


