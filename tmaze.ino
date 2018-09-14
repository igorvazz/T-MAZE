#include <Servo.h>

//servos-------
Servo myservo1;  
Servo myservo2;
//-------------


//STEPPER----------------------

#include <AccelStepper.h>

int velocidade_motor = 500; 
int aceleracao_motor = 500;
int sentido_horario = 0;
int sentido_antihorario = 0;
int numero = 0; 

// Definicao pino ENABLE
int pino_enable = 7;

// Definicao pinos STEP e DIR
AccelStepper motor1(1,10,9 );


int dose = 25;

//------------------------------




//IR SENSORS PINS----------------
int LNP = 26 ; //Left Nose-Poke
int MNP = 28 ; //Middle Nose-Poke
int RNP = 30 ; //Right Nose-Poke


//--------------------------------

int randomNumber = 0;
int RandomnumberLED1 = 42;
int RandomnumberLED2 = 44;

//Buzzer--------
int frequencia1;
int frequencia2;
int buzzer = 12;
//--------------------------------




 
void setup() {

//Buzzer pin-------
pinMode(11,OUTPUT);
//-----------------


//STEPPER---------------------------------
pinMode(pino_enable, OUTPUT);
// Configuracoes iniciais motor de passo
motor1.setMaxSpeed(velocidade_motor);
motor1.setSpeed(velocidade_motor);
motor1.setAcceleration(aceleracao_motor);
//----------------------------------------  

//IR SENSORS-------
pinMode(LNP,INPUT);
pinMode(MNP,INPUT);
pinMode(RNP,INPUT);
digitalWrite(LNP, HIGH); //sets internal pull-up resistor due to open colector sensor configuration
digitalWrite(MNP, HIGH);
digitalWrite(RNP, HIGH);
//-----------------


pinMode(RandomnumberLED1,OUTPUT);
pinMode(RandomnumberLED2,OUTPUT);


//servos pins -----
myservo1.attach(5);  
myservo2.attach(6);
myservo1.write(25);
myservo2.write(80);
//-----------------

randomSeed(analogRead(A0)); //Generate random value with analog value from pin A0

}
 
void loop() {

//Reads Sensor Values-----------
int LNPvalue = digitalRead(LNP);
int MNPvalue = digitalRead(MNP);
int RNPvalue = digitalRead(RNP);
//------------------------------



if((MNPvalue == 0)&&(randomNumber == 0 )){
  
  tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
  noTone(buzzer);     // Stop sound...
  
  randomNumber = random(1,3);
  
  if (randomNumber == 1){
   digitalWrite(RandomnumberLED1,HIGH);
  }
  if (randomNumber == 2){
   digitalWrite(RandomnumberLED2,HIGH);
  }
  
  myservo1.write(90);
  myservo2.write(140);
  
  } 


if((LNPvalue == 0)&&(randomNumber == 1)){     //receives reward
  
 

  digitalWrite(pino_enable, LOW);
  //motor1.moveTo(13);
  //motor1.run();
  

  motor1.runToNewPosition(dose);
  dose += 25;

  digitalWrite(pino_enable, HIGH);

  delay(3000);
  myservo1.write(25);
  myservo2.write(80);
  
  randomNumber = 0;
  digitalWrite(RandomnumberLED1,LOW);
  digitalWrite(RandomnumberLED2,LOW);

  }

if((LNPvalue == 0)&&(randomNumber == 2)){  //doesnt receive reward
  
  delay(3000);
  myservo1.write(25);
  myservo2.write(80);
  randomNumber = 0;
  digitalWrite(RandomnumberLED1,LOW);
  digitalWrite(RandomnumberLED2,LOW);

  }
  
if((RNPvalue == 0)&&(randomNumber == 2)){  //receives reward

  
  digitalWrite(pino_enable, LOW);
  //motor1.moveTo(13);
  //motor1.run();
  

  motor1.runToNewPosition(dose);
  dose += 25;

  digitalWrite(pino_enable, HIGH);

  delay(3000);
  myservo1.write(25);
  myservo2.write(80);

  
  randomNumber = 0;
  digitalWrite(RandomnumberLED1,LOW);
  digitalWrite(RandomnumberLED2,LOW);

  }

if((RNPvalue == 0)&&(randomNumber == 1)){  //doesnt receive reward

  delay(3000);
  myservo1.write(25);
  myservo2.write(80);
  randomNumber = 0;
  digitalWrite(RandomnumberLED1,LOW);
  digitalWrite(RandomnumberLED2,LOW);
  }

}
