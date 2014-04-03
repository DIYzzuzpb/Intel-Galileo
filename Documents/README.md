Galileo is a microcontroller board based on the Intel Quark SoC X1000 Application Processor, 
a 32-bit Intel Pentium-class system on a chip.

Digital pins 0 to 13 (and the adjacent AREF and GND pins),   
Analog inputs 0 to 5, the power header,   
ICSP header, and the UART port pins (0 and 1),   
are all in the same locations as on the Arduino Uno R3.    

工作所需电压(3.3V / 5.0V);   
The core operating voltage of Galileo is 3.3V.    
板上有一jumper(IOREF), 可通过跳线帽切换I/O的参考电压为3.3V / 5.0V;    


###Arduino Shield Supported Features
#####14 digital input/output pins.  
#####A0~A5 - 6 analog inputs,    
via an AD7298 analog-to-digital (A/D) converter, provides 12 bits of resolution 
(i.e., 4096 different values). By default they measure from ground to 5 volts.
#####I2C bus, TWI
#####SPI   
Defaults to 4MHz to support Arduino Uno shields. Programmable up to 25MHz.
#####UART
#####VIN
#####5V/3.3V OUTPUT PIN
Maximum current draw to the shield is 800 mA.
#####IOREF   
a selection jumper on the board is used to select between 3.3V and 5V shield operation. 





