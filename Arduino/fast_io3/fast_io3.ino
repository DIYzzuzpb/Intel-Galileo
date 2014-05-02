void setup(){
    pinMode(3, OUTPUT_FAST); //IO3 fast output mode.
}

void loop()
{
    register int x = 0;
    while(1){
        //digitalWrite(3, x); // 477kHz , the same with IO2 fast output mode.
 
        fastGpioDigitalWrite(GPIO_FAST_IO3, x); //an Intel extension of the Arduino API will achieve 680 kHz
        x =!x;
    }
}
