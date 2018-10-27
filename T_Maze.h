/*
  T_Maze.h - Library for controlling an automated T-Maze.
  Created by Igor S. Vaz, October 25, 2018.
  Released into the public domain.
*/
#ifndef T_Maze_h
#define T_Maze_h
#include <Keypad.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <AccelStepper.h>
#include "Arduino.h"

class T_Maze
{
  public:
    T_Maze();
    //~T_Maze();
    void readsensors();
    void trial_init_mode1();
    void trial_init_mode2();
    void trial_init_mode3();
    void trial_init_mode4();
    void left_side_mode1();
    void right_side_mode1();
    void MNP_mode2();
    void MNP_mode3();
    void MNP_mode4();
    void LNP_activated_reward();
    void RNP_activated_reward();
    void keypad_control();
    void SD_setup();
    int randomNumber;
    Servo Left_Servo;
    Servo Right_Servo;
    void keypadEvent(KeypadEvent key);
    int mode;
    int trial_flag;
    int MNPvalue;
    int MNP_State;
    int LNPvalue;
    int RNPvalue;
  private:

    int _pin;
};

#endif
