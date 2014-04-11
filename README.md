Intel-Galileo
=============
#####使用Galileo的正确姿势:    
一定要UART到电脑上( 串口连接方法见Documents/Galileo-UART.txt ), 得知Galileo的准确状态。   
板子复位以后启动很慢, 要等好久才可以用Arduino IDE下程序, 否则下载会失败, 提示超时。
#####电源保护
和电源相关的连线, 一定要用杜邦线帽保护好, 因为Galileo的电源引脚都在一起, VCC旁边就是GND, 裸露的杜邦线, 稍微碰下就导致短路, Galileo有严格的电源管理机制, 短路后会立刻重启.(这也是为什么要使用UART准确得到Galileo的状态的原因之一)

##连接

####UART TLL
Galileo provides UART TTL (5V/3.3V) serial communication, which is available on digital pin 0 (RX) and 1 (TX).   
 
#### UART RS-232
In addition, a second UART provides RS-232 support and is connected via a 3.5mm jack. 


##I/O基本操作

###Blink/ 点亮LED灯
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
Intel Galileo只有一个串口, 使用USB连接Galileo板子和电脑时, 板子上有转换电路。   
否则, 使用引脚RX(0),TX(1)外接TTL/USB连接电脑。
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
	
##Serial output 串口输出
###ASCIITable/ 输出语句
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
	
	
##以太网配置
####ChatServer/    
将Galileo配置成一个局域网中的聊天服务器.   
其他电脑可以telnet到此服务器, 输出的消息会广播给所有连接此服务器的客户端。    
使用SPI对应的引脚。
Ethernet shield attached to pins 10, 11, 12, 13
	

	#include <SPI.h>
	#include <Ethernet.h>
	
	//配置Galileo的Mac地址,IP,gateway, subnet.
	byte mac[] = { //Galileo 的以太网端口地址, 板子上有标签98:4F:EE:00:2E:98 
	0x98, 0x4F, 0xEE, 0x00, 0x2E, 0x98 };
	IPAddress ip(10,42,0, 177);
	IPAddress gateway(10,42,0, 1);
	IPAddress subnet(255, 255, 255, 00);
	
	// telnet defaults to port 23
	EthernetServer server(23);
	
	
	// initialize the ethernet device
	// 支持DHCP -> Ethernet.begin(max);
	//            Ethernet.begin(mac, ip); 
	Ethernet.begin(mac, ip, gateway, subnet);
	
	
	  // wait for a new client:
	  EthernetClient client = server.available();
	  
	  // 判断
	  if(client)
	  {
	  ...
	  }
	  
	  server.write(val);
	  server.write(buf, len);
	  
	  
	  //具体应用看 Arduino - Ethernet library - Server class / Client class
	  


###EEPROM
Intel Galileo的EEPROM是11KB, 所以地址是从0~(11 * 1024 - 1) , 即 0~11263. 

	#include <EEPROM.h>
	
	//the EEPROM can only hold a value from 0 to 255.
	EEPROM.write(address,value);
	
	value = EEPROM.read(address);

### UART0串口

 [ How do I access /dev/ttyS0?](https://communities.intel.com/message/219828#219828)



参考Programming_GPIO_From_Linux/的文档教程,学会linux下控制Galileo的gpio.


ttyS0对应UART0, 对应引脚IO0,IO1. 
  
查看Galileo IO mapping可知,
相关引脚在linux下为gpio40,gpio41,gpio4.    
gpio40是0-RX的选择器控制端口, 其值为0时选择ttyS0, 为1选择gpio50.     
gpio41是1-TX的选择器控制端偶, 0->ttyS0, 1->gpio51;
gpio4是Level Shifter OE, 要配置其输出为1使能.

导出引脚40,41,4
	
	echo -n 40 > /sys/class/gpio/export 
	echo -n 41 > /sys/class/gpio/export 
	echo -n 4 > /sys/class/gpio/export 
配置为输出:

	echo -n "out" > /sys/class/gpio/gpio40/direction  
	echo -n "out" > /sys/class/gpio/gpio41/direction
	echo -n "out" > /sys/class/gpio/gpio4/direction
	
配置输出值:

	echo -n "0" > /sys/class/gpio/gpio40/value
	echo -n "0" > /sys/class/gpio/gpio41/value 
	
	echo -n "1" > /sys/class/gpio/gpio40/value 

引脚配置完成, 现在ttyS0与UART0的TX0,RX0相连接.
硬件上, 使用TTL转USB模块, RX0(Galileo IO 0) -> TTL(TX),
TX0(Galileo IO 1) -> TTL(RX), Galileo GND -> TTL(GND);

USB -> 电脑.

硬件配置完成!

接下来, 在电脑终端中连接对应串口, 波特率默认为9600
Mac OS X下如下:

	$ screen /dev/tty.usbserial-A7027C8G 9600

然后, 在Galileo的控制台中, 

Galileo -> ttyS0

	root@clanton:~# echo "hello, ttyS0, this is Galileo" > /dev/ttyS0 

ttyS0 -> Galileo
	
	root@clanton:~# cat /dev/ttyS0 
这样就变为了输入.
然后在ttyS0端口,也就是电脑中的screen中,进行输出, 信息就会通过串口显示在Galileo中.
	