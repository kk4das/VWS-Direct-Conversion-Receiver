// Button handler
#include "DC_RadioControl.h"

//#define BTN_DEBUG

//////////////////////////////////////////////////////////////////////
//                                                                  //
//              Button Control Variables                            //
//                                                                  //
//////////////////////////////////////////////////////////////////////

// Encoder button control
byte EncButtonState = 0;
byte lastEncButtonState = 0;

//  Band Switch button control
byte BandButtonState = 0;
byte lastBandButtonState = 0;

// VFO select button control
byte VFOButtonState = 0;
byte lastVFOButtonState = 0;


//*********************Check Band ************************************
void SwapBand() {

  uint32_t freq;

  if (band == BAND20) {          // Switch to 40 Meter Band
    band = BAND40;
    freq = band40Freq;
  } else {                        // Switch to 20 Meter Band
    band = BAND20;
    freq = band20Freq;
  }

  //
  // Set the active VFO to new frequency
  //
  switch (active_vfo) {
    case VFOA:
      vfoAfreq = freq;
      setVFO(vfoAfreq);
      displayActVFO(vfoAfreq);
      break;
    case VFOB:
      vfoBfreq = freq;
      setVFO(vfoBfreq);
      displayActVFO(vfoBfreq);
      break;
  }

  //
  // Update the display to show the active mode
  //
  startSettingsTimer();
}

void CheckBand() {
  BandButtonState = digitalRead(BAND_BTN);
  if (BandButtonState != lastBandButtonState) {

    //
    // On button press, change band 20/40
    //
    if (BandButtonState == LOW) { // if button pressed

#ifdef BTN_DEBUG
      ToggleLED();
      String msg = F("CheckBand");
      displayBanner(msg);
#endif
      SwapBand();
    }

    lastBandButtonState = BandButtonState;
    Delay(50);
    BandButtonState = digitalRead(BAND_BTN);  //debounce
  }
}




//*********************VFO switch******* VFO A or B ****************
void SwapVFO() {

  if (active_vfo == VFOA) {
    active_vfo = VFOB;                // Make VFOB Active
  } else {
    active_vfo = VFOA;                // Make VFOA Active
  }

  //
  // Swap Active/Alternate frequency displays
  //
  switch (active_vfo) {
    case VFOA:
      setVFO(vfoAfreq);
      displayActVFO(vfoAfreq);
      displayAltVFO(vfoBfreq);
      break;
    case VFOB:
      setVFO(vfoBfreq);
      displayActVFO(vfoBfreq);
      displayAltVFO(vfoAfreq);
      break;
  }
  displayVFOAB(active_vfo);            // Change the A/B indicator
  
  startSettingsTimer();
}


void CheckVFO() {

  VFOButtonState = digitalRead(VFO_BTN);
  if (VFOButtonState != lastVFOButtonState) {
    if (VFOButtonState == LOW) {       // button pressed
      SwapVFO();
    }
    lastVFOButtonState = VFOButtonState;
    Delay(50);
    VFOButtonState = digitalRead(VFO_BTN);  //debounce
  }
}
