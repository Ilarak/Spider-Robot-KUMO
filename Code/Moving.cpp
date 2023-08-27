#include "Moving.h"

//Constructeur
Moving::Moving(int speed){
  setSpeed(speed);
}
int Moving::setSpeed(int Speed){
  this->speed=speed;
}
int Moving::getSpeed(){
  return (this->speed);
}

/*
To use the SSC-32 card that moves the servos, you have to do:
Serial.println(#pin Pposition Ttime);
With -pin : the servomotor hooked to this pin that we want to move
     -position : the position we want the servomotor to be (between 500 and 2500)
     -time: the time it takes the servomotor to go to this position
*/

//Function which has a list of pins and a position as input and which for each pin in the list asks to go to a position
void Moving::move(int list,float positio){
  Serial.print("#");
  Serial.print(list);
  Serial.print(" P");
  Serial.print(angle(list,positio)); // we use angle so that the displacement is symmetrical
  Serial.print(" T");
  Serial.println(500);
}

//Function which has a pin list and a position list as input and which for each pin in the list asks to go to a position
void Moving::moveList(int list[],float positio[], int size, int time){
  //us map(value, fromLow, fromHigh, toLow, toHigh) 
  for(int i=0; i<taille; i++){
    Serial.print("#");
    Serial.print(list[i]);
    thePosition = map(positio[i],-90,90,500,2500); // map converts input angles (-90 to 90) to the position that the ssc-32 map requests (500 to 2500)
    Serial.print(" P");
    Serial.print(angle(list[i],thePosition));
  }
  Serial.print(" T");
  Serial.println(time);
}

// Allows servo motors to rotate symmetrically
double Moving::angle(int pin ,float positio){
  //returns symmetry relative to 1500 for right servomotors
  if (pin==16 or pin==20 or pin==24){
    return (3000-positio);
  }
  if (pin==2 or pin==6 or pin==10 or pin==18 or pin==22 or pin==26){
    return (positio + 400);
  }
  //servo motors are not exactly at the same angle
  if(pin==25){return (positio+40);}
  if(pin==14){return (positio-100);}
  if(pin==21){return (positio+90);}
  if(pin==17){return (positio-20);}

  return positio;
}

//initializes the servomotors at a certain position to then be able to move it forward (and test that all the servomotors work well)
void Moving::testServos(){
  for (int i=0; i<SIZE6PIN;i++){
    movepinFront[i],2000);
    delay(500);
    movepinFront[i],1500);
    delay(500);
    move(pinMid[i],2000);
    delay(500);
    move(pinMid[i],1500);
    delay(500);
    move(pinBack[i],2000);
    delay(500);
    move(pinBack[i],1500);
    delay(500);
  }
}
void Moving::getUp(){
  for (int i=0; i<6 ; i++){
    listPosition[i]=listNormal[i];
  }
  for (int i=0;i<4;i++){
    listPositionBetaTeta[i]=0;
  }
  movingAction();
  moveList(allPin,list,SIZEALLPIN,time);
}

  

void Moving::checkLeft(){
  float positio[SIZE6PIN]={40,40,40,-40,-40,-40};
  moveList(pinFrony,positio,SIZE6PIN,time);
}

void Moving::checkWrite(){
  float positio[SIZEE6PIN]={-40,-40,-40,40,40,40};
  moveList(pinDevant,positio,TAILLE6PIN,temps);
}

void Moving::checkMilieu(){
  normalPosition();
}

void Moving::normalPosition(){
  //If all the legs touch the ground, they are lifted for movement
  if (listPosition[0]==normBeta && listPosition[2]==normBeta && !compareList()){
    raisePaw();
  }
  for (int i=0; i<6 ; i++){
    listPosition[i]=listNormal[i];
  }
  for (int i=0;i<4;i++){
    listPositionBetaTeta[i]=listNormalBetaTeta[i];
  }
  deplacementAction();
  moveList(allPin,list,SIZEALLPIN,time);
}


