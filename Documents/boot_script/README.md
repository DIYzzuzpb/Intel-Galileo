Automatically start script at boot script
===

为了调试方便, 需要配置Galileo, 使其可以开机自动配置好Etherent, 这样就可以远程ssh登录到Galileo上面进行调试。

1. 使用Arduino的IDE, 编译出Galileo的可执行文件。     
	*set_ethernet.ino*

		/*
    	* Ethernet shield attached to pins 10, 11, 12, 13
		*/

		#include <Ethernet.h>

		// Enter a MAC address and IP address for your controller below.
		// The IP address will be dependent on your local network.
		// gateway and subnet are optional:
		byte mac[] = {0x98, 0x4F, 0xEE, 0x00, 0x2E, 0x98 }; //根据具体的开发板而定
		
		IPAddress ip(10,42,0, 21);
		IPAddress gateway(10,42,0, 1);
		IPAddress subnet(255, 255, 255, 00);
		
		//ssh defaults to port 22
		EthernetServer server(22);
		
		void setup() {
			Ethernet.begin(mac, ip, gateway, subnet);
		}

		void loop() { }
		
	编译生成set_ethernet.elf, 自动下载在Galileo的/sketch中, 拷贝到/home/root/set_ethernet/作为备份。   
	(注意, 以上都是在SD卡的Galileo中进行。没有SD卡的Galileo每次断电后所有新数据都会丢失，再次上电需要重新下载程序。)
		
2. 编写后台脚本
	
	set_ethernet.sh
	
		#!/bin/sh
		/home/root/set_ethernet/set_ethernet.elf /dev/pts/0 /dev/ttyS0 &
 

3. 添加至开机自动执行的脚本中.

		root@clanton:~# cp set_ethernet/set_ethernet.sh /etc/init.d/.
		root@clanton:~# update-rc.d set_ethernet.sh start 99 .
	可以在/etc/rc5.d/中看到新添加的自启动脚本链接。
		
		root@clanton:~# ls -l /etc/rc5.d/