BUG of Galileo
====

##Reboot-BUG

拿到手的Intel Galileo有个BUG:    
不能正常reboot, 无论是按复位键,还是通过命令行, Galileo都是正常关闭, 但是在reboot时会卡住, 卡在下面这条信息:
	
	[ 8779.024881] reboot: machine restart
	
只有通过断电/上电才能正常reboot.

####解决办法
this is a known issue in the official 0.7.5 builds.   
It was fixed in BSP 0.8 and above. 

[AlexT_Intelhas](http://alextgalileo.altervista.org) newer unofficial builds on his site.

On firmware version 1.0.0 that problem goes away.


[Github: meta-alext-galileo](https://github.com/alext-mkrs/meta-alext-galileo)


## theTimerOne example bug 

https://communities.intel.com/message/219001#219001

Intel提供的例程Timerone->ISRBlink有问题.

	there's actually a second argument to attachInterrupt(), which is missing in the sketch.
	It has a default value of -1, which causes the callback not to be attached due to inconsistency with a certain check within that attachInterrupt().
	
eg.

	Timer1.initialize(100000); // the timer period is 100000 useconds, that is 0.1 sec  
	Timer1.attachInterrupt(timerIsr, 500000); // the callback will be called on each 5th timer interrupt, i.e. every 0.5 sec  
 
	
##Using rx tx instead of headphone jack?

https://communities.intel.com/message/230742#230742

Headphone jack是UART1, 对应/dev/ttyS1, 而板子上的RX,TX是UART0, 对应/dev/ttyS0.   
所以如果想要代替, 需要在grub配置文件中修改linux console.