#include <Arduino.h>
#define TRIGGER_PIN 4  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 5// Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 250 // Maximum distance we want to ping for (in centimeters).
#define TAILLETABLEAU 3
#define distanceMin 50

class Sensor{
 public : 
    Capteurs(); 
    long ultrasonicSensor();
    int IRSensor();
    void movementCheckObstacle();
    void mouvementIfObstacle();
    
  private :
    void obstacleDistance(int index);
    bool tab[3];
    void avoidanceObstacle(int typeMovement,int n);    
};