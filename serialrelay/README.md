Tasmota modified for Serial Relay Boards
=============

This firmware is a modified version of **Tasmota 12** wich handle 2 majors families of serial relay boards :

### ICSE01xA boards

ICSE01xA boards come with a TTL serial interface. You need to provide a controler.

![ICSE013A](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-icse013a.png) ![ICSE012A](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-icse012a.png) ![ICSE014A](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-icse014a.png)

 ### LC Technology boards

LC Technology boards come with an ESP-01, but features provided are far from what tasmota provides.

![LC Tech x1](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-lctech-x1.png) ![LC Tech x2](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-lctech-x2.png) ![LC Tech x4](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-lctech-x4.png) 


This tasmota modified version fully manage these boards without any specific configuration needed for GPIO or rules. Relays are managed like any relay directly connected to the ESP controler. Only thing you have to do is to select the board type you've connected and after a reboot, everything should be working.

Firmware is configured to use the Normally Open (NO) input. When relay is ON, contact is closed.

These serial relay boards are quite cheap, but not hyper reactive. So, you should expect few seconds to fully initialise the board.

It has been tested succesfully on a **HW-034** (ICSE012A), **HW-149** (ICSE014A), **LC Tech x2** and **LC Tech x4** boards.

## LC Technology specificities

LC Technology boards work according to 2 different modes :
  * **Mode 1** : a **blue LED** is ON
  * **Mode 2** : a **red LED** is ON
 
This tasmota firmware handles both modes. \
But if you are in Mode 1, it will take almost 15 seconds to get an operational boards at boot time. \
In mode 2, you'll need only 2-3 seconds.

You can switch from **Mode 2 to Mode 1** anytime by pressing **S1**. \
To switch from **Mode 1 to Mode 2**, you need to press **S1** button **while powering** the board. 

## ICSE01xA specificities

As ICSE01xA board works on 5V input, if you plan to use it with a Sonoff Basic or any ESP8266 board, you'll need an interface board to adjust **Rx** and **Tx** from 5V to 3.3V levels. Here is an example of working interface board using an **ESP-01**. Power supply is directly provided by the ICSE01xA board thru the micro-USB port. You just need a 5V / 1A USB charger.

Interface
---------

![ESP01 interface](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-icse-diagram.png)

Here is an example of PCB board that implements this diagram.

![ESP01 board](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-icse-pcb.png)

Screenshots
-----------

![Main screen](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-serialrelay-main.png) ![Board selection](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-serialrelay-boardselect.png)

Compilation
-----------

If you want to compile this firmware version, you just need to :
1. install official tasmota sources
2. place or replace files from this repository
3. place specific files from **tasmota/common** repository

Here is where you should place different files from this repository and from **tasmota/common** :
* **platformio_override.ini**
* tasmota/**user_config_override.h**
* tasmota/tasmota_drv_driver/**xdrv_01_9_webserver.ino**
* tasmota/tasmota_drv_driver/**xdrv_94_ip_address.ino**
* tasmota/tasmota_drv_driver/**xdrv_96_serial_relay.ino**
* tasmota/tasmota_sns_sensor/**xsns_120_timezone.ino**

If everything goes fine, you should be able to compile your own build.
