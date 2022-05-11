#include "SSB_Display.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); //0x27 or 0x3F 0r 0x20  //1604 LCD/w I2C


//////////////////////////////////////////////////////////////////////
//                                                                  //
//                    Display Routines                              //
// Display routines for 20x4 LCD                                    //
//                                                                  //
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Display Layout 20x4                                              //
//              1111111111                                          //
//    01234567890123456789                                          //
// 0: A>xX.XXX.XXX xxxx Hz                                          //
// 1: B>xX.XXX.XXX                                                  //
// 2: S:123456789+++                                                //
// 3: BANNER                                                        //
//////////////////////////////////////////////////////////////////////



//************************displayBanner*****************************
// Sets up display line 3 - call sign, increment display and version
//
// Note - Does not use the input String (included for code compatibility
//        with other display modules)  
// 
void displayBanner(String s) {
  
  // Setup line 3  call and tuning increment
  lcd.setCursor(0, 3);
  lcd.print(s.c_str());
}

  
//************************displaySmeter****************************
void displaySMeter(byte level) {
  int i;
  lcd.setCursor(0, 2);
  lcd.print("S:");
  for (i = 0; i < MAXSLEVELS; i++) {
    if (level >= i) {
      if (i < 9) {
        lcd.print(i + 1);
      } else {
        lcd.print("+");
      }
    } else {
      lcd.print(" ");
    }
  }
}


//************************displayVFOAB*******************************
// displayVFOAB(int VFO)
//   Updates the display with flag for the currently active VFO - A or B
//
void displayVFOAB(int vfo) {
  // Reset the active VFO flag

  // Clear previous flag
  lcd.setCursor(1,0);
  lcd.print(F(" "));
  lcd.setCursor(1,1);
  lcd.print(F(" "));

  // Set flag by currently active VFO
  if (vfo == VFOA) {
    lcd.setCursor(1, 0);
  } else {
    lcd.setCursor(1, 1);
  }
  lcd.print(F(">"));
}


//************************displayVFO*******************************

// displayVFO(vfo,freq)
//   Updates the dispaly for the input vfo
//
void displayVFO(int vfo, long  freq) {
  int row = 0;

#ifdef DEBUG
  sprintf(debugmsg, "VFO %d %lu", vfo, uint32_t(freq));
  Serial.println(debugmsg);
#endif

  if (vfo == VFOA) {
    row = 0;
  } else if (vfo == VFOB) {
    row = 1;
  }

  // Update the frequency
  lcd.setCursor(2, row);
  if (freq < 10000000) {
    lcd.print(" ");
  }
  lcd.print(int((freq / 1000000))); //millions
  lcd.print(".");
  lcd.print(((freq / 100000) % 10)); //hundredthousands
  lcd.print(((freq / 10000) % 10)); //tenthousands
  lcd.print(((freq / 1000) % 10)); //thousands
  lcd.print(".");
  lcd.print(((freq / 100) % 10)); //hundreds
  lcd.print(((freq / 10) % 10));  //tens
  lcd.print(((freq / 1) % 10));   //ones

}

//************************displayActVFO*******************************
// Updates the frequencey display for the current active VFO A or B
void displayActVFO(uint32_t  freq) {
  displayVFO(active_vfo, freq);
}

//************************displayActVFO*******************************
// Updates the frequencey display for the current alternate VFO A or B
extern void displayAltVFO(uint32_t  freq) {
 if (active_vfo == VFOA) {
  displayVFO(VFOB, freq);
 } else{
  displayVFO(VFOA, freq);
 }
}


//************************displayIncr*******************************
//
// displayIncr(increment)
//   Display the tuning increment
//
void displayIncr(unsigned long increment) {

  String hertz = "    "; // tune step display

#ifdef DEBUG
  sprintf(debugmsg, "Increment: %ld", increment);
  Serial.println(debugmsg);
#endif

  if (increment == 10) {
    hertz = F("  10");
  } else if (increment == 100) {
    hertz = F(" 100");
  } else if (increment == 1000) {
    hertz = F("  1K");
  } else if (increment == 10000) {
    hertz = F(" 10K");
  } else if (increment == 100000) {
    hertz = F("100K");
  } else if (increment == 1000000) {
    hertz = F("  1M");
  }

  lcd.setCursor(13, 0);
  lcd.print(hertz);
}

//
// displayDebug 
// Displays debug message on the banner line
void displayDebug(String msg) {
  lcd.setCursor(0, 3);
  lcd.print(F("                    "));
  lcd.setCursor(0, 3);
  lcd.print(msg);
}


///////////////////////////////////////////////////////////////////////////
// displaySetup()
// Initialze and populate the display
///////////////////////////////////////////////////////////////////////////
void displaySetup(String banner,
                  uint32_t  vfoActfreq, uint32_t  vfoAltfreq,
                  uint32_t activeVFO,
                  uint32_t increment,
                  byte s_meter) {
  lcd.init();
  lcd.clear();
  delay(100);
  lcd.backlight();

  // Setup fixed screeen elements

  // Line 0 - VFO A
  lcd.setCursor(0,0);
  lcd.print(F("A"));

  // Line 1 VFO B
  lcd.setCursor(0,1);
  lcd.print(F("B"));

  lcd.setCursor(18, 0);
  lcd.print("Hz");
  
  //
  // Display the intitial values
  //
  displayBanner(banner);
  displayActVFO(vfoActfreq);
  displayAltVFO(vfoAltfreq);
  displayVFOAB(activeVFO);
  displayIncr(increment);
  
#ifdef SMETER
  displaySMeter(s_meter);
#endif
 }
