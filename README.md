# TuyaOS Embedded Zigbee E-Ink Display

[English](./README.md) | [中文](./README_zh.md)

<br>

## Introduction

In this demo, we will show you how to build an e-ink display and make it IoT-enabled. Based on the [Tuya IoT Development Platform](https://iot.tuya.com/), we use Tuya's Zigbee module, MCU SDK, and the Smart Lift app to connect this display to the cloud. Regarding the hardware, we need one MCU, Zigbee module, e-ink display, and font library module to implement features including full and partial refresh, desk information entry and update, desk booking display and update, low battery warning, and device reset. You can sync information like desk ID and QR code with the e-ink display in real time by using the Smart Life app.

<br>

## Get started

### Set up development environment

- Set up the IDE as per the user guide by Keil.

   [Download the MDK.](https://www.keil.com/download/product/)

   [Install the MDK5 software packs.](https://www.keil.com/dd2/pack/)

- Port Tuya's MCU SDK and compile your project.

<br>

### Compile and flash

- Edit code

   1. In `protocol.h`, specify the PID of the product you created on the [Tuya IoT Development Platform](https://iot.tuya.com/).

      ```
       #define PRODUCT_KEY     "xxxxxxxx"
      ```

      Change `xxxxxxxx` to the PID.

- Compile code

   Correct the file according to the error messages, compile your project, and download code to the hardware. Then, you can use Tuya's debugging assistant to verify the serial communication between the MCU and the Zigbee module.

<br>

### File introduction
```c
├── User
│   ├── main.c                                             /* Entry to the main application */
│   └── MY_ST_config.h                                     /* Hardware configuration */
├── system                                                 /* System files */
│   ├── delay.c                                            /* Delay function */
│   ├── delay.h
│   ├── EPAPER.c                                           /* E-ink display initialization */
│   ├── EPAPER.h
│   ├── GT5SLAD3BFA_stm32l431_keil5.lib                    /* Static library for the font library module  */
│   ├── GT5SLAD3B-GTA6401.h                                /* Header file of the font library module */
│   ├── IO.c                                               /* GPIO initialization */
│   ├── IO.h
│   ├── key.c                                              /* Key initialization */
│   ├── key.h
│   ├── picture.h                                          /* Pixel data storage */
│   ├── RCC.c                                              /* System clock configuration */
│   ├── RCC.h
│   ├── SPI.c                                              /* SPI initialization */
│   ├── SPI.h
│   ├── sys.c                                              /* System task files */                 
│   ├── sys.h
│   ├── qrcode_create.c                                    /* QR code components */
│   ├── qrcode_create.h
│   ├── tuya_qrcode_create.c                               
│   ├── tuya_qrcode_create.h
│   ├── USART.c                                            /* UART initialization */
│   ├── USART.h
│   ├── utf8ToUnicode.c                                    /* Convert UTF-8 to Unicode */
│   └── utf8ToUnicode.h
├── CJSON
│   ├── cJSON.c                                            /* Configuration file in JSON */
│   └── cJSON.h
├── mcu_sdk
│   ├── mcu_api.c                                          /* Data point (DP) data */
│   ├── mcu_api.h
│   ├── protocol.c                                         /* Protocol parsing and response to received serial data */
│   ├── protocol.h
│   ├── system.c                                           /* Architecture for serial communication */
│   ├── system.h
│   └── zigbee.h                                           /* Macro definitions */
```

### Entry to application
Entry file: `/user/main.c`

+ `int main(void)` is executed to initialize the system project. This function is executed only once.
+ `while(1)` is used to execute the application code. It is executed in a loop.



### Pin configuration

| Features | **Peripheral interfaces** | **Pins** |
| :--------: | :--: | :--: |
| Communication between the MCU and the Zigbee module. | USART2 | PA2 PA3 |
| Log printing (USB) | USART3 | PB11 PB10 |
| E-ink display CS | SPI2 | PB1 |
| Font library module CS | SPI2 | PA12 |
| E-ink display | SPI2 | PB5  PB8  PB9 |
| Font library module | SPI2 | PB13  PB14  PB15 |
| The module wakes up the MCU | GPIO | PB0 |
| The MCU wakes up the module | GPIO | PC13 |

### FAQs

- Q: The board is not detected when I try to download code to it.

   A: STM32 enables either an SWD or JTAG protocol to be used on the debug port. You need to choose the correct debug mode in the software based on your hardware connection.

- Q: Partial refresh results in ghosting on the screen.

   A: You can refresh the e-ink screen to white multiple times to remove the ghost image. After that, partial refresh, light intensity, and high partial refresh rate can also cause the faint display of text or graphics.

- Q: I use the MCU as the controller and e-ink display module and font library module as the agents. When the MCU gets character data from the font library module and determines the MISO, I get an error of incorrect variable definition.

   A: If you specify the MCU as the input, you must configure this pin as read. Define the MISO macro as follows.

  ```c
  #define MISO		HAL_GPIO_ReadPin(ZK_SPI2_PORT, ZK_SPI2_MISO_PIN)
  ```


- Q: When I use the MCU simulation mode in the debugging assistant to verify serial communication, the module response failed.

   A: In the MCU simulation mode, the serial port must be connected to the external `MTX` and `MRX` that are connected to the `PA2` and `PA3`.

- Q: The module is offline after pairing.

- A: The possible reasons:

   a: Network downtime.

   b: The MCU sends a reset command to the module. The module enters pairing mode.

   c: The MCU does not respond to the heartbeat within 90 seconds after pairing.

   You can remove the paired device from the mobile app and pair it again.

- Q: The MCU responds to the module's commands after four to five seconds.

   A: The polling cycle of the Zigbee module is set to five seconds. Therefore, after the module sends a command, the MCU responds after a polling cycle ends.

- Q: The module does not send data but keeps restarting when I debug the low power feature.

   A: The polling cycle of the Zigbee module is set to five seconds. Therefore, after the module sends a command, the MCU responds after a polling cycle ends.

## Reference

- [Zigbee serial protocol](https://developer.tuya.com/en/docs/iot/tuya-zigbee-module-uart-communication-protocol?id=K9ear5khsqoty)
- [Port the MCU SDK](https://developer.tuya.com/en/docs/iot/mcu-sdk?id=K9ikp5uwm0rb9)
- [Tuya Project Hub](https://developer.tuya.com/demo)


## Technical support

You can get support from Tuya with the following methods:

+ [Tuya Developer Platform](https://developer.tuya.com/en/)
+ [Help Center](https://support.tuya.com/en/help)
+ [Service & Support](https://service.console.tuya.com)

<br>
