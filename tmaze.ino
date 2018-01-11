#include <Servo.h>

const int buzzer = 5; //buzzer to arduino pin 9
int IR1 = 2 ;
int IR2 = 3 ;
int IR3 = 7 ;
Servo myservo1;  // servo da porta central
int pos1;
Servo myservo2;
int pos2;

long Tbase,Te,Td;

int randomNumber = 0;

int x,y;
void setup(){
  Serial.begin(9600);
  pinMode(IR1,INPUT);
  pinMode(IR2,INPUT);
  pinMode(IR3,INPUT);
  //Serial.begin(115200);
  myservo1.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(10);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  randomSeed(analogRead(A0));
}

void loop(){
  int IR1value = digitalRead(2);
  int IR2value = digitalRead(3);
  int IR3value = digitalRead(7);
 // Serial.println(randomNumber);

if(IR1value == 0){
  Tbase = millis();
  tone(buzzer, 5000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  randomNumber = random(1,3);
  Serial.println(Tbase);
  } 

if(randomNumber == 1){
  myservo1.write(180);
  randomNumber = 0;
  }

if(randomNumber == 2){
  myservo2.write(180);
  randomNumber = 0;
  }

if(IR2value == 0){
  Te = millis();
  delay(3000);
  myservo1.write(0);
  Serial.println(Te);
  }
  
if(IR3value == 0){
  Td = millis();
  delay(3000);
  myservo2.write(0);
  Serial.println(Td);
  }

  
}
