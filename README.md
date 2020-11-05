# NFC-Gate-MCS51
基于51单片机的NFC门控
（[详情](https://blog.csdn.net/GavinWang23/article/details/104848886)）

keil5 工程

RC522 ➕ 51单片机 ➕ 舵机

将TEST522.hex烧录到51单片机中

串口会输出刷卡的id，再将这个id写到程序中即可

## 接口说明
引脚 | 作用 
--|--
 P1^0 | 舵机脉冲信号输出 
 P2^7 | 接RC522模块SDA
 P2^6 | 接RC522模块SCK
 P2^5 | 接RC522模块MOSI
 P2^4 | 接RC522模块MISO
 P2^3 | 接RC522模块RST
 P1^7 | 电容触摸开关输入
 P1^6 | 预留开关信号1，我接入的是esp8266控制的继电器（天猫精灵）（[详情](https://blog.csdn.net/GavinWang23/article/details/101637481)）
 P1^5 | 预留开关信号2
