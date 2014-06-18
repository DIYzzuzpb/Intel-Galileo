/************************************************************************
 * 
 * read data from SCA60C module to PID controller for control three motor.
 * AIM: Makes three motors to keep the platform level.
 * 
 *
 * SCA60C( N1000060 accelerometer) 
 * 
 *        (Vo2 output pin)
 * 
 *               UP
 *               ^
 *               |
 *               |  
 * LEFT <--------|--------- RIGHT
 *               |
 *               |
 *             DOWN
 * 
 *        (Vo1 output pin)
 * 
 * 
 * (0.5~4.5V) (-90~90 degrees)
 * Vo1: left-right   (x axis), LEFT is X-axis positive direction.
 * Vo2: up-down      (y axis), UP   is Y-axis positive direction.
 *
 *          M1          ^ Y
 *         / \          | 
 *        M2  M3        L___> X
 *
 *    Y-axis up, SCA60C_angle_Y > 0, PID output_y < 0, M1 down.
 *    X-axis up, SCA60C_angle_X > 0, PID output_x < 0, M3 down & M2 up.
 *-----------------------------------------------------------------------
 * Connect:
 * 
 * SCA60C:
 * Vo1 - Galileo Analog Input A0
 * Vo2 - Galileo Analog Input A1
 * VCC - Galileo 5V
 * GND - Galileo GND 
 *
 * (0.5~4.5V) (-90~90 degrees)
 * Vo1: left-right   (x axis)
 * Vo2: up-down      (y axis)
 * 
 * Motor: (pwm 0~255) (dir: 0 up, 1 down)
 * pwm1:  IO 3
 * pwm2:  IO 6
 * pwm3:  IO 9
 * 
 * dir1:  IO 2
 * dir2:  IO 5
 * dir3:  IO 8
 * 
 * read three double data from AHRS(always keep output data ).
 * 
 * 
 ***********************************************************************/

#define SERIAL_DEBUG   //open the Serial for debugging...turn off by commenting this row.

#include "Wire.h"
#include <PID_v1.h>
#include <SCA60C.h>

int time_start = 0, time_end = 0;
#define TIME_S (time_start = millis())
#define TIME_E (time_end = millis(),Serial.print("Time:"),Serial.println(time_end - time_start))

//SCA60C module in <SCA60C.h>
//Vo1 -> A0
//Vo2 -> A1
SCA60C sca(A0, A1, 2.25, 2.22); // set the output offset voltage when horizontal. 
double angle_x = 0;
double angle_y = 0;

//Motor pwm
// PWM pin : 3,5,6,9,10,11
int pwm1 = 3;
int pwm2 = 6;
int pwm3 = 9;
// motor dir
int dir1 = 2;
int dir2 = 5;
int dir3 = 8;

#define M1_UP    (digitalWrite(dir1, LOW))
#define M1_DOWN  (digitalWrite(dir1, HIGH))
#define M2_UP    (digitalWrite(dir2, LOW))
#define M2_DOWN  (digitalWrite(dir2, HIGH))
#define M3_UP    (digitalWrite(dir3, LOW))
#define M3_DOWN  (digitalWrite(dir3, HIGH))

#define M1_SPEED(X)  (setPwmI2C(pwm1, (X)))
#define M2_SPEED(X)  (setPwmI2C(pwm2, (X)))
#define M3_SPEED(X)  (setPwmI2C(pwm3, (X)))

int counter = 0;


//PID controller
//(Input range = +-20, Output range = +- 250, Kp = 12.50)
//(if Kp = 25, range will equals +- 10).
double KP_x = 25, KI_x = 5, KD_x = 2;
double KP_y = 25, KI_y = 5, KD_y = 2;
//Define Variables we'll be connecting to
double Setpoint_x = 0, Input_x = 0, Output_x = 0;
double Setpoint_y = 0, Input_y = 0, Output_y = 0;
//PID output Limit, ±100;    Stable range = ±3.
double OutMinLimit_x = -100 * KP_x, OutMaxLimit_x = 100 * KP_x, OutputRange_x = 3 * KP_x;
double OutMinLimit_y = -100 * KP_y, OutMaxLimit_y = 100 * KP_y, OutputRange_y = 3 * KP_y;
//Specify the links and initial tuning parameters
PID myPID_x(&Input_x, &Output_x, &Setpoint_x, KP_x, KI_x ,KD_x , DIRECT);
PID myPID_y(&Input_y, &Output_y, &Setpoint_y, KP_y, KI_y ,KD_y , DIRECT);
boolean output_changed = false; // the return value of PID.Compute().

