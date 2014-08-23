#define hwSerial Serial1

#include <Servo.h> 


String readBuffer ="";
Servo mservo;

void setup()
{
  Serial.begin(9600);
  hwSerial.begin(115200,SERIAL_8N1);
  while(!Serial);
  Serial.println("Serial init complete");
  
  hwSerial.print("AT");
  mservo.attach(2);
}

void loop()
{
  bool hasRead=false;
  while (hwSerial.available()) {
     //readBuffer += (char)hwSerial.read();
     byte b = hwSerial.read();
     Serial.print((char)b);
     mservo.write((int)b);
     hasRead=true;
     delay(10);
  }
  if(hasRead){
    Serial.println();
  }
  hasRead=false;
  while(Serial.available()>0){
    char c = Serial.read();
    if(c!='\n'){
      hwSerial.write(c);      
      Serial.print(c);
    }
    hasRead=true;
    delay(10);
  }
  if(hasRead){
    Serial.println();
  }
}


