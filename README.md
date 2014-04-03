Intel-Galileo
=============

##Blink/ 点亮LED灯
###I/O基本操作

    int led = 13;            // Arduino板子的LED一般是pin13
    pinMode(led, OUTPUT);    // 配置IO引脚的方向
    digitalWrite(led, HIGH); // 控制引脚, 输出高/低电平

    delay(1000);               // wait for a second, 毫秒级延时

##DigitalReadSerial/ 串口输出,数字电平读取
###串口配置
    Serial.begin(9600);             // 配置波特率
    Serial.println(buttonState);    // 串口输出
###数字电平读取
    int pushButton = 2;
    pinMode(pushButton, INPUT);
    int buttonState = digitalRead(pushButton);
###模拟电平读取
    int sensorValue = analogRead(A0); //这里的A0是板子上ANALOG IN中的A0~A5.
