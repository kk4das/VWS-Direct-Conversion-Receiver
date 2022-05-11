#ifndef DC_RadioControl_h
#define DC_RadioControl_h

#define CALLSIGN "KK4DAS"
#define VERSION "0.1"  // test build
#define RIGNAME "VWS DCR"


//#define DEBUG
#ifdef DEBUG
 extern char debugmsg[];
#endif


//=============================== FEATURE SELECTION =========================
// Each flag below enables optional features
// SMNETER    - Uncomment if S-meter sensor circuit is installed
// DUAL_BAND  - Uncomment to enable band switching 20/40 if installed


//=============================== S-METER INSTALLED =========================
//#define SMETER              // uncomment if SMETER mod installed

//=============================== DUAL BAND MOD INSTALLED ===================
//#define DUAL_BAND           // uncomment if dual band mod installed 20/40 


//============================== BOARD TYPE (Nano, Every) =====================
#define Nano 0
#define Every 1
#define Uno 2

#if defined(ARDUINO_AVR_NANO)
#define _BOARDTYPE Nano
#endif
#if defined(ARDUINO_AVR_NANO_EVERY)
#define _BOARDTYPE Every
#endif
#if defined (ARDUINO_AVR_UNO)
#define _BOARDTYPE Uno
#endif

//============================= INCLUDES =====================================
#include <Arduino.h>
#include "Utility.h"         // General purpose common functions
#include "SSB_Display.h"     // Display handling
#include "RotaryEnc.h"       // Rotary encoder handling
#include "si5351.h"          // SI 5351 clock
#include "Settings.h"        // Get/Store Settings in EEPROM
#include "Smeter.h"          // Smeter definitions

//============================== Symbolic constants ==========================


//////////////////////////////////////////////////////////////////////
//                                                                  //
//           Arduino Pin Definitions                                //
//                                                                  //
//////////////////////////////////////////////////////////////////////


#define ENCODER_A      2                  // Rotary Lib Default - Encoder pin A  D2 (interrupt pin)
#define ENCODER_B      3                  // Rotary Lib Default - Encoder pin B  D3 (interrupt pin)

// Dual Band Pins (requires dual band mod)
#define BAND_BTN       6                  // Band Switch momentary button
#define BAND_PIN       7                  // Band Switch Relay -  LOW = Band A (40M), HIGH = Band B (20M)

#define VFO_BTN        A0                 // VFO A/B button
#define ENCODER_BTN    A3                 // Rotary Lib Default - Encoder push button
#define I2C_SDA        A4                 // I2C SDA Pin
#define I2C_SCL        A5                 // I2C SCL Pin

//  Smeter
#define SMETER_PIN     A7                 // Requires Signal Strength Sensor




//
// Dual Band Mode Constants (requires DUAL_BAND)
//
#define BAND20  1
#define BAND40  2
#define BAND20_EDGE  14000000
#define BAND40_EDGE  7000000

// VFO used both here and in the display
// Define here if not already defined 

// VFO selection
#define VFOA 0 
#define VFOB 1


//=============== Globals ============================================

// Rotary Encoder
extern Rotary encoder;

// VFO A/B frequencies
extern uint32_t vfoAfreq; 
extern uint32_t vfoBfreq; 

// Tuning increment
extern uint32_t increment;

// Active VFO indicator
extern byte active_vfo;


// Band specific memory - requires DUAL_BAND
extern uint32_t band20Freq;
extern uint32_t band40Freq;
extern byte band;
 
// S Meter
extern byte smeter;


//=============== Function Prototypes ============================================

extern void setupEncoder();
extern void setVFO(uint32_t freq);
extern void CheckIncrement();
extern void AdvanceIncrement();
extern void CheckEncoder();
extern void AdjustVFO(long delta);
extern void CheckVFO();
extern void SwapVFO();
extern void setupPins();
extern void CheckBand();    // Only called if DUAL_BAND enabled
extern void CheckSmeter();  // Only called if SMETER enabled

#endif
