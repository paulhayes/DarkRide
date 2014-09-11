#include <PowerFunctions.h>
#include <elapsedMillis.h>

elapsedMillis timeElapsed; 
elapsedMillis timeSinceLastAnalogRead; 

unsigned int interval = 200;

int led = 2;
int triggerPin = 3;
int flexPin = A0;
boolean ledState = LOW;

PowerFunctions pf(led, 0);

int flexThreshold = 545;
int flexInterval = 40;

void setup(){
  Serial.begin(9600);
  
  //while( !Serial );
  
  pinMode(led, OUTPUT);
  pinMode(triggerPin, INPUT);
}

void loop(){
        /*
        if (timeElapsed > interval){
              pf.single_pwm(output, pwm);
              pf.single_pwm(output, PWM_BRK);
              pf.single_pwm(output, PWM_FLT);

             //ledState = !ledState;
             //digitalWrite(led, ledState);
             timeElapsed = 0; 
        }
        */
        
        
        if( digitalRead(triggerPin) == HIGH && timeElapsed > 200 ){          
              pf.single_pwm(RED, PWM_FWD7);
              timeElapsed = 0; 
        }
        
        if( timeSinceLastAnalogRead > flexInterval ){
          int flexRead = analogRead(flexPin);
          Serial.println(flexRead);
          if( flexRead < flexThreshold ){
            pf.single_pwm(RED, PWM_FWD7);
          }
          timeSinceLastAnalogRead = 0;
        } 
}

 
