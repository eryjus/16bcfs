//===================================================================================================================
//  ctrl-rom-test.ino -- Using an ESP32, perform tests on the EEPROM
//
//  Ok, here is a list of pins I need in order to implement this test:
//  ------------------------------------------------------------------
//   0-14:  These are the address pins for the SRAM
//  15-16:  Rx/Tx pins for the serial port
//     17:  Clock for latching the SRAM contents for review
//  18-25:  The 8-bit payload
//     26:  Reset hold
//
//  Without testing each pin to confirm each can be used, the high water mark for the number of pins available is 
//  25.  25 at best.  This circuit needs 27.
//
//  So, we need to use a shifter to shift some bits -- either in or out.  The best bang for the buck is to shift 
//  out the address value to read.  This trades 15 pins for 3: 
//  * shift clock
//  * data pin
//  * clear signal
//
//  The result is 15 pins required to make this circuit work.
//
//      Date     Tracker  Version  Description
//  -----------  -------  -------  ---------------------------------------------------------------------------------
//  2024-Mar-13  Initial  v0.0.1   Initial version
//
//===================================================================================================================


#include "incbin.h"


extern unsigned char ______control_logic_ctrl0_bin[];


//
// -- Which pins are relevant to us?  (by GPIO)
//    -----------------------------------------
#define BIT7 36       // input bit 7
#define BIT6 39       // input bit 6
#define BIT5 34       // input bit 5
#define BIT4 35       // input bit 4
#define BIT3 32       // input bit 3
#define BIT2 33       // input bit 2
#define BIT1 25       // input bit 1
#define BIT0 26       // input bit 0

#define RHLD 13       // Reset Hold input


<<<<<<< Updated upstream
//
// -- Output Pins
//    -----------
#define LTCH 22       // Latch Clock Output
=======
//#define CLK  22       // Latch Clock Output
>>>>>>> Stashed changes
#define SHFT 18       // Shift Clock Output
#define ADDR 19       // Address Pin Output
#define CLR  21       // Shifter Clear Signal Output
#define RST  5        // Reset button
<<<<<<< Updated upstream
//#define CLK  17       // The main clock
=======
#define CLK  17       // The main clock
>>>>>>> Stashed changes


#define LED_BUILTIN 2


//
// -- Set up the ESP32
//    ----------------
void setup() {
  //
  // -- Set up the pins
  //    ---------------
  pinMode(BIT0, INPUT);
  pinMode(BIT1, INPUT);
  pinMode(BIT2, INPUT);
  pinMode(BIT3, INPUT);
  pinMode(BIT4, INPUT);
  pinMode(BIT5, INPUT);
  pinMode(BIT6, INPUT);
  pinMode(BIT7, INPUT);

  pinMode(RHLD, INPUT_PULLUP);      // use an internal pullup resistor since it's an open drain signal


  //
  // -- Initialize the outputs to LOW...  but recall that there is only 1 inverter in the 3v3 to 5v 
  //    level shifter.  So, we need to be careful about inverting the signals in code as well.
  //    -------------------------------------------------------------------------------------------
  pinMode(LTCH, OUTPUT);
  digitalWrite(LTCH, HIGH);

  pinMode(SHFT, OUTPUT);
  digitalWrite(SHFT, HIGH);

  pinMode(ADDR, OUTPUT);
  digitalWrite(ADDR, HIGH);

  pinMode(CLR, OUTPUT);
  digitalWrite(CLR, HIGH);
  delay(10);
  digitalWrite(CLR, LOW);

  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);

<<<<<<< Updated upstream
//  pinMode(CLK, OUTPUT);
//  digitalWrite(CLK, LOW);

=======
  pinMode(CLK, OUTPUT);
  digitalWrite(CLK, LOW);
>>>>>>> Stashed changes

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);



  //
  // -- Send a greeting
  //    ---------------
  delay(2000);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("Welcome to the EEPROM tester"));
  Serial.println(F("Ready to test"));       

  delay(1);
  
  Serial.println(F("Press Enter to begin"));
  while (Serial.read() != '\n') {}
}



//
// -- For whatever we are at, read the byte from the standard
//    -------------------------------------------------------
byte getGoldenByte(long i) {
//  return pgm_read_byte(geepromData + i);
  return ______control_logic_ctrl0_bin[i];
}



//
// -- Read the resulting byte from the sram
//    -------------------------------------
byte readSram(void) {
  bool b7 = digitalRead(BIT7);
  bool b6 = digitalRead(BIT6);
  bool b5 = digitalRead(BIT5);
  bool b4 = digitalRead(BIT4);
  bool b3 = digitalRead(BIT3);
  bool b2 = digitalRead(BIT2);
  bool b1 = digitalRead(BIT1);
  bool b0 = digitalRead(BIT0);

  byte rv = (b7 ? 0x80 : 0) | (b6 ? 0x40 : 0) | (b5 ? 0x20 : 0) | (b4 ? 0x10 : 0) | 
      (b3 ? 0x08 : 0) | (b2 ? 0x04 : 0) | (b1 ? 0x02 : 0) | (b0 ? 0x01 : 0);

  return rv;
}



