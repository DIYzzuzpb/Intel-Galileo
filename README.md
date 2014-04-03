Intel-Galileo
=============

##Blink/ 点亮LED灯
###I/O基本操作
###数字电平输出
    int led = 13;            // Arduino板子的LED一般是pin13
    pinMode(led, OUTPUT);    // 配置IO引脚的方向 (INPUT, OUTPUT, INPUT_PULLUP)
    digitalWrite(led, HIGH); // 控制引脚, 输出高/低电平

    delay(1000);               // wait for a second, 毫秒级延时
###数字电平读取
    int pushButton = 2;
    pinMode(pushButton, INPUT);
    int buttonState = digitalRead(pushButton);
###模拟电平读取
    int sensorValue = analogRead(A0); //这里的A0是板子上ANALOG IN中的A0~A5.
###模拟电平输出
	analogWrite(pin, value);  
	 //value: the duty cycle: between 0 (always off) and 255 (always on).

##DigitalReadSerial/ 串口输出
###串口配置
    Serial.begin(9600);             // 配置波特率
    Serial.println(buttonState);    // 串口输出


##Millis/ 运行时间读取
    unsigned long time = millis(); 
    //Returns Number of milliseconds since the program started (unsigned long)

##Debounce/ 按键去抖
检测到按键按下之后, 记录millis()值.  
等待一段之间(定义的去抖阈值),再判断按键,若仍为按下状态,才执行对应操作.

##tonePitchFollower/ 模拟量转方波驱动扬声器
	int sensorReading = analogRead(A0);
	int thisPitch = map(sensorReading, 400, 1000, 120, 1500);
	tone(9, thisPitch, 10);
	//tone()在一个引脚产生占空比50%的方波
	//第三个参数为持续时间(毫秒), 可选.未声明则直到noTune()才停止.
	//统一时间只能有一个tone()工作.
	
##ASCIITable/ 输出语句
	Serial.println("hello");
	Serial.write(thisByte);
	Serial.print(thisByte);
	Serial.print(thisByte, HEX);
	Serial.print(thisByte, OCT);
	Serial.print(thisByte, BIN);
##Serial_Read/ 串口交互
	brightness = Serial.read(); //注意,串口那里需要设置成不自动添加回车, 否则发完数据会自动发一个回车.
	
##Arrays/ 数组操作
所有位置都可以改为数组元素操作.   

	pinMode(ledPins[thisPin], OUTPUT);
	digitalWrite(ledPins[thisPin], HIGH);
	
##ADXL3xx/ 加速计
	
	const int xpin=A3;   // x-axis
	const int ypin = A2; // y-axis
	const int zpin = A1;
	
	Serial.print(analogRead(xpin));
	Serial.print(analogRead(ypin));
	Serial.print(analogRead(zpin));
	