# stm32地址总线FMC学习





## 一、整体架构介绍（针对SDRAM）

![image-20240429111843494](D:\##-Work_Combine\4.同步\2.FSMC学习24.04.29\pic\FMC简介.png)

FSMC 是只能支持静态存储器。但是FMC可以驱动SDRAM，有支持SDRAM的定时刷新功能。

![FSMC功能框图合](D:\##-Work_Combine\4.同步\2.FSMC学习24.04.29\pic\FSMC功能框图合.jpg)



### 1、引脚大致介绍

FMC_NBL[3:0]：地址线

FMC_A[25:0]：地址线

FMC_D[31:0]：信号线



FMC_SDCLK：

FMC_SDNWE：写信号线。N：表示低电平有效。W：表示write。E：表示enable。

FMC_SDCKE[1:0]：写信号线。N：表示低电平有效。W：表示write。E：表示enable。

FMC_SDNE[1:0]：N：表示低电平有效。E：表示enable。

FMC_NRAS：

FMC_NCAS：



信号介绍. 

![FMC的SDRAM对应的引脚](D:\##-Work_Combine\4.同步\2.FSMC学习24.04.29\pic\FMC的SDRAM对应的引脚.jpg)

FMC_SDNE[1:0] 其实就是CS片选引脚。之所以有两根，是因为地址映射。

![CK两个引脚对应情况](D:\##-Work_Combine\4.同步\2.FSMC学习24.04.29\pic\CK两个引脚对应情况.jpg)

### 2、存储控制器

SDRAM中有控制寄存器、时序寄存器、命令模式寄存器、刷新定时器寄存器

![SDRAM的四个寄存器](D:\##-Work_Combine\4.同步\2.FSMC学习24.04.29\pic\SDRAM的四个寄存器.jpg)

根据配置这几个结构体就可以将FMC配置完成。





### 3、

### 4、

