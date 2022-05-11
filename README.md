# VWS-Direct-Conversion-Receiver
HF Direct Conversion Receiver project from the Vienna Wireless Society Makers
 VWS_Direct_Conversion
   Dean Souleles, KK4DAS, contact kk4das@gmail.com
 
   This sketch implements basic Direct Conversion (dual sideband)  radio control panel with the following features:
     
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
   
   REQUIRED LIBRARY
   LiquuidCystal_I2C - V1.2 - install with the Arduino Library Manager.
 
   BUILD INSTRUCTIONS
   Create a folder in your Arduino directory called "VWS_Direct_Conversion_1_0".  Copy all of the source files to that folder. 
   use the Arudino Library manager to download and install the required external library.
   
   This has been built and tested on an Arduino Nano and an Arduino Nano Every
