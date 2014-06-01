unsigned long time_start = 0, time_end = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT); //22ms
}

void loop()
{
  test();
  //test_pinmode(); // 22ms
  //test_pin(); // 2~3 ms, -> high -> low need 5ms
  //test_serial(); // 100 char, 30~50ms
  //test_cal();       // '+' operation, many time will spend 1ms, so it's fast.
  //test_cal2();        // '*' operation, so fast too.
}

void test()
{
  Serial.print("Time: ");
  time_start = millis(); 
  //Returns Number of milliseconds since the program started (unsigned long)
  //prints time since program started
  Serial.println(time_start);
  // wait a second so as not to send massive amounts of data
  delay(1000);
}

void test_pinmode()
{
  time_start = millis();
  pinMode(2, OUTPUT);
      time_end = millis();
  Serial.print("Time:");
  Serial.println(time_end - time_start);
}

void test_pin()
{
  time_start = millis();
  digitalWrite(2, HIGH);      //2~3 ms
  digitalWrite(2, LOW);
  time_end = millis();
  Serial.print("Time:");
  Serial.println(time_end - time_start);
}

char ch = 'a';
int ch_i = 0;
void test_serial()
{
  time_start = millis();
  for(ch_i = 0; ch_i < 10000; ch_i++)
    Serial.print(ch);
  time_end = millis();
  Serial.print("Time:");
  Serial.println(time_end - time_start);
 // delay(100);
}

int i = 0;
int int_value = 0;
long long_value = 0;

void test_cal()
{
  int_value = 0;
  time_start = millis();
  for(i = 0; i < 100; i++)
    int_value += i;
  time_end = millis();
  Serial.print(int_value);
  Serial.print("Time:");
  Serial.println(time_end - time_start);
}

double dou_value = 1;
void test_cal2()
{
  dou_value = 1;
  time_start = millis();
  for(i = 0; i < 1000; i++)
    dou_value *= 1.01;
  time_end = millis();
  Serial.print(dou_value);
  Serial.print("Time:");
  Serial.println(time_end - time_start);
}
