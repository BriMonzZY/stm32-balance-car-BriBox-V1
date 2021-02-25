# 平衡车V1

## 简介：

#### 作者：

BriMon

博客：https://www.cnblogs.com/Brimon-zZY/

邮箱：1610937885@qq.com



#### 所需模块：

此平衡车项目采用模块拼装。

降压模块（项目使用LM2596S）

电机驱动（项目使用TB6612FNG）

蓝牙模块（项目使用JDY-31）

OLED屏

stm32最小系统

12V电池（项目使用18650）

洞洞板 

带编码器的减速电机

MPU6050

超声波测距模块





## 引脚功能：

==超声波模块：==

**PA3**：trig		**PA2**：echo

==OLED模块：==

**PA4**：SCL		**PA5**：SDA

==蓝牙串口uart3：==

**PB10**：TXD		**PB11**：RXD

==电机驱动：==

**PA8**：PWMA		**PA11**：PWMB

**PB12**：AIN2		**PB13**：AIN1

**PB14**：BIN1		**PB15**：BIN2

（方向以超声波所对方向为正方向）

**A01**：左轮M1		**A02**：左轮M2

**B01**：右轮M2		**B02**：右轮M1



==编码器：==

**PB6**：左轮A		**PB7**：左轮B

**PB6**：左轮A		**PB7**：左轮B

**PA0**：右轮A		**PA1**：右轮B

==MPU6050：==

**PB4**：SCL		**PB3**：SDA		**PB5**：INT

==OLLED_SPI:==

**PA6**:CS		**PA7**:DC

**PB0**:RES		**PB1**:D1

**PA12**:D0



## 问题日志：

### 2021年2月25日00:25:51

> HCSR04文件夹中，hc2.c以及hcsr04.c都有问题，其中，hcsr04.c中的程序会使mpu采集数据不正常，初步猜测可能与TIM3有关，程序换为hcsr04_2.c后可以正常运行。



```c
Moto1 = Balance_Pwm-Velocity_Pwm-Turn_Pwm;                 //=计算左轮电机最终PWM
Moto2 = Balance_Pwm-Velocity_Pwm+Turn_Pwm;                 //=计算右轮电机最终PWM

/*上面的一段代码有问题，会使速度环失去作用，而下面的代码，加上乘积后，速度环可以发挥作用，目前不知道为什么*/

Moto1 = Balance_Pwm-balance_UP_KP*Velocity_Pwm-Turn_Pwm;    //=计算左轮电机最终PWM
Moto2 = Balance_Pwm-balance_UP_KP*Velocity_Pwm+Turn_Pwm;    //=计算右轮电机最终PWM
```

目前还不理解PID，慢慢来...



> 我想在蓝牙串口加上一个调节PID的功能，比如输入KI值对应的16进制，KI就会调整成对应的值，目前在usart3.c 中有16进制转换为10进制的代码，但是没有串口接收字符的函数（我也不知道怎么写）。



> 目前的程序（ID：e9f802e8dadec9db5282d53738722c286552b73b）在显示方面有问题。显示的左编码器的值是右面电机的码值，懒得改了，希望以后可以改过来。

### 2021年2月25日13:53:53

> 如果把mpu6050倒放的话，检测到的值会从0到-90再到0，如果这时侯stby接高电平，也就是说电机会转，那么速度积分就会疯狂上升，那么小车就不受控制了 



在参数为：

> float balance_UP_KP = 330; 	 // 小车直立环PD参数  500时出现大幅度抖动   300
> float balance_UP_KD = 1;    // 极性为正        1.7出现高频振动    1  

> #define VKP -80
> float Velocity_Kp = VKP;     // 小车速度环 PI参数   极性为负
> float Velocity_Ki = VKP * 0.005;// ki = (1/200)kp

时，小车已经基本平稳，但还是在中心位置前后1cm晃动，初步猜测应该是重心没有调整好



> 移植OLED——SPI的时候出现bmp.h和oledfont.h的重复定义，创建c文件，extern其中的数组即可。



目前版本(95d0cc893dd259dbdf22022d9451b68f0341ae5f)的缺陷：

> * 行走的时候会有卡顿
> * 站立的时候会在平衡点附近摆动
> * 有的时候转向不能及时反馈（比如左转向后，按好几次右转向才会有反应向右转）
> * 在转向的时候前进会突然加速然后摔倒
> * 避障不及时
> * 行走时不能及时停住，比较软
> * OLED显示的左码值是右轮的（懒得改了）