//
// -- Check a byte identified by index i
//    ----------------------------------
bool checkByte(long i) {
  int addr = ~i;      // -- remember, the address needs to be inverted

  pinMode(CLR, OUTPUT);
  digitalWrite(CLR, HIGH);
  digitalWrite(ADDR, HIGH);
  delay(10);
  digitalWrite(CLR, LOW);

  for (int b = 0; b < 16; b ++) {
    int bit = addr & (1<<(15-b));

    digitalWrite(ADDR, (bit == 0) ? LOW : HIGH);
    digitalWrite(SHFT, LOW);
    delay(1);
    digitalWrite(SHFT, HIGH);
  }

<<<<<<< Updated upstream
  digitalWrite(LTCH, LOW);
  delay(2);
//  while (Serial.read() != '\n') {}
  digitalWrite(LTCH, HIGH);
=======
  delay(1);
  digitalWrite(CLK, LOW);
  delay(1);
  digitalWrite(CLK, HIGH);
  delay(1);

//  while (Serial.read() != '\n') {}
>>>>>>> Stashed changes

  byte s = readSram();
  byte g = getGoldenByte(i);

  Serial.printf("Expected %02.2x ", g);
  Serial.printf("Read %02.2x\n", s);

  return (g ^ s) == 0;
}



//
// -- This code is executed once per iteration, prepeatedly
//    -----------------------------------------------------
void loop() {
<<<<<<< Updated upstream
//  unsigned long counter = 0;
  unsigned long earlyStop = 0;
  unsigned long lateStop = ((32768 + 3 - 0) * 9) - 1;

  //
  // -- Execute a Reset Sequence
  //    ------------------------
  Serial.println(F("Resetting.  Press Enter to continue."));
  digitalWrite(RST, HIGH);
=======
  unsigned long counter = 0;
  unsigned long earlyStop = 0 * 9;
  unsigned long lateStop = ((32768 + 3 - 0) * 9) - 1;


  //
  // -- Execute a Reset Sequence
  //    ------------------------
  digitalWrite(RST, HIGH);
  Serial.println(F("In reset.  Press Enter to continue."));
>>>>>>> Stashed changes

  while (Serial.read() != '\n') {}

  digitalWrite(RST, LOW);
  delay(250);


  //
  // -- wait for the reset hold to come high -- as it will work on the real hardware
  //    ----------------------------------------------------------------------------
<<<<<<< Updated upstream
  Serial.println(F("... waiting for #RHLD"));

  while (digitalRead(RHLD) == LOW) {
#if 0
    if (counter < earlyStop || counter >= lateStop) {
      Serial.printf("%10.10ld: Enter for LOW CLK\n", counter);
      while (Serial.read() != '\n') {}
    }

    digitalWrite(CLK, HIGH);
    delay(1);

    if (counter < earlyStop || counter >= lateStop) {
      Serial.printf("%10.10ld: Enter for HIGH CLK\n", counter);
      while (Serial.read() != '\n') {}
    }

    digitalWrite(CLK, LOW);
    delay(1);
    counter ++;
#endif
  }
=======
  Serial.println(F("Copying data from EEPROM to SRAM"));
  Serial.println(F("... waiting for #RHLD"));
>>>>>>> Stashed changes

  while (digitalRead(RHLD) == LOW) {
  #if 0
  
    if (counter < earlyStop || counter >= lateStop) {
      Serial.printf("%10.10ld: Enter for LOW CLK\n", counter);
      while (Serial.read() != '\n') {}
    }

    digitalWrite(CLK, HIGH);
    delay(1);

    if (counter < earlyStop || counter >= lateStop) {
      Serial.printf("%10.10ld: Enter for HIGH CLK\n", counter);
      while (Serial.read() != '\n') {}
    }

    digitalWrite(CLK, LOW);
    delay(1);
    counter ++;

#else

    delay(500);

#endif
  }

  //
  // -- turn on the LED to indicate testing
  //    -----------------------------------
  digitalWrite(LED_BUILTIN, HIGH);
<<<<<<< Updated upstream
  Serial.println(F("Testing!"));
  delay(250);

//  while (Serial.read() != '\n') {}
=======
  Serial.println(F("Copy complete!  Testing!"));
  delay(10);
>>>>>>> Stashed changes

//  while (Serial.read() != '\n') {}


  //
  // -- clear the shift registers
  //    -------------------------
  pinMode(CLR, OUTPUT);
  digitalWrite(CLR, HIGH);
  delay(10);
  digitalWrite(CLR, LOW);



  for (long i = 0; i < 32768; i ++) {
    //
    // -- output some status indicators
    //    -----------------------------
//    if (i % 0x100 == 0) {
      Serial.printf("  address 0x%04.4x ", i);
//    }


    //
    // -- check if failed and if so, flash the LED and do nothing else
    //    ------------------------------------------------------------
//    checkByte(i);
    if (checkByte(i) == false) {
      Serial.println(F("Test FAILED!!!"));
      Serial.print(F("   location ")); Serial.println(i);


      //
      // -- flashing LED forever
      //    --------------------
      while (true) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
      }
    } 
  }


  //
  // -- if we get here, everything worked
  //    ---------------------------------
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println(F("Testing complete: SRAM is good"));
  delay(10);



  //
  // -- wait for another reset and then loop
  //    ------------------------------------
  while (digitalRead(RHLD) == HIGH) {
    delay(500);
  }
}
