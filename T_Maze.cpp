/*
  T_Maze.h - Library for controlling an automated T-Maze.
  Created by Igor S. Vaz, October 25, 2018.
  Released into the public domain.
*/

#include "Arduino.h"
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include "T_Maze.h"

int mode = 0;

int trial_number = 0;
int trial_flag = 0;
int randomNumber = 0;
long TIME_TRIAL = 0,
TIME_LNP = 0, TIME_RNP = 0, TIME_MNP = 0,
TIME_SERVO_L_OPENED = 0, TIME_SERVO_L_CLOSED = 0, TIME_SERVO_R_OPENED = 0, TIME_SERVO_R_CLOSED = 0,
TIME_STEPPER_L = 0, TIME_STEPPER_R = 0;


//LCD PINS
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//Buzzer frequencies and pin
int frequencia1 = 1000;
int frequencia2 = 8000;
int buzzer = 13;


//Stepper motors variables
int dose = -28;
int pino_enable = 7;
int velocidade_motor = 300;
int aceleracao_motor = 300;
AccelStepper motor2(1,40,41 );
AccelStepper motor1(1,39,38 );

//Servos Variables
int right_servo_state = 0;
int left_servo_state = 0;
int LeftServoState = 0;
int RightServoState = 0;
Servo Left_Servo;
Servo Right_Servo;

//IR SENSORS PINS
int LNP = 10 ; //Left Nose-Poke
int MNP = 9; //Middle Nose-Poke
int RNP = 8 ; //Right Nose-Poke
int MNP_State = 0;
int LNPvalue;
int MNPvalue;
int RNPvalue;

//keypad Variables
const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



byte led1Pin = 38;
byte led2Pin = 40;


//PinMode Setups
T_Maze::T_Maze(){
pinMode(LNP,INPUT);
pinMode(MNP,INPUT);
pinMode(RNP,INPUT);
pinMode(buzzer,OUTPUT);
digitalWrite(LNP, HIGH); //sets internal pull-up resistor due to open colector sensor configuration
digitalWrite(MNP, HIGH);
digitalWrite(RNP, HIGH);
pinMode(pino_enable, OUTPUT);
motor1.setMaxSpeed(velocidade_motor);
motor1.setSpeed(velocidade_motor);
motor1.setAcceleration(aceleracao_motor);

motor2.setMaxSpeed(velocidade_motor);
motor2.setSpeed(velocidade_motor);
motor2.setAcceleration(aceleracao_motor);
lcd.begin(20, 4); //LCD CONFIG

pinMode(led1Pin, OUTPUT);              // Sets the digital pin as output.
pinMode(led2Pin, OUTPUT);

}

void T_Maze::SD_setup()
{

Left_Servo.attach(12);
Right_Servo.attach(11);
Left_Servo.write(80); // closed
Right_Servo.write(85); // closed

lcd.setCursor(0,0);
lcd.print("Initializing SD card...");
delay(1500);
lcd.setCursor(0,2);
lcd.print("initialization done.");
delay(2500);
lcd.clear();
lcd.setCursor(4,0);
lcd.print("SELECT MODE");
lcd.setCursor(0,1);
lcd.print("Mode1 (1), Mode2 (2)");
lcd.setCursor(0,2);
lcd.print("Mode3 (3), Mode4 (4)");
lcd.setCursor(0,3);
lcd.print("Mode5 (5)");
}

//Function to read sensor values
void T_Maze::readsensors()
{
LNPvalue = digitalRead(LNP);
MNPvalue = digitalRead(MNP);
RNPvalue = digitalRead(RNP);
}

