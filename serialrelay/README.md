Tasmota modified for Serial Relay Boards
=============

This firmware is a modified version of **Tasmota 9.3.1** wich handle 2 majors families of serial relay boards :
  * **ICSE boards**
![ICSE013A](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/serialrelay-icse013a.png)

  * **LC Technology boards**


As ICSE01xA board works on 5V input, if you plan to use it with a Sonoff Basic (or similar), you'll need an interface board to adjust **Rx** and **Tx** from 5V (ICSE) to 3.3V (Sonoff) levels.

**ICSE01xA** chipstet protocol is very badly documented. But some very usefull informations are available on this [stackoverflow page](https://stackoverflow.com/questions/26913755/need-help-understading-sending-bytes-to-serial-port).

These boards are quite cheap, but not hyper reactive. So you should expect around 8 seconds to fully initialise the board.

This firmware detects the board connected to the serial port and setup the number of relays accordingly.

Firmware is configured to use the Normally Open (NO) contact of the relays.

It has been tested succesfully with few days stability on a **HW-149** board, based on ICSE014A.

Here is an example of working interface board for **ESP-01** :

![ESP01 interface](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-icse-diagram.png)

On this diagram, power supply comes fully from the micro-USB port of the ICSE01xA board. \
You just need a 5V / 1A USB charger for the complete setup.

Here is an example of PCB board that just fits and works :

![ESP01 board](https://raw.githubusercontent.com/NicolasBernaerts/tasmota/master/serialrelay/screen/tasmota-icse-pcb.png)
