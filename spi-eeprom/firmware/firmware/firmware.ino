//===================================================================================================================
//  firmware.ino -- Arduino sketch to program an SPI Serial EEPROM
//
//      Copyright (c) 2023-2025 - Adam Clark
//      License: Beerware
//
//  This sketch will be used to ferret out the details for programming a 256 Kbit EEPROM using the SPI protocol.
//
//  The following items are things to be able to support at a high level:
//  * (L) Lock the EEPROM by setting the WPEN bit
//  * (U) Unlock the EEPROM by clearing the WPEN bit
//  * (B) Block Protect the EEPROM to 1 of 4 states by setting a Block Protect Value:
//    * 0: No Protection
//    * 1: Protect Upper Quarter EEPROM
//    * 2: Protect Upper Half EEPROM
//    * 3: Protect Entire EEPROM
//  * (F) Fill the EEPROM (or range) with a value
//  * (P) Poke a value into the EEPROM at a specified address
//  * (D) Dump EEPROM Page (running address or range)
//  * (Z) Write a test pattern into the EEPROM and verify it can be read
//  * (W) Program Entire EEPROM
//  * (V) Verify Entire EEPROM against binary
//  * (R) Read Entire EEPROM
//  * (S) EEPROM Status
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2023-Dec-19  Initial  v0.0.1   Initial version
//===================================================================================================================



#include "HardwareSerial.h"


//
// -- some parameters that define the EEPROM we are dealing with
//    ----------------------------------------------------------
#define PAGE_SIZE 64
#define EEPROM_BYTES 32768



//
// -- define the pins we are going to use
//    -----------------------------------
#define   CS 2
#define MISO 3
#define MOSI 4
#define SCLK 5



//
// -- the EEPROM recognizes the following commands
//    --------------------------------------------
#define READ  0b00000011
#define WRITE 0b00000010
#define WRDI  0b00000100
#define WREN  0b00000110
#define RDSR  0b00000101
#define WRSR  0b00000001



//
// -- a global variable used for which page to dump
//    ---------------------------------------------
int dp = 0;



//
// -- Print the Welcome Message to the serial port
//    --------------------------------------------
void Welcome() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Welcome to the SPI 256 Kbit EEPROM Programmer"));
  Serial.println(F("  Copyright (c) 2023-2025 -- Adam Clark under the Beerware License"));
  Serial.println(F("  See https://github.com/eryjus/16bcfs/blob/master/LICENSE.md for complete details"));
  Serial.println(F("=================================================================================="));
}



//
// -- Print the help to the serial monitor
//    ------------------------------------
void PrintHelp() {
  Serial.println();
  Serial.println(F("Available Commands:"));
  Serial.println(F("\tS\t\tDecode the EEPROM STATUS register and print the contents"));
  Serial.println(F("\tL\t\tLock the EEPROM by setting the WPEN bit"));
  Serial.println(F("\tU\t\tUnlock the EEPROM by clearing the WPEN bit"));
  Serial.println(F("\tZ<xxxx>\t\tWrite a test pattern to a page, and verify it"));
  Serial.println(F("\tD<xxxx>\t\tDump a page to the serial port"));
  Serial.println(F("\tBbb\t\tSet the Block Protect Option: (00=None; 01=Upper Quarter; 10=Upper Half; 11=All"));
  Serial.println(F("\tFxx\t\tFill the EEPROM with the specified value"));
  Serial.println(F("\tPxx xxxx\tPoke a value into the EEPROM memory"));
  Serial.println(F("\tW\t\tWrite (Program) an entire EEPROM"));
  Serial.println(F("\tR\t\tRead an entire EEPROM"));
  Serial.println(F("\tV\t\tVerify the Contents of an EEPROM against the selected binary"));
  Serial.println();
  Serial.print("> ");
}



//
// -- read a command from the serial port
//    -----------------------------------
String ReadCommand() {
  Serial.setTimeout(2147483647);    // -- wait about 24 days for input
  return Serial.readStringUntil('\n');
}



