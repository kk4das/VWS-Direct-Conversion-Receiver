/*
   VWS_Direct_Conversion
   Dean Souleles, KK4DAS, contact kk4das@gmail.com
 
   This sketch implement s basic Direct Conversion (dual sideband)  radio control panel with the following features:
     
       Dual VFO A/B
       Rotary encoder for tuning, push button to change tuning increment
       SI5351 Clock generator for the VFO/LO
       Settings memory (last frequency and mode are saved)
       Optional S-Meter

       Controls
         * Rotary encoder to change frequence
         * Rotary encoder button changes tuning increment
         * VFO A/B Select toggle

         * Optional dual-band support 20/40

       20x4 LCD Display features
         * Dual VFOS
         * TuningStep Inidicator
         * Optional S Meter
         * Banner including callsign

   Version 1.0
   Jan 10, 2022
       Copied from SSB Radio Control 1_5
       Removed CAT, Nextion, TFT, TX and SSB functions

   NOTE TO BUILDERS
   This is a reference implementation of an Direct Conversion radio control program for a homebrew Direct Conversion receiver.
   It is a fully functioning Direct Conversion radio control program. While it has been built for my particular hardware
   configuration every attempt has been made to make it modular in design so that the builder can swap out
   modules at will.  It should be fairly straight forward to adapt to the builder's hardware selection
   of switches, buttons and knobs or even alternate displays.

*/

#include "DC_RadioControl.h"


#ifdef DEBUG
char debugmsg[25];
#endif

//=============== Globals ============================================

//////////////////////////////////////////////////////////////////////
//                                                                  //
//            si5351 Clock Module                                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////
Si5351 si5351;

// Calibration offest - adjust for variability in the SI5351 module
// crystal - must be set for the particular SI5351 installed
//#define CALIBRATION_OFFSET 1190  // Calibration for the SI-5351

#define CALIBRATION_OFFSET 1250  // Calibration for the SI-5351
//////////////////////////////////////////////////////////////////////
//                                                                  //
//            BFO and VFO Constants and Variables                   //
//                                                                  //
//////////////////////////////////////////////////////////////////////

//
// Startup VFO A/B frequencies
//
uint32_t  vfoAfreq = 7200000L;   //  7.200.000
uint32_t  vfoBfreq = 7074000L;   //  FT-8 7.074.000


uint32_t increment = 1000;                //  startup VFO tuning increment in HZ.


//////////////////////////////////////////////////////////////////////
//                                                                  //
//            Active VFO                                            //
//                                                                  //
//////////////////////////////////////////////////////////////////////
byte active_vfo = VFOA;  // startup on VFOA



//////////////////////////////////////////////////////////////////////
//                                                                  //
//           Dual Band Starup                                       //
//                                                                  //
/////////////////////////////////////////////////////////////////////
uint32_t band20Freq = 14200000L;  // 14.200.000
uint32_t band40Freq = 7200000L;   //  7.200.000
byte band = BAND40;

//////////////////////////////////////////////////////////////////////
//                                                                  //
//           S Meter 0-9 +10, +20 +30                               //
//                                                                  //
//////////////////////////////////////////////////////////////////////
byte smeter = 0;             //  startup s_meter reading (requires SMETER)




///////////////////////////////////////////////////////////
//  setBandFilters(band)                                 //
//    For 20 meters turn relay ON   (NO)                 //
//    For 40 meters turn relay OFF  (NC)                 //
///////////////////////////////////////////////////////////
void setBandFilters(int band) {
#ifdef DUAL_BAND
  switch (band) {
    case BAND20:
      digitalWrite(BAND_PIN, HIGH);
      break;
    case BAND40:
      digitalWrite(BAND_PIN, LOW);
      break;
  }
#endif
}


