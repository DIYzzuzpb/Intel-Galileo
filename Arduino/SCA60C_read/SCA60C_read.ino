/*
  SCA60C(N1000060) 
 
              (Vo2)
               up
               ^
               |
               |  
 left <--------|---------right
               |
               |
             down
             (Vo1)
 
 
 (0.5~4.5V) (-90~90 degrees)
 Vo1: left-right   (x axis)
 Vo2: up-down      (y axis)
 
 */

//Vo1 -> A0
//Vo2 -> A1
int SCA60C_Pin_x = A0;
int SCA60C_Pin_y = A1;

int sensorValue_x = 0;
int sensorValue_y = 0;

//output voltage when horizontal.
double voltage_offset_x = 2.29;
double voltage_offset_y = 2.37;

double voltage_x = 0;
double voltage_y = 0;

double angle_x = 0;
double angle_y = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  test();
}

double SCA60C_getX()
{
  static double X_tmp = 0;
  
  sensorValue_x = analogRead(SCA60C_Pin_x);
  voltage_x = (sensorValue_x / 204.8);
  X_tmp = -asin(0.5 * (voltage_x - voltage_offset_x)) * 57.292779524;
  if(isnan(X_tmp))   // if X_tmp is nan, just ignore it.
    return angle_x;
  else
  {
    angle_x = X_tmp;
    return X_tmp;
  }
}

double SCA60C_getY()
{
  static double Y_tmp = 0;
  
  sensorValue_y = analogRead(SCA60C_Pin_y);
  voltage_y = (sensorValue_y / 204.8);
  Y_tmp = -asin(0.5 * (voltage_y - voltage_offset_y)) * 57.292779524;
  if(isnan(Y_tmp)) // if Y_tmp is nan, just ignore it.
    return angle_y;
  else
  {
    angle_y = Y_tmp;
    return Y_tmp;
  }
}

void test()
{
  //read data by analog input.
  sensorValue_x = analogRead(SCA60C_Pin_x);  // return int Value. 0~5V will return Value (0~1024).
  sensorValue_y = analogRead(SCA60C_Pin_y);

  //calculate voltage.
  voltage_x = (sensorValue_x / 1024.0) * 5.0;
  voltage_y = (sensorValue_y / 1024.0) * 5.0;

  /*calculate angle
   
   Theta = asin((V_out - V_offset) / 2);
   Theta = asin(0.5 * (V_out - Vdd / 2));
   
   Vdd = 5(V);
   V_offset = V(0g) = Vdd / 2 = 2.5(V);
   V_out = 0.5 ~ 2.5 ~ 4.5 (-1g ~ 0g ~ 1g);
   
   */
  angle_x = -asin(0.5 * (voltage_x - voltage_offset_x)) * 180.0 / 3.1415926;
  angle_y = -asin(0.5 * (voltage_y - voltage_offset_y)) * 180.0 / 3.1415926;

  //print data for debugging
  Serial.print("X axis:vol ");
  Serial.print(voltage_x);
  Serial.print(",angle ");
  Serial.print(angle_x);
  Serial.print("---");

  Serial.print("Y axis:vol ");
  Serial.print(voltage_y);
  Serial.print(",angle ");
  Serial.print(angle_y);

  Serial.println(" ");
}

void test2()
{
  SCA60C_getX();
  SCA60C_getY();
  Serial.print(angle_x);
  Serial.print(",");
  Serial.print(angle_y);
  Serial.println(" ");
}

