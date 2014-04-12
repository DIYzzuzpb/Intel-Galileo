void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
}

void loop() {
  byte read_data;

  // check if data has been sent from the computer:
  if (Serial.available()) {
    // read the most recent byte (which will be from 0 to 255):
    read_date = Serial.read();

    Serial.print("I got ");
    Serial.println(read_data);
  }
}
