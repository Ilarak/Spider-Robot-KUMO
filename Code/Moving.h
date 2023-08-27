#ifndef _MOVING_
#define _MOVING_
#include <Arduino.h>
#define SIZEALLPIN 18
#define SIZE6PIN 6

class Moving {
  public :
    int getSpeed();
    int setSpeed(int speed);

    int speed;  
    Moving(int speed); //Constructeur
    void testServos();
    void getUp();
    void moveForward(bool var);
    void moveBack(bool var);
    void turnLeft(bool var);
    void turnRight(bool var);
    void movement(int typeMoving);
    /* we have 0 : move back
               1 : move forward
               2 : turn left
               3 : turn right
    */
    void checkLeft();
    void checkRight();
    void checkMid();

    void normalPosition();
    void positionCommSwarm();
    
  private :
    void move(int list,float positio);   
    void moveList(int liste[],float positio[], int size ,int time);
    double angle(int pin, float positio); // allows the robot to advance symmetrically
    int thePosition; // uses to use the angle function in the cpp
    //These lists are useful for moving the spider. They correspond to all the pins attached to the servomotor.
    const int allPin[18] = {0,1,2,4,5,6,12,13,14,16,17,18,20,21,22,24,25,26};
    const int pinFront[6] ={0,4,12,16,20,24};
    const int pinMid[6] ={1,5,13,17,21,25};
    const int pinBack[6] ={2,6,14,18,22,26};

    int const normTeta = 20;
    int const normBeta = 10;

    int time = 600; 
    int tmp=200;

    int beta=-20;
    int teta=-6;
    // test with 2 lists
    const float listAddition1[6]={15,-11,15,-15,11,-15};
    const float listAddition2[6]={15,-11,15,15,-11,15};
    const float listAddition3[6]={-15,11,-15,-15,11,-15};

    float listPosition[6]={16,-3,-12,25,10,-16};
    int listPositionBetaTeta[4]={normBeta,normTeta,normBeta,normTeta};
    float listNormal[6]={16,-3,-12,25,10,-16};
    int listNormalBetaTeta[4]={normBeta,normTeta,normBeta,normTeta};
    int a=25;
    int b=10;
    float listPositionSwarm[18]={16,normBeta+a,normTeta+b,-3,normBeta-10,normTeta-60,-12,normBeta+a,normTeta+b,25,normBeta+a,normTeta,10,normBeta-10,normTeta-60,-16,normBeta+a,normTeta+b};

    bool yes=true; //lets you know when both legs are on the ground, which one to move forward (explained in the .cpp)
    bool beforeNormal=false;
    bool compareList();
    void raisePaw();
    void movingChoice(int typeMoving, bool var);
    void movingAction();
    float liste[18];
    int k;

};
#endif