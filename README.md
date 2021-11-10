# Tuya IoTOS Embedded Demo Zigbee E-ink-display

[English](./README.md) | [中文](./README_zh.md)

<br>

## Introduction

This demo is based on [Tuya IoT Platform](https://iot.tuya.com/), Tuya Smart APP, IoTOS Zigbee MCU SDK, using Tuya Zigbee series modules quickly implement a smart E-ink-display application. The hardware part of the E-ink-display consists of 1 MCU, 1 communication module, 1 ink screen and 1 word library chip, which realizes functions such as the seat information setting and update,reservation information display, reservation information update, reservation information synchronization, low power alarm,equipment reset and other functions. In addition, the smart hula hoop also realizes the counting function of time, number of laps, and calories. Among them, the number of laps is detected by a switch-type Hall sensor. In addition,the demo also realized the screen refresh local brush and full brush. The developer can deliver the information displayed on the screen in real time through the Tuya Smart APP, such as seat number,Qr code image Settings.

<br>

## Get started

### Set up development environment

- The IDE is installed according to KEIL's official instructions.

   [MDK download](https://www.keil.com/download/product/)

   [Install chip pack](https://www.keil.com/dd2/pack/)

- The migration methods of the Tuya ZIGBEE MCU SDK are as follows: Modify the errors as prompted during project compilation to complete project compilation.


<br>

### Compile and flash

- Edit code

   1. In `protocol.h`, specify the PID of the product you have created on the [Tuya IoT Platform](https://iot.tuya.com/).

      ```
      #define PRODUCT_KEY     "xxxxxxxx"
      ```

      Change `xxxxxxxx` to the PID.

- Compile and run Demo code

   After modifying the file according to the error prompted by SDK compilation, compile the system engineering and download the project to the hardware operation. When debugging MCU and module, you can first use doodle serial debugging assistant to debug whether the two work normally.

<br>

### File introduction

```c
├── User
│   ├── main.c                                            /* local time module */
│   └── MY_ST_config.h                                     /* Hardware configuration file */
├── system                                                 /* System File directory */
│   ├── delay.c                                            /* delay function */
│   ├── delay.h
│   ├── EPAPER.c                                           /* electronic screen driver*/
│   ├── EPAPER.h
│   ├── GT5SLAD3BFA_stm32l431_keil5.lib                    /* Font chip.lib */
│   ├── GT5SLAD3B-GTA6401.h                                /* Font chip.h */
│   ├── IO.c                                               /* GPIO port init */
│   ├── IO.h
│   ├── key.c                                              /* KEY driver */
│   ├── key.h
│   ├── picture.h                                          /* Image pixel data storage */
│   ├── RCC.c                                              /* System Clock Configuration*/
│   ├── RCC.h
│   ├── SPI.c                                              /* SPI driver */
│   ├── SPI.h
│   ├── sys.c                                              /* System task file */                 
│   ├── sys.h
│   ├── qrcode_create.c                                 
│   ├── qrcode_create.h
│   ├── tuya_qrcode_create.c                                /* tuya QRcode component */
│   ├── tuya_qrcode_create.h
│   ├── USART.c                                            /* Serial Port Initialization */
│   ├── USART.h
│   ├── utf8ToUnicode.c                                    /* UTF8 to UNICODE */
│   └── utf8ToUnicode.h
├── CJSON
│   ├── cJSON.c                                            /* JSON configuration file */
│   └── cJSON.h
├── mcu_sdk
│   ├── mcu_api.c                                          /* Dp function data file */
│   ├── mcu_api.h
│   ├── protocol.c                          /* Protocol analysis and response of the receiving module when sending messages */
│   ├── protocol.h
│   ├── system.c                         /* Framework analysis of communication between single chip microcomputer and ZigBee */
│   ├── system.h
│   └── zigbee.h                                           /* Macro definitions used in the SDK */
```



### Entry to application
Entry file: `/user/main.c`

+ `int main(void)` is executed to initialize Tuya IoTOS Embedded Zigbee MCU SDK. This function is executed only once.
+ `while(1)` is used to execute the application code. It is executed in a loop.

<br>

### Data point (DP)

|                 Peripheral Function                  | **Peripheral interface** |  **Gpio Port**   |
| :--------------------------------------------------: | :----------------------: | :--------------: |
| Serial port for communication between MCU and module |          USART2          |     PA2 PA3      |
|                Print log serial port                 |          USART3          |    PB11 PB10     |
|                    Ink screen CS                     |           SPI2           |       PB1        |
|                   library chip CS                    |           SPI2           |       PA12       |
|                  Ink screen driver                   |           SPI2           |  PB5  PB8  PB9   |
|                 library chip driver                  |           SPI2           | PB13  PB14  PB15 |
|                  Module wake up MCU                  |        General IO        |       PB0        |
|                  MCUwake up Module                   |        General IO        |       PC13       |

### FAQ

- **Q：**Development board burning download can not detect the microcontroller

  **A:** STM32 has two download modes: SW and JTAG. If the hardware download port is connected to the SW port, the software tool Debug download mode should also be selected in SW mode.

- **Q：**The ink screen will leave a residual shadow in the brush, which is also a disadvantage of the brush;

  **A:**The residual shadow can be gradually eliminated by brushing the white screen for many times, but then the picture will gradually become shallower in the second brush, and the light intensity will also have an impact on the shallower local brush time; If the frequency of brush is higher, the color of the picture will be lighter, while if the frequency is lower, the color of the picture will be lighter.

- **Q：**MCU as the host, ink screen and font chip as the slave machine to choose, when MCU obtains the character data from the font, it needs to judge whether it is in MISO mode, at this time, compilation error variable definition error;

  **A:**MISO - Master Input Slave Output: data Input from the Master device and data Output from the Slave device. When MCU is input, the pin should be configured as read input mode. MISO macro definition should be changed

  ```c
  #define MISO		HAL_GPIO_ReadPin(ZK_SPI2_PORT, ZK_SPI2_MISO_PIN) 
  ```

- **Q：**When the MCU_SDK project was transplanted, doodle module debugging assistant was used to test whether MCU could communicate normally, which could be tested by MCU simulation and module simulation respectively. Module data recovery failed during MCU simulation.

  **A:**During MCU simulation, it is necessary to pay attention to whether the serial port wiring is correct. The serial port is connected to the external MTX and MRX connected to PA2 and PA3, so as to ensure the correct data recovery of the module.

- **Q：**After the completion of the module network, there will be disconnection, and the network needs to be re-configured.

​       **A:**There are several possibilities for the device to display offline:

​           a:The network is disconnected

​           b:MCU terminal sends the reset command and enters the network configuration mode

​          c: After network configuration, MCU does not respond to heartbeat packets within 90 seconds

​       When the device is offline, the device can be removed and the network can be re-configured, and   

​       the MCU can send the network instruction to re-configure the network.

- **Q：**When the module sends DP data, MCU responds after 4~5s;

  **A:**There is a poll period value in the ZigBee network policy parameter, which is the wake up period of the low-power module in the network. The poll period of the module is set to 5s. Therefore, after dp is delivered, MCU will wait for one poll period before returning data.

- **Q：**When debugging the low-power part of the module, the module could not find data and the device kept restarting.

- **A:**There is a poll period value in the ZigBee network policy parameter, which is the wake up period of the low-power module in the network. The poll period of the module is set to 5s. Therefore, after dp is delivered, MCU will wait for one poll period before returning data.

<br>

## Reference

- [ZIGBEE Serial protocol description](https://developer.tuya.com/en/docs/iot/tuya-zigbee-module-uart-communication-protocol?id=K9ear5khsqoty)
- [ZIGBEE MCU SDK transplant instructions](https://developer.tuya.com/en/docs/iot/mcu-sdk?id=K9ikp5uwm0rb9)
- [Tuya Project Hub](https://developer.tuya.com/demo)

<br>


## Technical support

You can get support from Tuya with the following methods:

+ [Tuya Developer Platform](https://developer.tuya.com/en/)
+ [Help Center](https://support.tuya.com/en/help)
+ [Service & Support](https://service.console.tuya.com)

<br>