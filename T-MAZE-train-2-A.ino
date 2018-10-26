//KEYPAD==================================================================================================================
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {31, 33, 35, 37}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {39, 41, 43, 45}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//=========================================================================================================================




//SD-CARD==================================================================================================================
#include <SPI.h>
#include <SD.h>

File myFile;

int trial_number = 0;
int trial_flag = 0;

long TIME_TRIAL = 0, 
TIME_LNP = 0, TIME_RNP = 0, TIME_MNP = 0, 
TIME_SERVO_L_OPENED = 0, TIME_SERVO_L_CLOSED = 0, TIME_SERVO_R_OPENED = 0, TIME_SERVO_R_CLOSED = 0,
TIME_STEPPER_L = 0, TIME_STEPPER_R = 0; 
//=========================================================================================================================

//DISPLAY-LCD===============================================
#include <LiquidCrystal.h>

const int rs = 46, en = 42, d4 = 22, d5 = 8, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//===========================================================



//servos=============================
#include <Servo.h>
int right_servo_state = 0;
int left_servo_state = 0;
Servo Left_Servo;  
Servo Right_Servo;
int LeftServoState = 0;
int RightServoState = 0;

//===================================


//STEPPER----------------------
#include <AccelStepper.h>

int velocidade_motor = 300; 
int aceleracao_motor = 300;

int pino_enable = 7;

// Definicao pinos STEP e DIR
AccelStepper motor2(1,10,9 );
AccelStepper motor1(1,12,13 );

int dose = -28;

//------------------------------



//IR SENSORS PINS----------------
int LNP = 26 ; //Left Nose-Poke
int MNP = 32 ; //Middle Nose-Poke
int RNP = 38 ; //Right Nose-Poke

int MNP_State = 0;
//--------------------------------

//Buzzer--------
int frequencia1 = 1000;
int frequencia2 = 8000;
int buzzer = 11;
//--------------------------------


 
void setup() {

//Buzzer pin-------
pinMode(buzzer,OUTPUT);
//-----------------

lcd.begin(20, 4); //LCD CONFIG
//SD.begin(53); // SD CARD CONFIG

loadSD();

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
pinMode(MNP,INPUT);
pinMode(RNP,INPUT);
digitalWrite(LNP, HIGH); //sets internal pull-up resistor due to open colector sensor configuration
digitalWrite(MNP, HIGH);
digitalWrite(RNP, HIGH);
//-----------------


//servos pins -----

Left_Servo.attach(5);  
Right_Servo.attach(6);
Left_Servo.write(80); // fechado
Right_Servo.write(85); // fechado
//Left_Servo.write(160); // aberto
//Right_Servo.write(145); // aberto

//-----------------

randomSeed(analogRead(A0)); //Generate random value with analog value from pin A0

}
 
 
void loop() {

//KEYPAD=============================================================================================================================

char key = keypad.getKey();

            

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
    
    /*Left_Servo.write(110); // hatch closes
    Right_Servo.write(130); // hatch closes
    Serial.println(key);*/ 
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
    
    /*Left_Servo.write(170); // hatch closes
    Right_Servo.write(180); // hatch closes
    Serial.println(key);*/ 
  }
//======================================================================



//configs for left pump ================================================

//backs all the way
  if (key == '7'){
      digitalWrite(pino_enable, LOW);
      motor1.runToNewPosition(motor1.currentPosition()+2000);
      digitalWrite(pino_enable, HIGH);
}

//backs a little
 if (key == '*'){
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

 if (key == 'C'){
      digitalWrite(pino_enable, LOW);
      motor2.runToNewPosition(motor2.currentPosition()-280);
      digitalWrite(pino_enable, HIGH);
}

//delivers one reward
if (key == 'D'){
      digitalWrite(pino_enable, LOW);
      motor2.runToNewPosition(motor2.currentPosition()-28);
      digitalWrite(pino_enable, HIGH);
}

//===================================================================================================================================

  

//Reads Sensor Values-----------
int LNPvalue = digitalRead(LNP);
int MNPvalue = digitalRead(MNP);
//int RNPvalue = digitalRead(RNP);
//-----------------------------------------

//initializes trial -----------------------
if(trial_flag == 0){
    TIME_TRIAL = millis();
    ++trial_number;
    myFile = SD.open("test2A.txt", FILE_WRITE);

    lcd.setCursor(2,0);
    lcd.print("TRIAL ");
    lcd.print(trial_number);
    lcd.print(" STARTED");

    trial_flag = 1;
}
//-----------------------------------------

if((MNPvalue == 0)&&(MNP_State == 0)){
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
      

if((LNPvalue == 0) && (MNP_State == 1)){     //receives reward
    TIME_LNP = millis();
            
    lcd.setCursor(1,2);
    lcd.print("Left NP Activated");
                  
    digitalWrite(pino_enable, LOW);
    motor1.runToNewPosition(motor1.currentPosition()-28);
    digitalWrite(pino_enable, HIGH);
      
    TIME_STEPPER_L = millis();

    lcd.setCursor(1,3);
    lcd.print("Reward Delivered");
     
    delay(3000);
    lcd.clear();
      
            
    Left_Servo.write(80);//closes left hatch
    TIME_SERVO_L_CLOSED = millis();
      
    MNP_State = 0;
    trial_flag = 0;

    //waits 5 seconds before trial begins
    //delay(5000);
      
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
    delay(1000); 
}


}


void loadSD()
{
  if (!SD.begin(53)) {
      lcd.setCursor(6,0);
      lcd.print("SD failed!");
      lcd.setCursor(6,1);
      lcd.print("SD failed!");
      lcd.setCursor(6,2);
      lcd.print("SD failed!");
      lcd.setCursor(6,4);
      lcd.print("SD failed!");
      delay(10000);
      return;
  }

  lcd.setCursor(0,0);
  lcd.print("Initializing SD card...");
  delay(1500);
  lcd.setCursor(0,2);
  lcd.print("initialization done.");
  delay(2500);
  lcd.clear();
}
