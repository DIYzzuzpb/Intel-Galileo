void setup()
{
  Serial.begin(9600);
}
void loop()
{
  Serial.print("Time: ");
  unsigned long time = millis(); 
  //Returns Number of milliseconds since the program started (unsigned long)
  //prints time since program started
  Serial.println(time);
  // wait a second so as not to send massive amounts of data
  delay(1000);
}