//
// -- Decode and present the contents of the status register
//    ------------------------------------------------------
void PrintStatus() {
  byte stat = GetStatus();
  Serial.println(F("EEPROM Status:"));
  Serial.print(F("   WPEN\t"));  Serial.println((stat & 0x80) != 0 ? F("Enabled") : F("Disabled"));
  Serial.print(F("   BP  \t"));  if (((stat >> 2) & 0x3) < 1) { Serial.print('0'); } Serial.println((stat >> 2) & 0x3, BIN);
  Serial.print(F("   WEL \t"));  Serial.println((stat & 0x02) != 0 ? F("Latched") : F("Unlatched"));
  Serial.print(F("   WIP \t"));  Serial.println((stat & 0x01) != 0 ? F("True") : F("False"));
  Serial.println();
  Serial.print(F("The current Page Location is 0x")); Serial.println(dp, HEX);
  Serial.println();
  Serial.println(F("WPEN = Write Protect ENable"));
  Serial.println(F("BP   = Block Protect (00=None; 01=Upper Quarter; 10=Upper Half; 11=All"));
  Serial.println(F("WEL  = Write Enable Latch"));
  Serial.println(F("WIP  = Write In Progress"));
}



//
// -- Lock the EEPROM by setting the WPEN bit in the STATUS register
//    --------------------------------------------------------------
void LockEeprom() {
  byte stat = GetStatus();
  stat |= 0x80;         // -- set the WPEN bit
  stat &= ~0x03;        // -- clear the read only bits

  // -- Now we nee to set the WEL
  WriteStatus(stat);

  Serial.println(F("Locked"));
}


//
// -- Unlock the EEPROM by clearing the WPEN bit in the STATUS register
//    -----------------------------------------------------------------
void UnlockEeprom() {
  byte stat = GetStatus();
  stat &= ~0x80;        // -- clear the WPEN bit
  stat &= ~0x03;        // -- clear the read only bits

  // -- Now we nee to set the WEL
  WriteStatus(stat);

  Serial.println(F("Unlocked"));
}



//
// -- Set the Block Protect option
//    ----------------------------
void BlockProtect(int p) {
  p &= 0x3;
  byte stat = GetStatus();
  stat &= ~0x0f;        // -- clear the read only and these bits
  stat |= (p << 2);            // -- set the new value

  WriteStatus(stat);

  Serial.print(F("Block Protect set to "));
  if (p < 1) Serial.print('0');
  Serial.println(p, BIN);
}



//
// -- Check the EEPROM lock
//    ---------------------
bool CheckLock(void) {
  byte stat = GetStatus();
  return ((stat & 0x80) == 0 ? true : false);
}



//
// -- Write a byte ti the specified location in the EEPROM
//    ----------------------------------------------------
void WriteByte(int loc, byte val) {
  SetWEL();
  Start();
  ShiftByte(WRITE);
  ShiftByte((loc >> 8) & 0xff);
  ShiftByte(loc & 0xff);
  ShiftByte(val);
  Finish();
}



//
// -- Write the page buffer into the EEPROM
//    -------------------------------------
void WritePage(int loc, byte *buffer, int size) {
  SetWEL();
  Start();
  ShiftByte(WRITE);
  ShiftByte((loc >> 8) & 0xff);
  ShiftByte(loc & 0xff);

  for (int i = 0; i < size; i ++) {
    ShiftByte(buffer[i]);
  }
  Finish();
}



//
// -- Verify a page is correct
//    ------------------------
bool VerifyPage(int loc, byte *buffer, int size) {
  bool rv = true;

  Start();
  ShiftByte(READ);
  ShiftByte((loc >> 8) & 0xff);
  ShiftByte(loc & 0xff);

  for (int i = 0; i < size; i ++) {
    if (ReadByte() != buffer[i]) {
      Serial.print(F("FAIL: bytes do not match at location0x"));  Serial.println(loc + i, HEX);
      rv = false;
    }
  }

  Finish();
  return rv;
}



//
// -- Read a page's contents
//    ----------------------
void ReadPage(long loc, byte *array)
{
  Start();
  ShiftByte(READ);
  ShiftByte((loc >> 8) & 0xff);
  ShiftByte(loc & 0xff);

  for (int i = 0; i < PAGE_SIZE; i ++) {
    array[i] = ReadByte();
  }

  Finish();
}



