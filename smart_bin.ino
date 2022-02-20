//--------------------------------------------------------------
//  AUTHOR : DIVINE LETSU
//  EMAIL  : divinellive@gmail.com
//  Description : Arduino nano and c++ smart waste bin
//--------------------------------------------------------------


//--------------------------------------------------------------
//[ SERVO DECLARATIONS----------]
#include <Servo.h>
Servo myServo;
char servoPin=6;
unsigned char servoPosition=0;


//[ ULTRASONIC DECLARATIONS-----]
char trigPin=4;
char echoPin=5;
unsigned char lidThreshold=60;
//[SAMPLING ULTRASONIC VARIABLES]
char aboveThreshold =0;
char belowThreshold=0;
char sampleLimit=100;


//[ PHOTO DECLARATIONS----------]
char photoPin = A0;
//[SAMPLING LIGHT VARIABLES]
char beyondThreshold =0;
char beneathThreshold=0;
char photoSampleLimit=100;


//[  BUZZER DECLARATIONS--------]
char buzzerPin = 3;


//[  LED DECLARATIONS-----------]
char redPin = 2;
char greenPin = 7;
char bluePin = 8;
char activeLed=0;
//--------------------------------------------------------------




//--------------------------------------------------------------
//[ SETUP CODE **************** ]
//-------------------------------
void setup() {
  // put your setup code here, to run once:

  // SERVO SETUP ---------]
  myServo.attach(servoPin);

  // ULTRASONIC SETUP-----]
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  // LASER SETUP ---------]
  pinMode(photoPin,INPUT);
}
//--------------------------------------------------------------




//--------------------------------------------------------------
//[ VOID LOOP **************** ]
//------------------------------
void loop() {
// put your main code here, to run repeatedly:
  servoLogic();
  photoLogic();
}
//--------------------------------------------------------------




//[ MY FUNCTIONS------------------------------------------------------

//[ SERVO CONTROL ******** ]
//--------------------------
void servoLogic(){
  //[INCREASING SAMPLING VARIABLES] 
  //-------------------------------
  if(getdistance() > lidThreshold){
    aboveThreshold+=1;
  }
  else{
    belowThreshold+=1;
  }
  
  //[REACTING TO SAMPLED VARIABLES] 
  //-------------------------------
  if(aboveThreshold >= sampleLimit){ //CLOSING LID-------------
    servoPosition=myServo.read();
    for(unsigned char i=servoPosition;servoPosition>93;servoPosition--){
        delay(8);
        myServo.write(servoPosition);
    }
    aboveThreshold=0;
    belowThreshold=0;
  }else if(belowThreshold >= sampleLimit){ //OPENING LID-------
    noTone(buzzerPin);
    servoPosition=myServo.read();
    for(unsigned char i=servoPosition;servoPosition<180;servoPosition++){
      delay(4);
      myServo.write(servoPosition);
    }
    aboveThreshold=0;
    belowThreshold=0;
  }
}  


//[ LASER SENSING ******** ]
//--------------------------
void photoLogic(){
  
  //[INCREASING SAMPLING VARIABLES] 
  //-------------------------------
  if(analogRead(photoPin) > 13){
    beyondThreshold+=1;
  }
  else{
    beneathThreshold+=1;
  }
  
  //[REACTING TO SAMPLED VARIABLES] 
  //-------------------------------
  if(beyondThreshold>=photoSampleLimit){ //[BIN IS FULL]
    if( myServo.read()<98){ //[BIN IS CLOSED]
      tone(buzzerPin, 100);
    }
    if(activeLed==0){
        led('r');
        activeLed++;
      }else if(activeLed==1){
        led('p');
        activeLed++;
      }else if(activeLed==2){
        led('b');
        activeLed++;
      }else if(activeLed==3){
        led('p');
        activeLed=0;
      }
    beyondThreshold=0;
    beneathThreshold=0;
  }else if(beneathThreshold>=photoSampleLimit){  //[BIN NOT FULL]
    noTone(buzzerPin);
    led('g');
    beyondThreshold=0;
    beneathThreshold=0;
  }
}


//[ ULTRASONIC SENSING ******** ]
//-------------------------------
  int getdistance(){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  int duration = pulseIn(echoPin,HIGH);
  return duration*0.034/2;
 }


//[ STATUS LED *************** ]
//------------------------------
void led(char color){
  switch(color){
    case 'r':
    digitalWrite(redPin,HIGH);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,LOW);
    break;
    case 'g':
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,HIGH);
    digitalWrite(bluePin,LOW);
    break;
    case 'b':
    digitalWrite(redPin,LOW);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,HIGH);
    break;
    case 'p':
    digitalWrite(redPin,HIGH);
    digitalWrite(greenPin,LOW);
    digitalWrite(bluePin,HIGH);
    break;
  }
} 