void T_Maze::keypad_control()
{
    char key = keypad.getKey();
    //Sends 1000 hz tone
    if (key == 'C'){
          tone(buzzer, 1000); // Send 1KHz sound signal...
          delay(1000);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
    }

    //Sends 8000 hz tone
    if (key == 'D'){
          tone(buzzer, 8000); // Send 1KHz sound signal...
          delay(1000);        // ...for 1 sec
          noTone(buzzer);     // Stop sound...
    }

    //configs for servos ================================================
    if (key == 'A'){
        if(left_servo_state == 0){
            Left_Servo.write(80);
            left_servo_state = 1;
        }

        else{
            Left_Servo.write(160);
            left_servo_state = 0;
        }
    }

    if (key == 'B'){
        if(right_servo_state == 0){
            Right_Servo.write(85);
            right_servo_state = 1;
        }

        else{
            Right_Servo.write(145);
            right_servo_state = 0;
       }
    }
    //======================================================================

    //config to select operation mode =====================================
    if ((key == '1') && (mode == 0)){
        lcd.clear();
        mode = 1;
        lcd.setCursor(2,1);
        lcd.print("MODE 1 SELECTED");
        delay(3000);
        lcd.clear();
    }

    if ((key == '2') && (mode == 0)){
        lcd.clear();
        mode = 2;
        lcd.setCursor(2,1);
        lcd.print("MODE 2 SELECTED");
        delay(3000);
        lcd.clear();
    }

    if ((key == '3') && (mode == 0)){
        lcd.clear();
        mode = 3;
        lcd.setCursor(2,1);
        lcd.print("MODE 3 SELECTED");
        delay(3000);
        lcd.clear();
    }

    if ((key == '4') && (mode == 0)){
        lcd.clear();
        mode =4;
        lcd.setCursor(2,1);
        lcd.print("MODE 4 SELECTED");
        delay(3000);
        lcd.clear();
    }

    if ((key == '5') && (mode == 0)){
        lcd.clear();
        mode = 5;
        lcd.setCursor(1,2);
        lcd.print("MODE 5 SELECTED");
        delay(3000);
        lcd.clear();
    }

    if (key == '*'){
        lcd.clear();
        mode = 0;
        trial_flag = 0;
        randomNumber = 0;
        trial_number = 0;
        MNP_State = 0;
        lcd.setCursor(4,0);
        lcd.print("SELECT MODE");
        lcd.setCursor(0,1);
        lcd.print("Mode1 (1), Mode2 (2)");
        lcd.setCursor(0,2);
        lcd.print("Mode3 (3), Mode4 (4)");
        lcd.setCursor(0,3);
        lcd.print("Mode5 (5)");

    }


    //configs for left pump ================================================

    //backs all the way
      if (key == '7'){
          digitalWrite(pino_enable, LOW);
          motor1.runToNewPosition(motor1.currentPosition()+2000);
          digitalWrite(pino_enable, HIGH);
    }

    //backs a little
     if (key == '6'){
          digitalWrite(pino_enable, LOW);
          motor1.runToNewPosition(motor1.currentPosition()+200);
          digitalWrite(pino_enable, HIGH);
    }

    // goes forward 2000
     if (key == '8'){
          digitalWrite(pino_enable, LOW);
          motor1.runToNewPosition(motor1.currentPosition()-280);
          digitalWrite(pino_enable, HIGH);
    }

    //delivers one reward
    if (key == '0'){
          digitalWrite(pino_enable, LOW);
          motor1.runToNewPosition(motor1.currentPosition()-28);
          digitalWrite(pino_enable, HIGH);
    }

    //======================================================================



    //configs for right pump ================================================

     if (key == '9'){
          digitalWrite(pino_enable, LOW);
          motor2.runToNewPosition(motor2.currentPosition()+2000);
          digitalWrite(pino_enable, HIGH);
    }

    if (key == '#'){
          digitalWrite(pino_enable, LOW);
          motor2.runToNewPosition(motor2.currentPosition()+200);
          digitalWrite(pino_enable, HIGH);
    }

     if (key == '#'){
          digitalWrite(pino_enable, LOW);
          motor2.runToNewPosition(motor2.currentPosition()-280);
          digitalWrite(pino_enable, HIGH);
    }

    //delivers one reward
    if (key == '#'){
          digitalWrite(pino_enable, LOW);
          motor2.runToNewPosition(motor2.currentPosition()-28);
          digitalWrite(pino_enable, HIGH);
    }

}





