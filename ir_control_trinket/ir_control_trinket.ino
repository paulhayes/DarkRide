#include <PowerFunctions.h>
#include <elapsedMillis.h>

elapsedMillis timeElapsed; 
int lastLapTime;
int lastElapsedTime;

int ledPin = 0;
int detectorPin = 2;
bool wasOn;

PowerFunctions pf(ledPin, 0);

void setup(){
  pinMode(detectorPin, INPUT );
  lastLapTime = 150000;
}

bool IsPassedTime(int time){
  return ( lastElapsedTime < time ) && ( time < timeElapsed );
}

void loop(){
  digitalTest();
  
  
  if( IsPassedTime(1000) ){
      pf.single_pwm(RED,PWM_REV6);
      delay(500);
  }
  if( IsPassedTime(5000) ){
      pf.single_pwm(RED,PWM_FWD6);
      delay(500);
  }
  
  lastElapsedTime = timeElapsed;
  
}

void digitalTest(){
  bool isOn = digitalRead(detectorPin);
  if( isOn != wasOn && timeElapsed > 1000 ){
    wasOn = isOn;
    if( isOn ){
    }
    else {
      LapStarted();      
    }
    
  }
  
}

void LapStarted(){
  lastLapTime = timeElapsed;
  lastElapsedTime = timeElapsed = 0;
  
  pf.single_pwm(RED,PWM_BRK);
}
