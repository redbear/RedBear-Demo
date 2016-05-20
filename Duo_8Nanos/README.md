# Duo-8Nanos

This demo allows a TCP client to connect to the Duo and conntrolling up to 8 BLE peripherals.


## Prerequisites

* 1 or up to 8 BLE Nano boards
* Duo board
* iOS or Android phone
	(For iOS, you need to compile yourself with your iOS developer license)

## Setup

* Follow [this](https://github.com/redbear/STM32-Arduino) to load the firmware (v0.2.4-rc3) and the `Duo_BLECentral` sketch to the Duo.
* Follow [this](https://github.com/redbear/nRF51822-Arduino) to load the ```BLENano_BLEPeripheral``` sketch to the BLE Nano.
* For Android: compile or load the APK file inside ```Android``` to your Android phone.
* For iOS: compile the project inside ```iOS``` to your iPhone, iPod touch or iPad.


## How it works

The Duo starts as a TCP server and waiting for a connection from TCP client. In this example, we will use iOS and Android for the TCP client. After the connection is made, the Duo will scan for the nearby BLE Nano.

Inside the Duo_BLECentral/BLE_Central_Multi_Peripherals/ble_nano.h, change the number of the BLE Nano you want to connect.

```#define NANO_NUM            2```

The Duo also runs mDNS service, so that you do not need to enter the IP address or port number assigned.
 