//MODE 1 =============================
void T_Maze::trial_init_mode1()
{

//myFile = SD.open("MODE1.txt", FILE_WRITE);
TIME_TRIAL = millis();
++trial_number;

lcd.setCursor(2,0);
lcd.print("TRIAL ");
lcd.print(trial_number);
lcd.print(" STARTED");

//generates 1 or 2 randomly
randomNumber = random(1,3);
}


void T_Maze::left_side_mode1()
{
//Reads Sensor Values-----------
LNPvalue = digitalRead(LNP);

if (LeftServoState == 0){

    Left_Servo.write(160); //opens left hatch
    TIME_SERVO_L_OPENED = millis();

    lcd.setCursor(1,1);
    lcd.print("Left Hatch Opened");

    LeftServoState = 1;
}

if(LNPvalue == 0){     //receives reward

    TIME_LNP = millis();

    lcd.setCursor(1,2);
    lcd.print("Left NP Activated");

    digitalWrite(pino_enable, LOW);
    motor1.runToNewPosition(motor1.currentPosition()-28);
    digitalWrite(pino_enable, HIGH);

    //pump simulated with leds
    digitalWrite(led1Pin,HIGH);
    delay(2000);
    digitalWrite(led1Pin,LOW);


    TIME_STEPPER_L = millis();

    lcd.setCursor(1,3);
    lcd.print("Reward Delivered");

    delay(5000);
    lcd.clear();


    Left_Servo.write(80);//closes left hatch
    TIME_SERVO_L_CLOSED = millis();


    LeftServoState = 0;
    randomNumber = 0;
    delay(3000);
    //waits 5 seconds before trial begins
    //delay(5000);

    /*
    myFile.print("Trial ");
    myFile.print(trial_number);
    myFile.print(" Started at: ");
    myFile.print(TIME_TRIAL);
    myFile.println(" ms -------------------------------------------------");
    myFile.println();
    myFile.println();
    delay(750);

    myFile.print("Left hatch opened: ");
    myFile.print(TIME_SERVO_L_OPENED);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("Left nose-poke activated: ");
    myFile.print(TIME_LNP);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("reward delivered: ");
    myFile.print(TIME_STEPPER_L);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("left hatch closes: ");
    myFile.print(TIME_SERVO_L_CLOSED);
    myFile.println(" ms");
    myFile.println();
    delay(1000);

    myFile.close();
    delay(1000); */
}

}


void T_Maze::right_side_mode1()
{
//Reads Sensor Values-----------
RNPvalue = digitalRead(RNP);

if (RightServoState == 0){
    Right_Servo.write(145); //opens right hatch
    TIME_SERVO_R_OPENED = millis();

    lcd.setCursor(1,1);
    lcd.print("Right Hatch Opened");

    RightServoState = 1;
}

if(RNPvalue == 0){  //receives reward
    TIME_RNP = millis();

    lcd.setCursor(1,2);
    lcd.print("Right NP Activated");

    digitalWrite(pino_enable, LOW);
    motor2.runToNewPosition(motor2.currentPosition()-28);
    digitalWrite(pino_enable, HIGH);


     //pump simulated with leds
    digitalWrite(led2Pin,HIGH);
    delay(2000);
    digitalWrite(led2Pin,LOW);

    TIME_STEPPER_R = millis();

    lcd.setCursor(1,3);
    lcd.print("Reward Delivered");

    delay(3000);
    lcd.clear();

    Right_Servo.write(85);
    TIME_SERVO_R_CLOSED = millis();

    randomNumber = 0;
    RightServoState = 0;
    delay(3000);
/*
    myFile.print("Trial ");
    myFile.print(trial_number);
    myFile.print(" Started at: ");
    myFile.print(TIME_TRIAL);
    myFile.println(" ms -------------------------------------------------");
    myFile.println();
    myFile.println();
    delay(750);

    myFile.print("Right Hatch Opened: ");
    myFile.print(TIME_SERVO_R_OPENED);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("Right Nose-Poke Activated: ");
    myFile.print(TIME_RNP);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("Reward Delivered: ");
    myFile.print(TIME_STEPPER_R);
    myFile.println(" ms");
    myFile.println();
    delay(750);

    myFile.print("Right Hatch Closed: ");
    myFile.print(TIME_SERVO_R_CLOSED);
    myFile.println(" ms");
    myFile.println();
    delay(1000);

    myFile.close();
    delay(1000);
 */
}

}



