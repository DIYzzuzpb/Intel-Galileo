void setup(){
    pinMode(2, OUTPUT_FAST); // 477KHZ, IO2 fast output mode.
}

void loop()
{
    register int x = 0;
    while(1){
        digitalWrite(2, x);
        x =!x;
    }
}
