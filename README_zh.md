# Tuya IoTOS Embedded Demo Zigbee E-ink-display

[English](./README.md) | [中文](./README_zh.md)

<br>

## 简介

本 demo 基于 [涂鸦IoT平台](https://iot.tuya.com/) 、涂鸦智能APP、IoTOS ZIGBEE MCU SDK，使用涂鸦ZIGBEE系列模组快速搭建一个墨水屏应用程序。该墨水屏的硬件部分由1个MCU、1个通信模组、1个墨水屏幕和1个字库芯片组成，实现了座位信息设置与更新、预约信息显示、预约信息更新、预约信息同步、低电量报警、设备复位等功能。另外，该demo还实现了屏幕的刷新局刷与全刷等。开发者可以通过涂鸦智能APP实时下发屏幕所显示信息，如座位编号、二维码图像等的设置。

<br>

## 快速上手

### 开发环境搭建

- IDE 根据KEIL官方说明进行安装。

  [MDK下载](https://www.keil.com/download/product/)

  [安装芯片包](https://www.keil.com/dd2/pack/)

- Tuya ZIGBEE MCU SDK移植方式：请按照工程编译提示的错误进行修改完成工程编译。 

<br>

### 编译烧录

- 代码修改

  1. 在 `protocol.h` 中填入在涂鸦 IoT 工作台创建的智能产品 PID。

     ```
      #define PRODUCT_KEY     "xxxxxxxx"
     ```

     将其中 `xxxxxxxx` 替换为开发者所创建的PID。

- 编译运行Demo代码

  按照SDK编译提示的错误修改好文件后编译系统工程，并下载工程至硬件运行，调试MCU和模组的时候可以先使用涂鸦串口调试助手调试 二者是否工作正常。

<br>

### 文件介绍
```c
├── User
│   ├── main.c                                             /* 主程序入口文件 */
│   └── MY_ST_config.h                                     /* 硬件配置文件 */
├── system                                                 /* 系统文件目录 */
│   ├── delay.c                                            /* 延时函数 */
│   ├── delay.h
│   ├── EPAPER.c                                           /* 电子屏幕初始化 */
│   ├── EPAPER.h
│   ├── GT5SLAD3BFA_stm32l431_keil5.lib                    /* 字库芯片静态库文件 */
│   ├── GT5SLAD3B-GTA6401.h                                /* 字库芯片头文件 */
│   ├── IO.c                                               /* GPIO口初始化 */
│   ├── IO.h
│   ├── key.c                                              /* 按键初始化 */
│   ├── key.h
│   ├── picture.h                                          /* 图片像素数据存储 */
│   ├── RCC.c                                              /* 系统时钟配置 */
│   ├── RCC.h
│   ├── SPI.c                                              /* SPI初始化 */
│   ├── SPI.h
│   ├── sys.c                                              /* 系统任务文件 */                 
│   ├── sys.h
│   ├── qrcode_create.c                                    /* 二维码组件 */
│   ├── qrcode_create.h
│   ├── tuya_qrcode_create.c                               
│   ├── tuya_qrcode_create.h
│   ├── USART.c                                            /* 串口初始化 */
│   ├── USART.h
│   ├── utf8ToUnicode.c                                    /* UTF8转UNICODE */
│   └── utf8ToUnicode.h
├── CJSON
│   ├── cJSON.c                                            /* JSON配置文件 */
│   └── cJSON.h
├── mcu_sdk
│   ├── mcu_api.c                                          /* dp功能数据文件 */
│   ├── mcu_api.h
│   ├── protocol.c                                         /* 协议分析和接收模块发送消息时的响应 */
│   ├── protocol.h
│   ├── system.c                                           /* 单片机与zigbee通信的框架分析 */
│   ├── system.h
│   └── zigbee.h                                           /* SDK中使用的宏定义 */
```

### 应用入口
入口文件：`/user/main.c`

+ `int main(void)` 对系统工程进行一些必要的初始化，该函数只执行一次。
+ `while(1)` 用来执行用户应用代码，该函数循环执行。



### I/O 列表

| 外设功能     | **外设接口** | **占用引脚** |
| :--------: | :--: | :--: |
| MCU与模组通信串口 | USART2 | PA2 PA3 |
| 打印log串口（USB） | USART3 | PB11 PB10 |
| 墨水屏CS |     SPI2     |       PB1        |
| 字库芯片CS |     SPI2     | PA12 |
| 墨水屏 |     SPI2     | PB5  PB8  PB9 |
| 字库芯片 |     SPI2     | PB13  PB14  PB15 |
| 模组唤醒MCU |    普通IO    | PB0 |
| MCU唤醒模组 |    普通IO    | PC13 |

### FAQ

- Q：开发板烧录下载时检测不到单片机

  A：STM32有SW和JTAG两种下载方式，若硬件下载口连线为SW端口，软件工具Debug中下载方式也要对应选择SW模式；

- Q：墨水屏幕在局刷时会留有残影，这也是局刷时的一个缺点； 

  A：可通过连续刷多次白屏将残影慢慢消除，但是此后再局刷时图片会逐渐变浅，光照强度也会对局刷时变浅有影响；如果局刷频率较高的话图片颜色变浅会更加明显，频率低时图片变浅基本差距不大。

- Q：MCU作为主机，墨水屏与字库芯片作为从机来选择，当MCU从字库中获取字符数据时需判断是否为MISO模式，此时编译报错变量定义错误；

  A：MISO– Master Input Slave Output,主设备数据输入，从设备数据输出；当MCU为输入时该引脚需配置为读输入模式，将MISO宏定义改为

  ```c
  #define MISO		HAL_GPIO_ReadPin(ZK_SPI2_PORT, ZK_SPI2_MISO_PIN)  即可。
  ```

- Q：当MCU_SDK工程移植完成后，使用涂鸦模组调试助手来测试MCU是否能够通信正常，可以通过MCU模拟和模组模拟分别来测试，MCU模拟时模组回  复数据失败；

  A：MCU模拟时，要注意串口接线是否正确，串口连接在与PA2 PA3相连的外部MTX与MRX上，这样才能保证模组正确回复数据；

- Q：模组配网完成后会出现掉线的情况，需要重新配网;

​       A：设备显示离线，有几种可能：

​             a:网络断网 

​             b:MCU端发送了重置指令，进入了配网模式

​            c:配网后，MCU 90秒未回复心跳包

​            设备离线可将设备移除后重新配网，MCU发送配网指令重新配网即可；

- Q：模组下发dp数据时，MCU经过4~5s后才回复；

   A：zigbee网络策略参数中存在poll周期值，已经加入到网络中的低功耗模块的唤醒周期，模组的poll周期设为5s，故dp下发后会等待一个poll周期MCU才会回复数据；

- Q：调试模组低功耗部分时，模组查不到数据并且设备一直重启；

   A：zigbee网络策略参数中存在poll周期值，已经加入到网络中的低功耗模块的唤醒周期，模组的poll周期设为5s，故dp下发后会等待一个poll周期MCU才会回复数据；

## 相关文档

- [ZIGBEE串口协议说明](https://developer.tuya.com/cn/docs/iot/tuya-zigbee-module-uart-communication-protocol?id=K9ear5khsqoty)
- [ZIGBEE MCU SDK移植说明](https://developer.tuya.com/cn/docs/iot/mcu-sdk?id=K9ikp5uwm0rb9)
- [涂鸦 Demo 中心](https://developer.tuya.com/demo)


## 技术支持

您可以通过以下方法获得涂鸦的支持:

- [涂鸦 AI+IoT 开发者平台](https://developer.tuya.com)
- [帮助中心](https://support.tuya.com/help)
- [服务与支持](https://service.console.tuya.com)