//MODE 2 ==============================
void T_Maze::trial_init_mode2()
{
TIME_TRIAL = millis();
++trial_number;
//myFile = SD.open("MODE2.txt", FILE_WRITE);

lcd.setCursor(2,0);
lcd.print("TRIAL ");
lcd.print(trial_number);
lcd.print(" STARTED");

trial_flag = 1;
}

void T_Maze::MNP_mode2()
{
TIME_MNP = millis();

tone(buzzer, frequencia1); // Send 1KHz sound signal...
delay(1000);        // ...for 1 sec
noTone(buzzer);     // Stop sound...

Left_Servo.write(160); //opens left hatch
TIME_SERVO_L_OPENED = millis();

lcd.setCursor(1,1);
lcd.print("Left Hatch Opened");

MNP_State = 1;
}

void T_Maze::LNP_activated_reward()
{
TIME_LNP = millis();

lcd.setCursor(1,2);
lcd.print("Left NP Activated");

digitalWrite(pino_enable, LOW);
motor1.runToNewPosition(motor1.currentPosition()-28);
digitalWrite(pino_enable, HIGH);

//pump simulated with leds
digitalWrite(led1Pin,HIGH);
delay(2000);
digitalWrite(led1Pin,LOW);


TIME_STEPPER_L = millis();

lcd.setCursor(1,3);
lcd.print("Reward Delivered");

delay(3000);
lcd.clear();


Left_Servo.write(80);//closes left hatch
TIME_SERVO_L_CLOSED = millis();

MNP_State = 0;
trial_flag = 0;
randomNumber = 0;
LeftServoState = 0;

//waits 5 seconds before trial begins
delay(5000);

//myFile.print("Trial ");
//myFile.print(trial_number);
//myFile.print(" Started at: ");
//myFile.print(TIME_TRIAL);
//myFile.println(" ms -------------------------------------------------");
//myFile.println();
//myFile.println();
//delay(750);
//
//myFile.print("Left hatch opened: ");
//myFile.print(TIME_SERVO_L_OPENED);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("Left nose-poke activated: ");
//myFile.print(TIME_LNP);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("reward delivered: ");
//myFile.print(TIME_STEPPER_L);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("left hatch closes: ");
//myFile.print(TIME_SERVO_L_CLOSED);
//myFile.println(" ms");
//myFile.println();
//delay(1000);
//
//myFile.close();
//delay(1000);
}



//MODE 3
void T_Maze::trial_init_mode3()
{
TIME_TRIAL = millis();
++trial_number;
//myFile = SD.open("MODE3.txt", FILE_WRITE);

lcd.setCursor(2,0);
lcd.print("TRIAL ");
lcd.print(trial_number);
lcd.print(" STARTED");

trial_flag = 1;
}

void T_Maze::MNP_mode3()
{
TIME_MNP = millis();

tone(buzzer, frequencia2); // Send 8KHz sound signal...
delay(1000);        // ...for 1 sec
noTone(buzzer);     // Stop sound...

Right_Servo.write(145); //opens left hatch
TIME_SERVO_R_OPENED = millis();

lcd.setCursor(1,1);
lcd.print("Right Hatch Opened");

MNP_State = 1;
}


