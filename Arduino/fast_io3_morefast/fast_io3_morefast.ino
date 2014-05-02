uint32_t latchValue;

void setup(){
    pinMode(3, OUTPUT_FAST);
    latchValue = fastGpioDigitalLatch();
}
void loop()
{
    while(1){
        //fastGpioDigitalWriteDestructive - can achieve GPIO speeds of about 2.93 MHz
         fastGpioDigitalWriteDestructive(latchValue);
         latchValue ^= GPIO_FAST_IO3;
    }
}
