//Infred distance sensor -- GP2Y0A02YK0F
/**
 Detection range: 20cm ~ 150cm
 The detection interval: 50ms 
 
 //Linear relationship
 10cm - 2.4V (0~10cm , 0V -> 2.4V , distance = Voltage / 2.4;)
 
 //Nonlinear relationship
 15cm - 2.75V
 20cm - 2.5V
 30cm - 2V
 40cm - 1.5V
 50cm - 1.25V
 60cm - 1V
 90cm - 0.75V
 130cm - 0.5V
 
 ----------------------------------------------------------------
 // real relationship
 
**/

//select the input pin for the Infred distance sensor 
int pin_M1 = A2;
int pin_M2 = A3;
int pin_M3 = A4;

//variable to store the value coming from the sensor.
int sensorValue1 = 0;
int sensorValue2 = 0;
int sensorValue3 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  sensorValue1 = analogRead(pin_M1); //6ms
  sensorValue2 = analogRead(pin_M2); //6ms
  //AD, 0V -> 0, 5V -> 1024, voltage = sensorValue * 5 / 1024 = sensorValue / 204.8;
  
  Serial.print(sensorValue1 / 204.8);
  Serial.print(" ");
  Serial.println(sensorValue2 / 204.8);
  //Serial.println(get_distance(sensorValue1 / 204.8));
}

double get_distance(double voltage)
{
  // 0 ~ 10cm
  return (voltage / 2.4);
}


void detection()
{
  
}