//********************setVFO******************************************
void setVFO(uint32_t  freq) {
  //
  //  Set CLK0 to the to input frequency 
  
#ifdef DUAL_BAND
  //
  // Set filters for the band based on frequency
  // Save frequency for band switching
  //
  if (freq >= BAND20_EDGE) {
    setBandFilters(BAND20);
    band20Freq = freq;
    band = BAND20;
  } else if (freq >= BAND40_EDGE) {
    setBandFilters(BAND40);
    band40Freq = freq;
    band=BAND40;
  }
#endif

  si5351.set_freq(freq, SI5351_PLL_FIXED, SI5351_CLK0);
  startSettingsTimer();  // start timer to save current settings
}


//*********************Setup Arduino Pins*****************************

void setupPins() {
  //
  // Set the control buttons to INPUT_PULLUP
  // Button state will be HIGH when open and LOW when pressed
  //

  pinMode(VFO_BTN, INPUT_PULLUP);                            // VFO A/B Select - momentary button
  pinMode(BAND_BTN, INPUT_PULLUP);                           // Band Switch 20/40 - momentary button
  pinMode(BAND_PIN, OUTPUT); digitalWrite(BAND_PIN, LOW);    // Band Switch Relay (LOW = NC = 40m,  HIGH = NO = 20m)
  pinMode(LED_BUILTIN, OUTPUT);                              // for debug / testing

}


//**********************Initialize SI5351******************************

void setupSI5351() {
  si5351.init(SI5351_CRYSTAL_LOAD_8PF);
  si5351.set_correction(CALIBRATION_OFFSET);           // calibration offset
  si5351.set_pll(SI5351_PLL_FIXED, SI5351_PLLA);
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_8MA); // Higher Drive since it is a ADE-1 DBM

  si5351.clock_enable(SI5351_CLK1, 0);  // disable clock 1 and 2 out of abundance of caution
  si5351.clock_enable(SI5351_CLK2, 0);


  //
  // Initialize the VFO
  //
  switch (active_vfo) {
    case VFOA:
      setVFO(vfoAfreq);
      break;
    case VFOB:
      setVFO(vfoBfreq);
      break;
  }
}


//////////////////////////////////////////////////////////////////////
//                         Setup                                    //
//                                                                  //
// Called once by the Arduino operating system at startup           //
// All initialization code goes here                                //
//                                                                  //
//////////////////////////////////////////////////////////////////////
void setup() {

  uint32_t  vfoActfreq;
  uint32_t  vfoAltfreq;

#ifdef DEBUG
  Serial.begin(57600);
  Serial.println("Debug on");
#endif

  setupPins();              // Initialize arduino pins
  setupEncoder();           // Initialize interrupt service for rotary encoder
  setupSettings();          // Retrive settings from EEPROM
  setupSI5351();


  if (active_vfo == VFOA) {
    vfoActfreq = vfoAfreq;
    vfoAltfreq = vfoBfreq;
  } else {
    vfoActfreq = vfoBfreq;
    vfoAltfreq = vfoAfreq;
  }

  
  //  Initialize the display with startup values
  Delay(500);  // short delay to let the display initialize - needed for Nextion

  // "Vx.x   RIGNAME  CALLSIGN"
  String banner;

  banner += F(CALLSIGN);
  banner += F(" ");
  banner += F(RIGNAME);
  banner += F("  ");
  banner += F("V");
  banner += F(VERSION);



  displaySetup(banner,              // version number. call sign
               vfoActfreq, vfoAltfreq,  // Initial active and alternate VFO
               active_vfo,          // VFO A/B indicator
               increment,           // Tuning increment
               smeter);            // S Meter

}

void loop() {

  CheckEncoder();   //VFO frequency changes
  CheckIncrement(); // Encoder Button
  CheckVFO();       // VFO A/B change

#ifdef DUAL_BAND
  CheckBand();      // Band Switch 20/40
#endif

#ifdef SMETER
  CheckSmeter();    // Signal strength
#endif

  CheckSettings();  // Update EEPROM on Settings Change

}