void Moving::moveForward(bool var){
  if (var){
    for (int i=0; i<6; i++){
      listPosition[i]+=listAddition1[i]*speed;
    }
  }
  else{
    for (int i=0; i<6; i++){
      listPosition[i]-=listAddition1[i]*speed;
    }
  }
}
void Moving::moveBack(bool var){
 if (var){
    for (int i=0; i<6; i++){
      listPosition[i]-=listAddition1[i]*speed;
    }
  }
  else{
    for (int i=0; i<6; i++){
      listPosition[i]+=listAddition1[i]*speed;
    }
  }
}
void Moving::turnLeft(bool var){
  if (var){
    for (int i=0; i<6; i++){
      listPosition[i]+=listAddition2[i]*speed;
    }
  }
  else{
    for (int i=0; i<6; i++){
      listPosition[i]-=listAddition2[i]*speed;
    }
  }
}
void Moving::turnRight(bool var){
  if (var){
    for (int i=0; i<6; i++){
      listPosition[i]+=listAddition3[i]*speed;
    }
  }
  else{
    for (int i=0; i<6; i++){
      listPosition[i]-=listAddition3[i]*speed;
    }
  }
}
//allows to know if the movement is to move back(0), to move forwards (1), to turn left (2) or to turn right (3)
void Moving::movingChoice(int typeMoving,bool var){
  if (var){
    listPositionBetaTeta[0]+=beta;
    listPositionBetaTeta[1]+=teta;
  }
  else{
    listPositionBetaTeta[2]+=beta;
    listPositionBetaTeta[3]+=teta;
  }
  switch (typeMoving){
    case 0 :
      moveForward(var);
      break;
    case 1 :
      moveBack(var);
      break;
    case 2 :
      turnLeft(var);
      break;
    case 3 :
      turnRight(var);
      break;
  }
}
void Moving::movement(int typeMoving){
  //we check if before the robot was in the normal position
  if(compareList()){
    raisePaw();
    beforeNormal=true;
  }
  //if the 1st leg is on the ground :
  // "Yes" allows it to work example: both are on the ground, to know which one must move forward
  else if (listPositionBetaTeta[0]<=normBeta && yes){
    movingChoice(typeMoving,0);
  }
  //if the 2nd leg is on the ground :
  else if (listPositionBetaTeta[2]<=normBeta && !yes){
    movingChoice(typeMoving,1);
  }
  //If the leg is up to the maximum, change the list so that the leg goes down after
  if (!beforeNormal){
    if (listPositionBetaTeta[0]>=normBeta+40||listPositionBetaTeta[2]>=normBeta+40){ 
      beta=-20*speed;
      teta=-6*speed;  
    }
  }
  else{
    beforeNormal=false;
  }
  //If all the legs touch the ground, then the beta and theta angles must be positive to move forward
  if ((listPositionBetaTeta[0]<=normBeta)&&(listPositionBetaTeta[2]<=normBeta)){
    beta=20*speed;
    teta=6*speed;
    yes=!yes;
  } 
  //change the position of the spider
  movingAction();
  moveList(allPin,list,SIZEALLPIN,tmp);
}

void Moving::raisePaw(){
  listPositionBetaTeta[2]=normBeta+40;
  listPositionBetaTeta[3]=normTeta+12;
}

bool Moving::compareList(){
  for (int i=0;i<6;i++){
    if (listPosition[i]!=listNormal[i]){
      return false;
    }
  }
  if(listPositionBetaTeta[0]!=listNormalBetaTeta[0]||listPositionBetaTeta[2]!=listNormalBetaTeta[2]){
    return false;
  }  
  return true;
}

void Moving::movingAction(){
  k=0;
  for (int i=0;i<18;i++){
    if(i%3==0){
      list[i]=listPosition[k];
      k+=1;
    }
    else if (i%3==1 && i%2==0){
        list[i]=listPositionBetaTeta[2];
    }
    else if (i%3==1 && i%2==1){
      list[i]=listPositionBetaTeta[0];
    }
    else if (i%3==2 && i%2==0){
      list[i]=listPositionBetaTeta[1];
    }
    else{
      list[i]=listPositionBetaTeta[3];
    }
  }
}
 
void Moving::positionCommSwarm(){
  normalPosition();
  delay(100);
  moveList(allPin,listPositionSwarm,SIZEALLPIN,time);
}