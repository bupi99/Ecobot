//PixyCam code - Start 3 feb 2019

#include <Pixy2.h>
#include <SPI.h>
#include <Pixy2CCC.h>  

const int leftForward = 2;
const int leftBackward = 3;
const int rightForward = 4;
const int rightBackward = 5;
int backArr[120][2];
int backArrLen = 0;

// This is the main Pixy object 
Pixy2 pixy;

void left_turn(int x);
void right_turn(int x);
void straight();
void returnToStart(int arr[][2], int x);

void setup(){
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();
  for (int i=0; i<120; i++){
    for (int j=0; j<2; j++){
      backArr[i][j] = 0;
    }
  }
}



void loop(){ 
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, LOW);
  delay(1000);
   
   
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks){
    int k = -1;
    int x;
    int y;
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    
    for (int i=0; i<pixy.ccc.numBlocks; i++){
      x = pixy.ccc.blocks[i].m_x;
      y = pixy.ccc.blocks[i].m_y;
      int height = pixy.ccc.blocks[i].m_height;
      int width = pixy.ccc.blocks[i].m_width;
      if(height*width < 500){
        Serial.print("Marble Detected \n");
        k = i;
        break;
      }
      else if(height*width >= 500){
        Serial.print("Obstacle Detected: Area = ");
        Serial.print(height*width);
        Serial.print("\n");
      }
    }
    
    if (k != -1){
      for (int i=1; i <= 7 ; i++){      
        if(pixy.ccc.blocks[k].m_signature == i){
          pixy.ccc.blocks[k].print();
          if(x<140){
            left_turn(x);
            backArr[backArrLen][0] = 1;
            backArr[backArrLen][1] = x;
            backArrLen++;
          }
          else if(x>180){
            right_turn(x);
            backArr[backArrLen][0] = 2;
            backArr[backArrLen][1] = x;
            backArrLen++;
          }
          else{
            straight();
            backArr[backArrLen][0] = 0;
            backArrLen++;
          }       
        }
      }
    }
    
  }
  if(backArrLen >= 120){
    returnToStart(backArr, backArrLen);
  }
}
// End of Loop


void left_turn(int x){
  int del = 158-x;
  digitalWrite(rightForward,  LOW);
  digitalWrite(rightBackward, HIGH);
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  Serial.print("start turning left: x=");
  Serial.print(x);
  delay(del*10);
  Serial.print(" delay =");
  Serial.print(del*10);
  Serial.print("\nend turning left\n");
}

void right_turn(int x){
  int del = x - 158;
  digitalWrite(rightForward,  HIGH);
  digitalWrite(rightBackward, LOW);
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  Serial.print("start turning right: x =");
  Serial.print(x);
  delay(del*10);
  Serial.print(" delay =");
  Serial.print (del*10);
  Serial.print("\nend turning right\n");
}

void straight(){
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
  Serial.print("start straight: del = 1000\n");
  delay(1000);
  Serial.print("end straight\n");
}
void reverse(){
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
  Serial.print("start reverse: del = 1000\n");
  delay(1000);
  Serial.print("end reverse\n");
}

void returnToStart(int arr[][2], int len){
  for(int i=len-1; i>-1; i--){
    if(arr[i] == 1){
      right_turn(arr[i][1]);
    }
    else if(arr[i] == 2){
      left_turn(arr[i][1]);
    }
    else{ 
      reverse();
    }
  }
  
}