//
// -- Dump a page's contents
//    ----------------------
void DumpPage(int loc) {
  Start();
  ShiftByte(READ);
  ShiftByte((loc >> 8) & 0xff);
  ShiftByte(loc & 0xff);

  Serial.print(F("Dumping the contents of the EEPROM at 0x")); Serial.println(loc, HEX);
  Serial.println();
  Serial.println("Loc   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F");
  Serial.print  ("----- -----------------------------------------------");

  for (int i = 0; i < PAGE_SIZE; i ++) {
    if ((i & 15) == 0) {
      Serial.println();

      int l = loc + i;

      if (l < 4096) Serial.print('0');
      if (l < 256)  Serial.print('0');
      if (l < 16)   Serial.print('0');

      Serial.print(loc + i, HEX);
      Serial.print(": ");
    }

    byte val = ReadByte();

    if (val < 16) Serial.print('0');
    Serial.print(val, HEX);
    Serial.print(" ");
  }

  Serial.println();

  Finish();
}



//
// -- Write a test pattern to the EEPROM, and then verify it was written properly
//    ---------------------------------------------------------------------------
void TestPattern(int loc) {
  loc &= 0x7fff;      // -- clear MSbit
  loc &= ~(PAGE_SIZE-1);         // -- align to page size

  if (!CheckLock()) {
    Serial.println("FAIL: EEPROM is locked; please unlock it first");
    return;
  }

  byte pattern[PAGE_SIZE] = { F("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\0") };

  SetWEL();
  WritePage(loc, pattern, PAGE_SIZE);
  bool result = VerifyPage(loc, pattern, PAGE_SIZE);

  if (result) {
    Serial.print(F("SUCCESS: Test pattern successfully written to and verified at location 0x"));
    Serial.println(loc, HEX);
  } else {
    Serial.print(F("FAIL: Unable to verify the test pattern."));
    Serial.print(F("  (see previous messages for more details)"));
  }
}



//
// -- Fill the EEPROM with the byte value
//    -----------------------------------
void Fill(byte val) {
  byte buffer[PAGE_SIZE];
  memset(buffer, val, PAGE_SIZE);

  Serial.print(F("Filling EEPROM with the byte value 0x"));
  if (val < 16) Serial.print('0');
  Serial.println(val, HEX);

  Serial.print(F("0000: "));

  for (long i = 0; i < EEPROM_BYTES; i += PAGE_SIZE) {
    if ((i & 0x7ff) == 0 && i != 0) {
      Serial.println();
      if (i < 0x1000) Serial.print('0');
      Serial.print(i, HEX);
      Serial.print(F(": "));
    }
    Serial.print('.');
    WritePage(i, buffer, PAGE_SIZE);
  }

  Serial.println();
  Serial.print(F("Filled the EEPROM with the value 0x"));
  if (val < 16) Serial.print('0');
  Serial.println(val, HEX);
}



//
// -- Enter Write Mode, program an entire EEPROM
//    ------------------------------------------
void Write(void) {
  // -- send 3 breaks (well, really ETX) characters to get the client's attention
  Serial.println("Please select the file to program to the EEPROM");
  Serial.print(F("\x03\x03\x03"));
  Serial.flush();

  // -- make sure we get an ACK back in a reasonable amount of time (0.5 sec should do)
  Serial.setTimeout(500);
  byte inp[2];
  if (Serial.readBytes(inp, 1) == 0) return;
  if (inp[0] != '\x06') return;

  // -- now we need to inform the client that we are executing a WRITE
  Serial.print(F("W"));
  Serial.flush();

  while (Serial.readBytes(inp, 1) == 0) {}    // wait forever for a response
  if (inp[0] != '\x06') return;               // check for an ACK

  byte buf[PAGE_SIZE];
  byte wrk[1];

  for (long i = 0; i < EEPROM_BYTES; i += PAGE_SIZE) {
    digitalWrite(LED_BUILTIN, HIGH);

    for (int j = 0; j < PAGE_SIZE; j ++) {
      while (Serial.available() < 1) {}          // wait for a page to be available

      Serial.readBytes(wrk, 1);
      buf[j] = wrk[0];
    }

    digitalWrite(LED_BUILTIN, LOW);

    WritePage(i, buf, PAGE_SIZE);

    Serial.print(F("\x06"));
    Serial.flush();
  }
}



