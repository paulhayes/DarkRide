#include <PowerFunctions.h>
#include <elapsedMillis.h>

#define ledPin 0
#define detectorPin 2
#define resetPin 1

unsigned long minPossibleLapTime = 40000;
unsigned long baseLapTime = 86200;
unsigned long defaultLapTime = 94800;

unsigned long c1 = 30+250;
unsigned long c2 = 5760+250;
unsigned long c3 = 14120;
unsigned long c4 = 32280+500;
unsigned long c5 = 47440;
unsigned long c6 = 59440;
unsigned long c7 = 77200+1500;

elapsedMillis timeElapsed; 
unsigned long currentFrameTime;
unsigned long lastLapTime;
unsigned long lastFrameTime;

float lapLapTimeRatio = 1.0;

byte TURN = BLUE;

bool startMode = true;

bool wasOn;

PowerFunctions pf(ledPin, 0);

void setup(){
  pinMode(detectorPin, INPUT );
  lastLapTime = defaultLapTime;

}

bool HasCrossedTime(unsigned long time){
  time = LapCompensate(time);
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
    move1();
  }
  if( HasCrossedTime(c2) ){
    move2();
  }
  
  if( HasCrossedTime(c3) ){
    move3();
  }
 
  if( HasCrossedTime(c4) ){
    move4();
  }
 
  if( HasCrossedTime(c5) ){
    move5();
  }
 
  if( HasCrossedTime(c6) ){
    move6();
  }
 
  if( HasCrossedTime(c7) ){
    move7();
  }
 
   delay(10); 
}

void move1(){
    fullLeft(); //PWM_REV6
}

void move2(){
  pf.single_pwm(TURN,PWM_BRK);
}

void move3(){
  pf.single_pwm(TURN,PWM_FWD6);
}

void move4(){
  pf.single_pwm(TURN,PWM_REV6);
}

void move5(){
  pf.single_pwm(TURN,PWM_BRK);
}

void move6(){
    fullRight();  //PWM_FWD6 
}

void move7(){
  pf.single_pwm(TURN,PWM_BRK);  //
}

void fullLeft(){
      pf.single_pwm(TURN,PWM_REV6);
      pf.single_increment(TURN,false);
}

void fullRight(){
      pf.single_pwm(TURN,PWM_FWD6);
      pf.single_increment(TURN,true);
}

unsigned long LapCompensate(unsigned long time){
  return lapLapTimeRatio * time;
}

bool CheckLapStart(){
  bool isOn = digitalRead(detectorPin);
  bool bouncePrevention = ( currentFrameTime > minPossibleLapTime ) || startMode;
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
  if( startMode ){
    lastLapTime = defaultLapTime;
  }
  else{
    lastLapTime = currentFrameTime;
  }
  
  lapLapTimeRatio = 1.0f * lastLapTime / baseLapTime;
  
  startMode = false;
  currentFrameTime = lastFrameTime = timeElapsed = 0;
  
  pf.single_pwm(TURN,PWM_BRK);
}
