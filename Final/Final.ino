#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int i = 0;
ADXL345 adxl = ADXL345();
void setup(){
  Serial.begin(9600);                 // Start the serial terminal
  Serial.println("SparkFun ADXL345 Accelerometer Hook Up Guide Example");
  Serial.println();
  adxl.powerOn();
  adxl.setRangeSetting(16);
  adxl.setSpiBit(0);
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.singleTapINT(1);
}

void loop(){
  int x,y,z;   
  adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
  ADXL_ISR();
}

void ADXL_ISR() {
  byte interrupts = adxl.getInterruptSource();
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
    lcd.begin(16, 2);
    Serial.print("YO!");
    lcd.print(i++);
    
  } 
}