//
// -- Enter Read Mode, read an entire EEPROM
//    --------------------------------------
void Read(void) {
  // -- send 3 breaks (well, really ETX) characters to get the client's attention
  Serial.println("Please select the file to save the EEPROM program");
  Serial.print(F("\x03\x03\x03"));
  Serial.flush();

  // -- make sure we get an ACK back in a reasonable amount of time (0.5 sec should do)
  Serial.setTimeout(500);
  byte inp[2];
  if (Serial.readBytes(inp, 1) == 0) return;
  if (inp[0] != '\x06') return;

  // -- now we need to inform the client that we are executing a WRITE
  Serial.print(F("R"));
  Serial.flush();

  while (Serial.readBytes(inp, 1) == 0) {}    // wait forever for a response
  if (inp[0] != '\x06') return;               // check for an ACK

  byte buf[PAGE_SIZE];
  byte wrk[1];

  for (long i = 0; i < EEPROM_BYTES; i += PAGE_SIZE) {
    digitalWrite(LED_BUILTIN, HIGH);
    ReadPage(i, buf);
    digitalWrite(LED_BUILTIN, LOW);

    for (int j = 0; j < PAGE_SIZE; j ++) {
      // -- need to be real clear about which version of `Serial.print()` is called!
      Serial.print((char)buf[j]);
    }

    while (Serial.readBytes(inp, 1) == 0) {}    // wait forever for a response
    if (inp[0] != '\x06') return;               // check for an ACK
  }
}



//
// -- Enter Verify Mode, read an entire EEPROM
//    ----------------------------------------
void Verify(void) {
  // -- send 3 breaks (well, really ETX) characters to get the client's attention
  Serial.println("Please select the file to save the EEPROM program");
  Serial.print(F("\x03\x03\x03"));
  Serial.flush();

  // -- make sure we get an ACK back in a reasonable amount of time (0.5 sec should do)
  Serial.setTimeout(500);
  byte inp[2];
  if (Serial.readBytes(inp, 1) == 0) return;
  if (inp[0] != '\x06') return;

  // -- now we need to inform the client that we are executing a WRITE
  Serial.print(F("V"));
  Serial.flush();

  while (Serial.readBytes(inp, 1) == 0) {}    // wait forever for a response
  if (inp[0] != '\x06') return;               // check for an ACK

  byte inBuf[PAGE_SIZE];
  byte romBuf[PAGE_SIZE];
  byte wrk[1];

  for (long i = 0; i < EEPROM_BYTES; i += PAGE_SIZE) {
    digitalWrite(LED_BUILTIN, HIGH);

    for (int j = 0; j < PAGE_SIZE; j ++) {
      while (Serial.available() < 1) {}          // wait for a page to be available

      Serial.readBytes(wrk, 1);
      inBuf[j] = wrk[0];
    }

    digitalWrite(LED_BUILTIN, LOW);

    bool good = true;
    ReadPage(i, romBuf);

    for (int j = 0; j < PAGE_SIZE; j ++) {
      if (inBuf[j] != romBuf[j]) good = false;
    }

    Serial.print(good?F("\x06"):F("\x15"));
    Serial.flush();
  }
}



//
// -- start a conversation with the EEPROM
//    ------------------------------------
void Start() {
  delay(2);
  digitalWrite(CS, LOW);
  delay(1);
}


//
// -- finish a conversation with the EEPROM
//    -------------------------------------
void Finish() {
  delay(1);
  digitalWrite(CS, HIGH);
  delay(5);
}


//
// -- send a bit to the SPI interface and read a bit at the same time
//
//    NOTE: I am not likely to do both actions at the same time in this code,
//          but the hardware does support full duplex communications, so
//          this code needs to be able to support it.
//    ------------------------------------------------------------------------
bool XchgBit(bool bit) {
  bool rv = digitalRead(MISO);

  if (bit) digitalWrite(MOSI, HIGH);
  else digitalWrite(MOSI, LOW);

  digitalWrite(SCLK, HIGH);
  digitalWrite(SCLK, LOW);

  return (rv == HIGH);
}



//
// -- This function will send out a byte to the slave device (ignoring anything read)
//    -------------------------------------------------------------------------------
void ShiftByte(byte val) {
  XchgBit((val & 0x80) != 0);
  XchgBit((val & 0x40) != 0);
  XchgBit((val & 0x20) != 0);
  XchgBit((val & 0x10) != 0);
  XchgBit((val & 0x08) != 0);
  XchgBit((val & 0x04) != 0);
  XchgBit((val & 0x02) != 0);
  XchgBit((val & 0x01) != 0);
}



//
// -- This function will read a byte from the slave device (sending all 0s)
//    ---------------------------------------------------------------------
byte ReadByte() {
  byte rv = 0;

  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);

  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);
  rv = (rv << 1) | (XchgBit(0) == HIGH ? 1 : 0);

  return rv;
}



