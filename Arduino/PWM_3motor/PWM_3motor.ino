#include "Wire.h"

// PWM pin : 3,5,6,9,10,11

int pwm1 = 3;
int pwm2 = 5;
int pwm3 = 6;

//  dir = high, motor down! 
//  dir = low,  motor up!
int dir1 = 2;
int dir2 = 4;
int dir3 = 7;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  setPwmI2C (pwm1, 0);
  setPwmI2C (pwm2, 0);
  setPwmI2C (pwm3, 0);
 
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);
  pinMode(dir3, OUTPUT);
  
  downall();
  
  /*
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  setPwmI2C (pwm1, 250);
  setPwmI2C (pwm2, 250);
  setPwmI2C (pwm3, 250);
  delay(200);
  
  //test1();
  Set_Motor_Stop();
  */
}

void loop() {
  //test2();
}

//set PWM use I2C
//set pin(_iPinNum) output PWM with duty cycle of (_iPwmVal / 255).
void setPwmI2C(int _iPinNum, int _iPwmVal)
{
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

  // Set period register (2Ah) - 0x01 / 0xff
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

void test1()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  
  delay(4000); //delay 3s
  
  digitalWrite(dir1, LOW);
  digitalWrite(dir2, LOW);
  digitalWrite(dir3, LOW);
  
  delay(1000); //delay 3s
}

void test2()
{
  int i = 0;
  double angle = 0;
  double data = 0;
  for(i = 0; i < 3140; i++,angle += 0.01)
  {
    data = sin(angle);
    set_pwm1(data);
    Serial.println(data);
    delay(20);
  }
}

void set_pwm1(double data)
{
  if(data > 0)
  {
    digitalWrite(dir1, LOW);
    digitalWrite(dir2, HIGH);
    digitalWrite(dir3, HIGH);
  }
  else
  {
    digitalWrite(dir1, HIGH);
    digitalWrite(dir2, LOW);
    digitalWrite(dir3, LOW);
  }
  
  data = abs(data);
  setPwmI2C (pwm1, data * 25);
  setPwmI2C (pwm2, data * 25);
  setPwmI2C (pwm3, data * 25);
}

void downall()
{
  digitalWrite(dir1, HIGH);
  digitalWrite(dir2, HIGH);
  digitalWrite(dir3, HIGH);
  setPwmI2C (pwm1, 250);
  setPwmI2C (pwm2, 250);
  setPwmI2C (pwm3, 250);
  delay(3000);
  Set_Motor_Stop();
}

void Set_Motor_Stop()
{
  //set pwm 0 to stop three motor.
  setPwmI2C (pwm1, 0); 
  setPwmI2C (pwm2, 0);
  setPwmI2C (pwm3, 0);
}

void Set_Motor_Speed(int pwm_speed)
{
  //set three motor's PWM value for control speed.(0 is stop and 255 is best speed of motor.)
  setPwmI2C (pwm1, pwm_speed);
  setPwmI2C (pwm2, pwm_speed);
  setPwmI2C (pwm3, pwm_speed);
}
