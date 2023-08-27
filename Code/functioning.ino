#include "Moving.h"
#include "Sensor.h"
#include "SoftwareSerial.h"
int speed=1;
Moving moving = Moving(speed);
Sensor sensor = Sensor();

int duration = 6000;
unsigned long ancientTime; 
unsigned long currentTime;


void setup() {
  Serial.begin(9600);
  spiderRadio.begin(9600);  
  moving.normalPosition();
  delay(2000);
  ancientTime=millis(); // we initialize ancientTime to now.
}

void loop() {
    currentTime=millis();
    if ((currentTime-ancientTime)>=duration){
      sensor.movementCheckObstacle();
      ancientTime=millis();
    }
    moving.movement(1);
}