void T_Maze::RNP_activated_reward()
{
TIME_RNP = millis();

lcd.setCursor(1,2);
lcd.print("Right NP Activated");

digitalWrite(pino_enable, LOW);
motor2.runToNewPosition(motor2.currentPosition()-28);
digitalWrite(pino_enable, HIGH);

//pump simulated with leds
digitalWrite(led2Pin,HIGH);
delay(2000);
digitalWrite(led2Pin,LOW);


TIME_STEPPER_R = millis();

lcd.setCursor(1,3);
lcd.print("Reward Delivered");

delay(3000);
lcd.clear();


Right_Servo.write(85);//closes left hatch
TIME_SERVO_R_CLOSED = millis();

MNP_State = 0;
trial_flag = 0;
randomNumber = 0;
RightServoState = 0;

//waits 5 seconds before trial begins
delay(5000);

//myFile.print("Trial ");
//myFile.print(trial_number);
//myFile.print(" Started at: ");
//myFile.print(TIME_TRIAL);
//myFile.println(" ms -------------------------------------------------");
//myFile.println();
//myFile.println();
//delay(750);
//
//myFile.print("Right hatch opened: ");
//myFile.print(TIME_SERVO_R_OPENED);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("Right nose-poke activated: ");
//myFile.print(TIME_RNP);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("reward delivered: ");
//myFile.print(TIME_STEPPER_R);
//myFile.println(" ms");
//myFile.println();
//delay(750);
//
//myFile.print("Right hatch closes: ");
//myFile.print(TIME_SERVO_R_CLOSED);
//myFile.println(" ms");
//myFile.println();
//delay(1000);
//
//myFile.close();
//delay(1000);
}


//MODE 4
void T_Maze::trial_init_mode4()
{
TIME_TRIAL = millis();
++trial_number;
//myFile = SD.open("MODE4.txt", FILE_WRITE);

lcd.setCursor(2,0);
lcd.print("TRIAL ");
lcd.print(trial_number);
lcd.print(" STARTED");

trial_flag = 1;
}


void T_Maze::MNP_mode4()
{
TIME_MNP = millis();

randomNumber = random(1,3);

if (randomNumber == 1){
    tone(buzzer, frequencia1); // Send 1KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...

    if (LeftServoState == 0){
        Left_Servo.write(160); //opens right hatch
        TIME_SERVO_L_OPENED = millis();

        lcd.setCursor(1,1);
        lcd.print("Left Hatch Opened");

        LeftServoState = 1;
     }
}
if (randomNumber == 2){
    tone(buzzer, frequencia2); // Send 8KHz sound signal...
    delay(1000);        // ...for 1 sec
    noTone(buzzer);     // Stop sound...

    if (RightServoState == 0){
        Right_Servo.write(145); //opens right hatch
        TIME_SERVO_R_OPENED = millis();

        lcd.setCursor(1,1);
        lcd.print("Right Hatch Opened");

        RightServoState = 1;
     }
}

MNP_State = 1;
}






//============================================================= TEST WITH KEYPAD =================================================================
//keypad.addEventListener(keypadEvent);
//char key = keypad.getKey();
// Taking care of some special events.
//void T_Maze::keypadEvent(KeypadEvent key){
//    switch (keypad.getState()){
////    case PRESSED:
////        if (key == '#') {
////            digitalWrite(led1Pin,!digitalRead(led1Pin));
////            led1Pin_state = digitalRead(led1Pin);        // Remember LED state, lit or unlit.
////        }
////        break;
//
//    case RELEASED:
//        if (key == '*') {
//            digitalWrite(led1Pin,LOW);
//            digitalWrite(led2Pin,HIGH);
//        }
//        break;
//
//    case HOLD:
//        if (key == '*') {
//            digitalWrite(led1Pin,HIGH);
//            digitalWrite(led2Pin,LOW);
//        }
//        break;
//    }
//}
