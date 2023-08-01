#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8);
char incomingByte=0;

void setup()
{
  mySerial.begin(9600);               // the GPRS baud rate   
  Serial.begin(9600);    // the GPRS baud rate 
   mySerial.println("AT");
      delay(2000);
      mySerial.println("AT+CMGF=1\r");
      delay(2000);  
      mySerial.println("AT+CSMP=17,167,0,0\r");
      delay(2000);
      mySerial.println("AT+CMGR=\"ALL\"\r");
      delay(2000);  
}


void loop()
{
  
mySerial.write("AT+CMGR=17\r");
 
 
        while (mySerial.available() > 0)
        {      
               incomingByte = mySerial.read();
               if(incomingByte!='/n')
               {
               Serial.print(incomingByte);
               }
               
               if(incomingByte=='/n')
               {
                 Serial.println(incomingByte);
               }
         
     
 
        }
}