//finite-state machine
#define STATE_STOP  0  //dir_state = 0, stop;
#define STATE_UP    1  //dir_state = 1, up;
#define STATE_DOWN  2  //dir_state = 2, down;
int dir_state_x = STATE_STOP;
int dir_state_y = STATE_STOP;

//variable for decoupling();
int move1 = 0, move2 = 0, move3 = 0;
int old_move1 = 0, old_move2 = 0, old_move3 = 0; // record the last value of move2 & move3.

//-------------------------------------------------------------------------------//
void setup() {
#ifdef SERIAL_DEBUG
  //turn on Serial for debugging...
  Serial.begin(9600);   
  Serial.println("Motor ready...");
#endif

  //begin Wire for controlling PWM by I2C.
  Wire.begin();
  setPwmI2C (pwm1, 0);  //first need 50ms~ (because of enable pwm output), and then need ~11ms
  setPwmI2C (pwm2, 0);
  setPwmI2C (pwm3, 0);

  pinMode(dir1, OUTPUT); //cost 25ms
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  //----> need 220ms.

  //set an initial position for three motors.
  downall(3000);
  upall(2000);


  //PID set
  Setpoint_x = 0; // set aim of X axis angle.
  dir_state_x = 0;
  Setpoint_y = 0; // set aim of Y axis angle.
  dir_state_y = 0;
  //turn the PID on
  myPID_x.SetOutputLimits(OutMinLimit_x, OutMaxLimit_x); //set the PID output Limits; 
  myPID_x.SetMode(AUTOMATIC);
  myPID_y.SetOutputLimits(OutMinLimit_y, OutMaxLimit_y); //set the PID output Limits; 
  myPID_y.SetMode(AUTOMATIC);
  //PID set over.
}

//-------------------------------------------------------------------------------//
void loop() {

  //1. read data of X,Y axis angle.
  Input_x = sca.GetAngleX();    //need 6ms, most is analogRead();
  Input_y = sca.GetAngleY();

  //2. PID, just need 1ms to calculate pid output. return new value of (Output_x & Output_y).
  myPID_x.Compute();
  output_changed = myPID_y.Compute();


#ifdef SERIAL_DEBUG      //need 10ms;
  //debug...
  counter++;
  if(output_changed == true)
  {
    Serial.print(counter);
    Serial.print(", In:");
    Serial.print(Input_x);
    Serial.print("/");
    Serial.print(Input_y);
    Serial.print(", Out:");
    Serial.print(Output_x);
    Serial.print("/");
    Serial.print(Output_y);
  }
#endif


  //3. decoupling for three motor.
  if(output_changed)
    decoupling();        //need 25~35ms to calculate (use setPwmI2C()), (use setPwm() is need 30~40ms)
    
    
    //loop() is over.
}

//-------------------------------------------------------------------------------//

//decoupling X/Y axis angle to three motor's speed.
//Direction of the motor 2 and motor 3 with an opposite direction of the motor.
//if PID output > 0, should up; if output < 0, down!
void decoupling()
{
  //motor 1 & motor 2,3 <---> Y axis.
  if(Output_y > OutputRange_y) // Output_y > 3 * KP_y, Y axis need to up!
  {
    if(dir_state_y != STATE_UP) // if not UP state, change state to UP now.
    {
      dir_state_y = STATE_UP;
      M1_UP;
      M1_SPEED(Output_y);
    }
#ifdef SERIAL_DEBUG
    Serial.print(",M1^");
#endif
    //M2,3 need to DOWN.
    move2 = -Output_y;
    move3 = -Output_y;
  }
  else if(Output_y < -OutputRange_y) // Output_y < -3 * Kp_y, Y axis need down!
  { 
    if(dir_state_y != STATE_DOWN) // if not DOWN state, change state to DOWN now.
    {
      dir_state_y = STATE_DOWN;
      M1_DOWN;
      M1_SPEED(abs(Output_y));
    }
#ifdef SERIAL_DEBUG
    Serial.print(",M1V");
#endif
    //M2,3 need to down.
    move2 = -Output_y;
    move3 = -Output_y;
  }
  else //else , Input - range < Output < Input + range. Y axis in Range(-3, +3), mean STABLE!
  {
    dir_state_y = STATE_STOP;
    M1_SPEED(10);      //a very little pwm value means a very slow speed of Motor.
    move2 = move3 = 0;
#ifdef SERIAL_DEBUG
    Serial.print(",M1-");
#endif
  }

  //X axis, Motor2 & 3;
   if(Output_x > OutputRange_x) // X axis need to up, M3.
   {
     move3 += Output_x;
     move2 -= Output_x;
     #ifdef SERIAL_DEBUG
     Serial.print(",X^"); //X axis is stable.
     #endif
   }
   else if(Output_x < -OutputRange_x) //X axis need to down, M3.
   { 
     move3 += Output_x;
     move2 -= Output_x;
     #ifdef SERIAL_DEBUG
     Serial.print(",XV"); //X axis is stable.
     #endif
   }
   else //else , Input - range < Output < Input + range. X axis in Range(-3, +3).
   {
     #ifdef SERIAL_DEBUG
     Serial.print(",X-"); //X axis is stable.
     #endif
   }
   
   #ifdef SERIAL_DEBUG
   //set Motor 2 & 3 by judging the value of move2&3;
   Serial.print(",move:");
   Serial.print(move2);
   Serial.print(",");
   Serial.print(move3);
   #endif

  if(move2 != old_move2)
  {
    old_move2 = move2;  //record the new value.
    //M2 direction
    if(move2 > 0)
      M2_UP;
    else
      M2_DOWN;
 
    M2_SPEED(abs(move2));
  }

  if(move3 != old_move3)
  {
    old_move3 = move3;  //record the new value.
    if(move3 > 0)
      M3_UP;
    else
      M3_DOWN;
      
    M3_SPEED(abs(move3));
  }
  
#ifdef SERIAL_DEBUG
  Serial.println(" ");
#endif
}

