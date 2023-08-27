#include "Sensor.h"
#include <Arduino.h>
#include <Wire.h>        // Instantiate the Wire library
#include <TFLI2C.h>      // TFLuna-I2C Library v.0.1.1
#include <NewPing.h>
#include "Moving.h"
TFLI2C tflI2C;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int16_t  tfDist;    // distance in centimeters
int16_t  tfAddr = TFL_DEF_ADR;  // Use this default I2C address
Moving moving = Moving(1);
Sensor::Sensor(){
}

//returns the obstacle detection distance using the HC-SR04 ultrasonic sensor
long Sensor::sensorUltrason(){
  delay(29);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  Serial.println("UltraSon : "+String(sonar.convert_cm(uS))+" cm"); // Convert ping time to distance and print result (0 = outside set distance range, no ping echo)
  return(sonar.convert_cm(uS));
}

//returns the obstacle detection distance using the TF-Luna LIDAR infrared sensor
int Capteurs::capteurIR(){
  if(tflI2C.getData(tfDist, tfAddr)){
     Serial.println("IR : "+String(tfDist)+" cm");
     return(tfDist);
  }
}

void Sensor::movementCheckObstacle(){
  moving.positionNormal();
  delay(200);
  moving.checkLeft();
  delay(1000);
  obstacleDistance(0);
  moving.checkWrite();
  delay(1000);
  obstacleDistance(1);
  moving.checkMid();
  delay(1000);
  obstacleDistance(2);
  mouvementIfObstacle();
  delay(500);
}
void Sensor::obstacleDistance(int index){
  // the sensor returns 0 if it does not pick up (distance too great), it is therefore necessary to check
  if ultrasonicSensor()<distanceMin && ultrasonicSensor()>0)){
    tab[index]=true;
  }
  else{
    tab[index]=false;
  }
}

void Sensor::mouvementIfObstacle(){
  //Obstacle left right and middle or left and right: turn 135° to the right
  if tab[0] && tab[2]){
    avoidanceObstacle(3,31);
  } 
  //Left obstacle: turns 90° to the right
  else if (tableau[1] && tableau[2]){
    avoidanceObstacle(2,21);
  }
  //Left and middle or middle obstacle: turn 90° to the right
  else if (tableau[1]){
    avoidanceObstacle(3,21);
  }
  //Left obstacle: turns 45° to the right
  else if (tableau[0]){
    avoidanceObstacle(3,10);
  }
  //Right obstacle: turns 45° to the left
  else if (tableau[2]){
    avoidanceObstacle(2,10);
  }
  
}
void Sensor::avoidanceObstacle(int typeMovement,int n){
  for (int i=0;i<n;i++){
    moving.movement(typeMovement);
  }
}