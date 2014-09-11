#include <PowerFunctions.h>
#include <elapsedMillis.h>

#define ledPin 0
#define detectorPin 2
#define resetPin 1

unsigned long baseLapTime = 86200;
unsigned long c1 = 30+250;
unsigned long c2 = 5760+250;
unsigned long c3 = 14120;
unsigned long c4 = 32280;
unsigned long c5 = 47440;
unsigned long c6 = 59440;
unsigned long c7 = 77200+1000;

byte m1 = PWM_REV6;
byte m2 = PWM_BRK;
byte m3 = PWM_FWD6;
byte m4 = PWM_REV6;
byte m5 = PWM_BRK;
byte m6 = PWM_FWD6;
byte m7 = PWM_BRK;


elapsedMillis timeElapsed; 
unsigned long currentFrameTime;
unsigned long lastLapTime;
unsigned long lastFrameTime;

byte TURN = BLUE;

bool startMode = true;

bool wasOn;

PowerFunctions pf(ledPin, 0);

void setup(){
  pinMode(detectorPin, INPUT );
  lastLapTime = baseLapTime;
}

bool HasCrossedTime(unsigned long time){
  return ( lastFrameTime < time ) && ( time <= currentFrameTime );
}

void loop(){
  lastFrameTime = currentFrameTime;
  currentFrameTime = timeElapsed;
  
  if( digitalRead(resetPin) ){
    startMode = true;
  }

  if( CheckLapStart() || startMode ){
    delay(10);
    return;
  }
      
  if( HasCrossedTime(c1) ){
      pf.single_pwm(TURN,m1);
  }
  if( HasCrossedTime(c2) ){
      pf.single_pwm(TURN,m2);
  }
  
  if( HasCrossedTime(c3) ){
      pf.single_pwm(TURN,m3);
  }
 
  if( HasCrossedTime(c4) ){
      pf.single_pwm(TURN,m4);
  }
 
  if( HasCrossedTime(c5) ){
      pf.single_pwm(TURN,m5);
  }
 
  if( HasCrossedTime(c6) ){
      pf.single_pwm(TURN,m6);
  }
 
  if( HasCrossedTime(c7) ){
      pf.single_pwm(TURN,m7);
  }
 
   delay(10); 
}

unsigned long lapCompensate(unsigned long time){
  return time * lastLapTime / baseLapTime;
}

bool CheckLapStart(){
  bool isOn = digitalRead(detectorPin);
  bool bouncePrevention = timeElapsed > 1000;
  if( isOn != wasOn && bouncePrevention ){
    wasOn = isOn;
    if( isOn ){
    }
    else {
      LapStarted();      
      return true;
    }
    
  }
  return false;
  
}

void LapStarted(){
  if( !startMode ){
    lastLapTime = timeElapsed;
  }
  else{
    lastLapTime = baseLapTime;
  }
  
  startMode = false;
  currentFrameTime = lastFrameTime = timeElapsed = 0;
  
  pf.single_pwm(TURN,PWM_BRK);
}
