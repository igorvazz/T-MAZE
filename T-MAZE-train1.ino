#include <Servo.h>

//servos-------
Servo myservo1;  
Servo myservo2;
int LeftServoState = 0;
int RightServoState = 0;
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
AccelStepper motor2(1,10,9 );
AccelStepper motor1(1,12,13 );

int dose1 = -7500;
int dose2 = 25;

//------------------------------



//IR SENSORS PINS----------------
int LNP = 26 ; //Left Nose-Poke
int RNP = 38 ; //Right Nose-Poke


//--------------------------------

int randomNumber = 0;

 
void setup() {


//STEPPER---------------------------------
pinMode(pino_enable, OUTPUT);

// Configuracoes iniciais motor de passo
motor1.setMaxSpeed(velocidade_motor);
motor1.setSpeed(velocidade_motor);
motor1.setAcceleration(aceleracao_motor);

motor2.setMaxSpeed(velocidade_motor);
motor2.setSpeed(velocidade_motor);
motor2.setAcceleration(aceleracao_motor);

//----------------------------------------  

//IR SENSORS-------
pinMode(LNP,INPUT);
pinMode(RNP,INPUT);
digitalWrite(LNP, HIGH); //sets internal pull-up resistor due to open colector sensor configuration
digitalWrite(RNP, HIGH);
//-----------------


//servos pins -----
myservo1.attach(5);  
myservo2.attach(6);
myservo1.write(120); // fechado
myservo2.write(120); // fechado
//myservo1.write(180); // aberto
//myservo2.write(180); // aberto

//-----------------

randomSeed(analogRead(A0)); //Generate random value with analog value from pin A0

}
 
void loop() {


//Reads Sensor Values-----------
int LNPvalue = digitalRead(LNP);
int RNPvalue = digitalRead(RNP);
//------------------------------

if (randomNumber == 0){
    //generates 1 or 2 randomly
    randomNumber = random(1,3);
    
    myservo1.write(110); // hatch closes
    myservo2.write(110); // hatch closes

    //waits 5 seconds before trial begins
    delay(5000);
}

  
if (randomNumber == 1){
    if (LeftServoState == 0){
    myservo1.write(180); //opens left hatch
    LeftServoState = 1;
    }
    if((LNPvalue == 0)&&(randomNumber == 1)){     //receives reward

      digitalWrite(pino_enable, LOW);
      motor1.runToNewPosition(dose1);
      dose1 -= 7500;
      digitalWrite(pino_enable, HIGH);

      delay(3000);
      myservo1.write(110);

      LeftServoState = 0;
      randomNumber = 0;
    }
    
}

if (randomNumber == 2){
    if (RightServoState == 0){
    myservo2.write(180); //opens right hatch
    RightServoState = 1;
    }
    
    if((RNPvalue == 0)&&(randomNumber == 2)){  //receives reward
  
      digitalWrite(pino_enable, LOW);
      motor2.runToNewPosition(dose2);
      dose2 += 25;
      digitalWrite(pino_enable, HIGH);

      delay(3000);
      myservo2.write(110);
  
      randomNumber = 0;
      RightServoState = 0;
    }  
}
  
}
