//PixyCam code - Start 3 feb 2019

#include <Pixy2.h>
#include <SPI.h>
#include <Pixy2CCC.h>  
#include <Servo.h>

Servo myservo;
const int leftForward = 10;
const int leftBackward = 3;
const int rightForward = 6;
const int rightBackward = 5;
int backArr[120][2];
int backArrLen = 0;
bool marble_detected = true;

// This is the main Pixy object 
Pixy2 pixy;

void left_turn(int x);
void right_turn(int x);
void straight();
void returnToStart(int arr[][2], int x);

void setup(){
  myservo.attach(9);
  myservo.write(90);// move servos to center position -> 90°
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
  analogWrite(leftForward, 0);
  analogWrite(leftBackward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBackward, 0);
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
      if (pixy.ccc.blocks[k].m_signature == 3 && marble_detected){
          for(int j=90; j>=0; j--){
          myservo.write(j);// move servos to center position -> 0°
          delay(15);
          }
          marble_detected = false;
          
          Serial.print("marble_detected value - ");
          Serial.print(marble_detected);
          Serial.print("saw yel0 marble /n"); 
      }
      
      if (pixy.ccc.blocks[k].m_signature != 3 && !marble_detected){
           for(int j=90; j<=180; j++){
            myservo.write(j);// move servos to center position -> 0°
            delay(15);
          }
          marble_detected = true;
          Serial.print("marble_detected value - ");
          Serial.print(marble_detected);
          Serial.print("see other marble /n");
      }
      
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
  analogWrite(rightForward,  0);
  analogWrite(rightBackward, 255);
  analogWrite(leftForward, 255);
  analogWrite(leftBackward, 0);
  Serial.print("start turning left: x=");
  Serial.print(x);
  delay(del*10);
  Serial.print(" delay =");
  Serial.print(del*10);
  Serial.print("\nend turning left\n");
}

void right_turn(int x){
  int del = x - 158;
  analogWrite(rightForward, 255);
  analogWrite(rightBackward, 0);
  analogWrite(leftForward, 0);
  analogWrite(leftBackward, 255);
  Serial.print("start turning right: x =");
  Serial.print(x);
  delay(del*10);
  Serial.print(" delay =");
  Serial.print (del*10);
  Serial.print("\nend turning right\n");
}

void straight(){
  analogWrite(leftForward, 0);
  analogWrite(leftBackward, 255);
  analogWrite(rightForward, 255);
  analogWrite(rightBackward, 0);
  Serial.print("start straight: del = 1000\n");
  delay(1000);
  Serial.print("end straight\n");
}

void reverse(){
  analogWrite(leftForward, 255);  
  analogWrite(leftBackward, 0);
  analogWrite(rightForward, 0);
  analogWrite(rightBackward, 255);
  Serial.print("start reverse: del = 1000\n");
  delay(1000);
  Serial.print("end reverse\n");
}

void returnToStart(int arr[][2], int len){
  for(int i=len-1; i>-1; i--){
    if(arr[i][2] == 1){
      right_turn(arr[i][1]);
    }
    else if(arr[i][2] == 2){
      left_turn(arr[i][1]);
    }
    else{ 
      reverse();
    }
  } 
}

 
