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

	
##Using rx tx instead of headphone jack?

https://communities.intel.com/message/230742#230742
