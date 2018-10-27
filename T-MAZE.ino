#include <T_Maze.h>
T_Maze TMaze;

//SD-CARD==================================================================================================================
#include <SPI.h>
#include <SD.h>

File myFile;
 
void setup() {

//SD.begin(53); // SD CARD CONFIG

 /* if (!SD.begin(53)) {
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
    }*/
TMaze.SD_setup();

randomSeed(analogRead(A0)); //Generate random value with analog value from pin A0

}
 
void loop() {

//KEYPAD===============
TMaze.keypad_control();

//reads Sensors States
TMaze.readsensors();

//INITIALIZES TRIAL - MODE 1 =================================
if ((TMaze.randomNumber == 0) && (TMaze.mode == 1)){
   TMaze.trial_init_mode1();
}

//Left side activated
if ((TMaze.randomNumber == 1) && (TMaze.mode == 1)){
    TMaze.left_side_mode1();
}

//Right side activated
if ((TMaze.randomNumber == 2) && (TMaze.mode == 1)){
    TMaze.right_side_mode1();
}


//INITIALIZES TRIAL - MODE 2 =================================
//if (TMaze.mode == 2){
//   TMaze.readsensors();
//}

if ((TMaze.mode == 2) && (TMaze.trial_flag == 0)){
   TMaze.trial_init_mode2();
}

if ((TMaze.mode == 2) && (TMaze.trial_flag == 0)){
   TMaze.trial_init_mode2();
}

if((TMaze.MNPvalue == 0)&&(TMaze.MNP_State == 0) && (TMaze.mode == 2)){
  TMaze.MNP_mode2();
}

if((TMaze.LNPvalue == 0) && (TMaze.MNP_State == 1) && (TMaze.mode == 2)){
  TMaze.LNP_activated_reward();
}

//INITIALIZES TRIAL - MODE 3 ===============================================
//if (TMaze.mode == 3){
//   TMaze.readsensors();
//}

if ((TMaze.mode == 3) && (TMaze.trial_flag == 0)){
   TMaze.trial_init_mode3();
}

if((TMaze.MNPvalue == 0)&&(TMaze.MNP_State == 0) && (TMaze.mode == 3)){
  TMaze.MNP_mode3();
}

if((TMaze.RNPvalue == 0) && (TMaze.MNP_State == 1) && (TMaze.mode == 3)){
  TMaze.RNP_activated_reward();
}

//INITIALIZES TRIAL - MODE 4 ===================================================================================
if ((TMaze.mode == 4) && (TMaze.trial_flag == 0)){
   TMaze.trial_init_mode4();
}

if((TMaze.MNPvalue == 0)&&(TMaze.MNP_State == 0) && (TMaze.mode == 4)){
  TMaze.MNP_mode4();
}


if((TMaze.LNPvalue == 0) && (TMaze.MNP_State == 1) && (TMaze.mode == 4) && (TMaze.randomNumber == 1)){
  TMaze.LNP_activated_reward();
}

if((TMaze.RNPvalue == 0) && (TMaze.MNP_State == 1) && (TMaze.mode == 4) && (TMaze.randomNumber == 2)){
  TMaze.RNP_activated_reward();
}


}
