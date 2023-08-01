/*  ********************************************* 
 *  SparkFun_ADXL345_Example
 *  Triple Axis Accelerometer Breakout - ADXL345 
 *  Hook Up Guide Example 
 *  
 *  Utilizing Sparkfun's ADXL345 Library
 *  Bildr ADXL345 source file modified to support 
 *  both I2C and SPI Communication
 *  
 *  E.Robert @ SparkFun Electronics
 *  Created: Jul 13, 2016
 *  Updated: Sep 06, 2016
 *  
 *  Development Environment Specifics:
 *  Arduino 1.6.11
 *  
 *  Hardware Specifications:
 *  SparkFun ADXL345
 *  Arduino Uno
 *  *********************************************/

 //For LCD
 // include the library code:
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

#include <TinyGPS.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library

/*********** COMMUNICATION SELECTION ***********/
/*    Comment Out The One You Are Not Using    */
//ADXL345 adxl = ADXL345(10);           // USE FOR SPI COMMUNICATION, ADXL345(CS_PIN);
ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

TinyGPS gps;
SoftwareSerial ss(6, 7);

/****************** INTERRUPT ******************/
/*      Uncomment If Attaching Interrupt       */
//int interruptPin = 2;                 // Setup pin 2 to be the interrupt pin (for most Arduino Boards)

int i=0; //for detecting how many times that you've tapped since inactivity 


/******************** SETUP ********************/
/*          Configure ADXL345 Settings         */
void setup(){
  
  Serial.begin(9600);                 // Start the serial terminal
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  ss.begin(9600);
  
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
                                      // Accepted values are 2g, 4g, 8g or 16g
                                      // Higher Values = Wider Measurement Range
                                      // Lower Values = Greater Sensitivity

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
                                      // Default: Set to 1
                                      // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library 
   
  adxl.setActivityXYZ(1, 0, 0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
 
  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(75);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
 
  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment
 
  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment
 
  // Setting all interupts to take place on INT1 pin
  //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);" 
                                                        // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
                                                        // This library may have a problem using INT2 pin. Default to INT1 pin.
  
  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(1);
  adxl.doubleTapINT(1);
  adxl.singleTapINT(1);
  
//attachInterrupt(digitalPinToInterrupt(interruptPin), ADXL_ISR, RISING);   // Attach Interrupt

}

/****************** MAIN CODE ******************/
/*     Accelerometer Readings and Interrupt    */
void loop(){
  
  // Accelerometer Readings
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z

  // Output Results to Serial
  /* UNCOMMENT TO VIEW X Y Z ACCELEROMETER VALUES */  
  //Serial.print(x);
  //Serial.print(", ");
  //Serial.print(y);
  //Serial.print(", ");
  //Serial.println(z); 
  
  ADXL_ISR();
  // You may also choose to avoid using interrupts and simply run the functions within ADXL_ISR(); 
  //  and place it within the loop instead.  
  // This may come in handy when it doesn't matter when the action occurs. 

}

/********************* ISR *********************/
/* Look for Interrupts and Triggered Action    */
void ADXL_ISR() {
  
  // getInterruptSource clears all triggered actions after returning value
  // Do not call again until you need to recheck for triggered actions
  byte interrupts = adxl.getInterruptSource();
  float flat, flon;
  unsigned long age;
  lcd.clear();
  // Tap Detection
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    Serial.println("\nACCIDENT DETECTED!");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACCIDENT");
    lcd.setCursor(0,1);
    lcd.print("DETECTED!");
    smartdelay(5000);
    gps.f_get_position(&flat, &flon, &age);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LAT: ");
    print_float_lcd(flat, TinyGPS::GPS_INVALID_F_ANGLE, 10, 6);
     //add code here to do when a tap is sensed
     lcd.setCursor(0, 1);
     lcd.print("LON: ");
    print_float_lcd(flon, TinyGPS::GPS_INVALID_F_ANGLE, 11, 6);
     smartdelay(5000);
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("SENDING MSG...");
     lcd.setCursor(0,1);
     lcd.print("TO: 7405672274");
    smartdelay(5000);
    Serial.print("AT+CMGF=1\r");    
    Serial.print("AT+CMGS=\"+917405672274\"\r");    //Number to which you want to send the sms
    delay(1000);
    Serial.print("ACCIDENT DETECTED!\nLatitude:");   //The text of the message to be sent
    delay(1000);
    Serial.print(flat,6);
    Serial.print("\nLongitude:");
    Serial.print(flon,6);
    Serial.print("Google Maps Link: http://maps.google.com/maps?q=");
    Serial.print(flat,6);
    Serial.print(",");
    Serial.print(flon,6);
    Serial.print("\r");
    Serial.write(0x1A);
    delay(1000); 
  } 
  smartdelay(1000);
}


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

static void print_float(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      lcd.print('*');
  }
  else
  {
    lcd.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartdelay(0);
}

static void print_float_lcd(float val, float invalid, int len, int prec)
{
  if (val == invalid)
  {
    while (len-- > 1)
      lcd.print('*');
  }
  else
  {
    lcd.print(val, prec);
  }
  smartdelay(0);
}
