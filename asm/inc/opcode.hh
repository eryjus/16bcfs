//===================================================================================================================
// opcode.hh -- functions for managing the opcodes supported by the architecture
//
//      Copyright (c) 2023-2024 - Adam Clark
//      License: Beerware
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Jan-12  Initial  v0.0.1   Initial Version
//===================================================================================================================



class OpCodes {
    OpCodes(const OpCodes &) = delete;
    OpCodes &operator=(const OpCodes &) = delete;


private:
    class OpCode_t {
    public:
        std::string opcodeDef;
        std::string byteStream;

    public:
        OpCode_t(const std::string &d, const std::string &s) : opcodeDef(d), byteStream(s) {}
        virtual ~OpCode_t() {}
    };


private:
    static OpCode_t *opcodeList[MAX_OPCODES];
    static std::string enumString;
    static OpCodes *singleton;


private:
    OpCodes(void) {}
    virtual ~OpCodes() {}


protected:
    static int hex2int(char h);
    static int ParseNumber(const std::string &s, uint64_t *val, int base);
    static OpCode_t *FindInstruction(const std::string &instr, uint64_t *val);


protected:
    static inline int ParseDecimal(const std::string &s, uint64_t *v) { return ParseNumber(s, v, 10); }
    static inline int ParseBinary(const std::string &s, uint64_t *v) { return ParseNumber(std::string(s.c_str() + 2), v, 2) + 2; }
    static inline int ParseOctal(const std::string &s, uint64_t *v) { return ParseNumber(s, v, 8); }
    static inline int ParseHex(const std::string &s, uint64_t *v) { return ParseNumber(std::string(s.c_str() + 2), v, 16) + 2; }


public:
    static OpCodes *Get(void) { if (!singleton) singleton = new OpCodes; return singleton; }
    static void AddEnum(const std::string &def, const std::string &bytes);
    static void OutputEnum(void);
    static void Add(const std::string &def, const std::string &bytes);
    static void Dump(void);
    static std::string NormalizeInstruction(const std::string &line);
    static void ParseInstruction(const std::string &line);
};



