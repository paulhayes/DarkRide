#define hwSerial Serial1

#include <Servo.h> 
#include <elapsedMillis.h>

elapsedMillis timeElapsed;

String readBuffer ="";
Servo mservo;

int position = 90;
int destPosition = position;

void setup()
{
  Serial.begin(9600);
  hwSerial.begin(115200,SERIAL_8N1);
  //while(!Serial);
  ///Serial.println("Serial init complete");
  
  //hwSerial.print("AT");
  
  mservo.attach(2,400,2400);
  mservo.write(position);
}

void loop()
{
  
  if (hwSerial.available()) {
     destPosition = hwSerial.read();
     hwSerial.write(position);
  }
  if( timeElapsed > 10 ){
    move();
    timeElapsed = 0;
  }
}

void move(){
  int diff = destPosition - position;
  if( diff > 0 ){
    mservo.write( ++position );
  }
  else if( diff < 0 ){
    mservo.write( --position );
  }
}