//
// -- READ the STATUS register
//    ------------------------
byte GetStatus() {
  Start();
  ShiftByte(RDSR);
  byte sr = ReadByte();
  Finish();
  return sr;
}



//
// -- Set the WEL (Write Enable Latch)
//    --------------------------------
void SetWEL() {
  Start();
  ShiftByte(WREN);
  Finish();
}


//
// -- WRITE the STATUS register
//    -------------------------
void WriteStatus(byte st) {
  // -- turn on write enable
  Start();
  SetWEL();
  Finish();

  // -- write the status bit
  Start();
  ShiftByte(WRSR);
  ShiftByte(st);
  Finish();
}






//
// -- standard function to perform the 1-time setup on the MCU
//    --------------------------------------------------------
void setup() {
  // -- first thing: disable the EEPROM
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);

  // -- perform additional configuration
  pinMode(MISO, INPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SCLK, OUTPUT);
  digitalWrite(SCLK, LOW);
  digitalWrite(MOSI, LOW);


  pinMode(LED_BUILTIN, OUTPUT);


  // -- initialize global variables
  dp = 0;

  // -- print the welcome message
  Welcome();
}


//
// -- convert a hex string to an integer
//    ----------------------------------
int xtoi(String &hex) {
  int rv = 0;

  for (int i = 0; i < hex.length(); i ++) {
    if (hex[i] >= '0' && hex[i] <= '9') {
      rv = rv * 16 + (hex[i] - '0');
      hex[i] = ' ';
    } else if (hex[i] >= 'A' && hex[i] <= 'F') {
      rv = rv * 16 + (hex[i] - 'A' + 10);
      hex[i] = ' ';
    } else return rv;
  }

  return rv;
}



//
// -- the standard loop code, executed with each iteration
//    ----------------------------------------------------
void loop() {
  PrintHelp();
  String cmd = ReadCommand();
  cmd.toUpperCase();
  cmd.trim();
  Serial.println(cmd);

  if (cmd == F("S")) {
    PrintStatus();

    return;
  }

  if (cmd == F("L")) {
    LockEeprom();

    return;
  }

  if (cmd == F("U")) {
    UnlockEeprom();

    return;
  }

  if (cmd[0] == 'Z') {
    if (cmd == F("Z")) TestPattern(0);   // this is the whole command, take the defaults
    else {
      cmd[0] = ' ';
      cmd.trim();
      int loc = xtoi(cmd);
      loc &= ~(PAGE_SIZE-1);                     // align to page size
      TestPattern(loc);
    }

    dp = 0;

    return;
  }

  if (cmd[0] == 'D') {
    if (cmd == F("D")) DumpPage(dp);      // this is the whole command, take the defaults
    else {
      cmd[0] = ' ';
      cmd.trim();
      int loc = xtoi(cmd);
      loc &= ~(PAGE_SIZE-1);                     // align to page size
      DumpPage(loc);
    }

    dp += PAGE_SIZE;

    return;
  }

  if (cmd == F("B00")) {
    BlockProtect(0b00);

    return;
  }

  if (cmd == F("B01")) {
    BlockProtect(0b01);

    return;
  }

  if (cmd == F("B10")) {
    BlockProtect(0b10);

    return;
  }

  if (cmd == F("B11")) {
    BlockProtect(0b11);

    return;
  }

  if (cmd[0] == 'F') {
    cmd[0] = ' ';
    cmd.trim();

    byte val = xtoi(cmd);
    Fill(val);

    dp = 0;

    return;
  }

  if (cmd[0] == 'P') {
    cmd[0] = ' ';
    cmd.trim();
    byte val = (byte)(xtoi(cmd) & 0xff);
    cmd.trim();
    int loc = xtoi(cmd);

    WriteByte(loc, val);

    dp = loc & ~(PAGE_SIZE-1);

    return;
  }

  if (cmd == F("W")) {
    Write();
    Serial.setTimeout(2147483647);    // -- restore timeout unconditionally
    return;
  }


  if (cmd == F("R")) {
    Read();
    Serial.setTimeout(2147483647);    // -- restore timeout unconditionally
    return;
  }


  if (cmd == F("V")) {
    Verify();
    Serial.setTimeout(2147483647);    // -- restore timeout unconditionally
    return;
  }
}