void downall(int time)
{
  //set a high speed.
  Set_Motor_Speed(250);
  //set dir(down). 
  M1_DOWN;
  M2_DOWN;
  M3_DOWN;
  delay(time); //delay some times.
  Set_Motor_Speed(0);
}

void upall(int time)
{
  //set a high speed.
  Set_Motor_Speed(250);
  //set dir(up). 
  M1_UP;
  M2_UP;
  M3_UP;
  delay(time); //delay some times.
  Set_Motor_Speed(0);
}

void Set_Motor_Speed(int pwm_speed)
{
  //set three motor's PWM value for control speed.(0 is stop and 255 is best speed of motor.)
  M1_SPEED(pwm_speed);
  M2_SPEED(pwm_speed);
  M3_SPEED(pwm_speed);
}


//set PWM use I2C 
//set pin(_iPinNum) output PWM with duty cycle of (_iPwmVal / 255).
void setPwmI2C(int _iPinNum, int _iPwmVal)
{
  if(_iPwmVal > 250) _iPwmVal = 250;
  // Select pin to write I2C commands to
  analogWrite(_iPinNum,1);            //analogWrite(pin, 1) will enable the PWM on the pin. 
  //but AnalogWrite(pin,0) does not disable the pwm.

  //Then there’s the I2C commands...

  // Select programmable PWM CLK source
  Wire.beginTransmission(0x20);
  Wire.write(0x29);  // CY8C9540A -  Config (29h)
  //xxxxx000b - 32kHz (default)
  //xxxxx001b - 24Mhz
  //xxxxx010b - 1.5Mhz
  //xxxxx011b - 93.75 kHz
  //xxxxx100b - 367.6 Hz (programmable)
  //xxxxx101b - Previous PWM
  //Wire.write(0x04);  // xxxxx100b - 367.6 Hz (programmable)
  Wire.write(0x02);    // 1.5MHZ , but output just 6khz. just use it~
  Wire.endTransmission();

  // Set divider the PWM CLK .
  Wire.beginTransmission(0x20);
  Wire.write(0x2C);
  Wire.write(0x01);
  Wire.endTransmission();

  // Set period register (2Ah) - 0x01 / 0xff, need 10ms
  Wire.beginTransmission(0x20);                
  Wire.write(0x2a);
  //xxxx0xxxb - Falling pulse edge (default)      
  //xxxx1xxxb - Rising pulse edge
  Wire.write(0xff);
  Wire.endTransmission();

  // Set minimum duty cycle - Pulse Width Register (2Bh)
  Wire.beginTransmission(0x20);
  Wire.write(0x2b);
  //This register sets the pulse width of the PWM output. 
  //Allowed values are between zero and the (Period - 1) value.
  ////// this is the pulse width...0-255 where 255 is all "on" for one cycle (1/clock frequency)
  Wire.write(_iPwmVal);
  Wire.endTransmission();
}

//just set pwm period & pulse of the PWM output. //need 10ms
void setPwm(int _iPinNum, int _iPwmVal)
{
  if(_iPwmVal > 250) _iPwmVal = 250;
  // Select pin to write I2C commands to
  analogWrite(_iPinNum,1);            //analogWrite(pin, 1) will enable the PWM on the pin. 

  Wire.beginTransmission(0x20);
  Wire.write(0x2a);
  Wire.write(0xff);
  Wire.endTransmission();

  Wire.beginTransmission(0x20);
  Wire.write(0x2b);
  Wire.write(_iPwmVal);
  Wire.endTransmission();
}
