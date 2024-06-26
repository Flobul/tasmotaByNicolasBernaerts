/*
  user_config_override.h - user configuration overrides my_user_config.h for Tasmota

  Copyright (C) 2019-2024  Theo Arends, Nicolas Bernaerts

    05/05/2019 - v1.0  - Creation
    16/05/2019 - v1.1  - Add Tempo and EJP contracts
    08/06/2019 - v1.2  - Handle active and apparent power
    05/07/2019 - v2.0  - Rework with selection thru web interface
    02/01/2020 - v3.0  - Functions rewrite for Tasmota 8.x compatibility
    05/02/2020 - v3.1  - Add support for 3 phases meters
    14/03/2020 - v3.2  - Add apparent power graph
    05/04/2020 - v3.3  - Add Timezone management
    13/05/2020 - v3.4  - Add overload management per phase
    19/05/2020 - v3.6  - Add configuration for first NTP server
    26/05/2020 - v3.7  - Add Information JSON page
    29/07/2020 - v3.8  - Add Meter section to JSON
    05/08/2020 - v4.0  - Major code rewrite, JSON section is now TIC, numbered like new official Teleinfo module
                         Web sensor display update
    18/09/2020 - v4.1  - Based on Tasmota 8.4
    07/10/2020 - v5.0  - Handle different graph periods and javascript auto update
    18/10/2020 - v5.1  - Expose icon on web server
    25/10/2020 - v5.2  - Real time graph page update
    30/10/2020 - v5.3  - Add TIC message page
    02/11/2020 - v5.4  - Tasmota 9.0 compatibility
    09/11/2020 - v6.0  - Handle ESP32 ethernet devices with board selection
    11/11/2020 - v6.1  - Add data.json page
    20/11/2020 - v6.2  - Checksum bug
    29/12/2020 - v6.3  - Strengthen message error control
    25/02/2021 - v7.0  - Prepare compatibility with TIC standard
                         Add power status bar
    05/03/2021 - v7.1  - Correct bug on hardware energy counter
    08/03/2021 - v7.2  - Handle voltage and checksum for horodatage
    12/03/2021 - v7.3  - Use average / overload for graph
    15/03/2021 - v7.4  - Change graph period parameter
    21/03/2021 - v7.5  - Support for TIC Standard
    29/03/2021 - v7.6  - Add voltage graph
    04/04/2021 - v7.7  - Change in serial port & graph height selection
                         Handle number of indexes according to contract
                         Remove use of String to avoid heap fragmentation 
    14/04/2021 - v7.8  - Calculate Cos phi and Active power (W)
    21/04/2021 - v8.0  - Fixed IP configuration and change in Cos phi calculation
    29/04/2021 - v8.1  - Bug fix in serial port management and realtime energy totals
                         Control initial baud rate to avoid crash (thanks to Seb)
    26/05/2021 - v8.2  - Add active power (W) graph
    22/06/2021 - v8.3  - Change in serial management for ESP32
    04/08/2021 - v9.0  - Tasmota 9.5 compatibility
                         Add LittleFS historic data record
                         Complete change in VA, W and cos phi measurement based on transmission time
                         Add PME/PMI ACE6000 management
                         Add energy update interval configuration
                         Add TIC to TCP bridge (command 'tcpstart 8888' to publish teleinfo stream on port 8888)
    04/09/2021 - v9.1  - Save settings in LittleFS partition if available
                         Log rotate and old files deletion if space low
    10/10/2021 - v9.2  - Add peak VA and V in history files
    02/11/2021 - v9.3  - Add period and totals in history files
                         Add simple FTP server to access history files
    13/03/2022 - v9.4  - Change keys to ISUB and PSUB in METER section
    20/03/2022 - v9.5  - Change serial init and major rework in active power calculation
    01/04/2022 - v9.6  - Add software watchdog feed to avoid lock
    22/04/2022 - v9.7  - Option to minimise LittleFS writes (day:every 1h and week:every 6h)
                         Correction of EAIT bug
    04/08/2022 - v9.8  - Based on Tasmota 12, add ESP32S2 support
                         Remove FTP server auto start
    18/08/2022 - v9.9  - Force GPIO_TELEINFO_RX as digital input
                         Correct bug littlefs config and graph data recording
                         Add Tempo and Production mode (thanks to Sébastien)
                         Correct publication synchronised with teleperiod
    26/10/2022 - v10.0 - Add bar graph monthly (every day) and yearly (every month)
    06/11/2022 - v10.1 - Bug fixes on bar graphs and change in lltoa conversion
    15/11/2022 - v10.2 - Add bar graph daily (every hour)
    04/02/2023 - v10.3 - Add graph swipe (horizontal and vertical)
                         Disable wifi sleep on ESP32 to avoid latency
    25/02/2023 - v11.0 - Split between xnrg and xsns
                         Use Settings->teleinfo to store configuration
                         Update today and yesterday totals
    03/06/2023 - v11.1 - Graph curves live update
                         Rewrite of Tasmota energy update
                         Avoid 100ms rules teleperiod update
    11/06/2023 - v11.2 - Change graph organisation & live display
    15/08/2023 - v11.3 - Evolution in graph navigation
                         Change in XMLHttpRequest management 
    10/10/2023 - v12.0 - Add support for Ecowatt signal in ESP32 versions
    17/10/2023 - v12.1 - Handle Production & consommation simultaneously
                         Display all periods with total
    07/11/2023 - v12.2 - Rotate daily and weekly files every second
                         Change in ecowatt stream reception to avoid overload
                         Remove daily graph to save RAM on ESP8266 1Mb
                         Change daily filename to teleinfo-day-00.csv
    19/11/2023 - v13.0 - Tasmota 13 compatibility
                         Switch to safeboot partitionning
                         Calculate active power for production
    05/12/2023 - v13.1 - Add RTE Tempo calendar
    07/12/2023 - v13.2 - Handle both Ecowatt v4 and v5 (command eco_version)
    03/01/2024 - v13.3 - Add alert management thru STGE
    15/01/2024 - v13.6 - Add support for Denky (thanks to C. Hallard prototype)
                         Add RTE pointe API
                         Add Emeraude 2 meter management
                         Add calendar and virtual relay management
    25/02/2024 - v14.0 - Complete rewrite of Contrat and Period management
                         Activate serial reception when NTP time is ready
                         Change MQTT publication and data reception handling to minimize errors
                         Support various temperature sensors
                         Add Domoticz topics publication (idea from Sebastien)
                         Add support for Wenky with deep sleep (thanks to C. Hallard prototype)
                         Lots of bug fixes (thanks to B. Monot and Sebastien)
                         Separation of curve and historisation sources
                         Removal of all float calculations
    27/03/2024 - v14.1 - Integration of Home Assistant auto discovery (with help of msevestre31)
                         Section COUNTER renamed as CONTRACT with addition of contract data
    28/03/2024 - v14.2 - Add Today and Yesterday conso and Prod
                         Disable Tasmota auto-discovery
    04/04/2024 - v14.3 - Correct RTE Tempo summer bug (different URL for Heure Ete / Heure Hiver)
                         All ESP32 are using Arduino 3.0 libraries
                         Add integration of Homie auto-discovery
                         Update Home assistant auto-discovery with state_class

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License aStart STGE managements published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _USER_CONFIG_OVERRIDE_H_
#define _USER_CONFIG_OVERRIDE_H_
#endif      // _USER_CONFIG_OVERRIDE_H_

// force the compiler to show a warning to confirm that this file is included
//#warning **** user_config_override.h: Using Settings from this File ****

/*****************************************************************************************************\
 * USAGE:
 *   To modify the stock configuration without changing the my_user_config.h file:
 *   (1) copy this file to "user_config_override.h" (It will be ignored by Git)
 *   (2) define your own settings below
 *
 ******************************************************************************************************
 * ATTENTION:
 *   - Changes to SECTION1 PARAMETER defines will only override flash settings if you change define CFG_HOLDER.
 *   - Expect compiler warnings when no ifdef/undef/endif sequence is used.
 *   - You still need to update my_user_config.h for major define USE_MQTT_TLS.
 *   - All parameters can be persistent changed online using commands via MQTT, WebConsole or Serial.
\*****************************************************************************************************/

/********************************************\
 *    Teleinfo firmware configuration
\********************************************/

// extension description
#define EXTENSION_NAME    "Teleinfo"          // name
#define EXTENSION_AUTHOR  "Nicolas Bernaerts" // author
#define EXTENSION_VERSION "14.3"              // version

// FTP server credentials
#ifdef USE_FTPSERVER
#define FTP_SERVER_LOGIN      "teleinfo"      // FTP server login
#define FTP_SERVER_PASSWORD   "teleinfo"      // FTP server password
#endif

// complementary modules
#ifdef USE_TELEINFO
#define USE_IPADDRESS                         // Add fixed IP configuration page
#define USE_TIMEZONE                          // Enable Timezone management
#define USE_TIMEZONE_WEB_CONFIG               // Enable timezone web configuration page
#define USE_TCPSERVER                         // Enable TCP server (for TIC to TCP)
# endif     // USE_TELEINFO

// home management integration
#define USE_TELEINFO_DOMOTICZ                 // Domoticz intégration
#define USE_TELEINFO_HASS                     // Home Assistant auto-discovery intégration
#define USE_TELEINFO_HOMIE                    // Homie protocol auto-discovery intégration

// build
#ifdef BUILD_ESP32S3_16M
#define EXTENSION_BUILD "esp32s3_16m"

#elif BUILD_ESP32S3_4M
#define EXTENSION_BUILD "esp32s3-4m"

#elif BUILD_ESP32S2
#define EXTENSION_BUILD "esp32s2-4m"

#elif BUILD_ESP32_DENKYD4
#define EXTENSION_BUILD "denkyd4-8m"
#define USER_TEMPLATE "{\"NAME\":\"Denky D4\",\"GPIO\":[32,0,0,0,1,0,0,0,0,1,1376,1,0,0,0,0,0,640,608,0,0,0,0,0,0,0,5632,0,0,0,0,0,0,0,0,0],\"FLAG\":0,\"BASE\":1}" 

#elif BUILD_ESP32_WINKY
#define EXTENSION_BUILD "winky-4m"
#define USER_TEMPLATE "{\"NAME\":\"Winky\",\"GPIO\":[1,4704,1376,4705,5632,4706,640,608,1,32,1,0,0,0,0,0,0,0,1,1,1,1,1,4096,0,0,0,0,0,0,0],\"FLAG\":0,\"BASE\":1}" 

#elif BUILD_ESP32_4M
#define EXTENSION_BUILD "esp32-4m"

#elif BUILD_16M14M
#define EXTENSION_BUILD "esp8266-16m"

#elif BUILD_4M2M
#define EXTENSION_BUILD "esp8266-4m"

#elif BUILD_2M1M
#define EXTENSION_BUILD "esp8266-2m"

#elif BUILD_1M
#define EXTENSION_BUILD "esp8266-1m"

#else
#define EXTENSION_BUILD "other"
#endif

// teleinfo display is in French
#define MY_LANGUAGE        fr_FR

// MQTT default
#undef MQTT_HOST
#define MQTT_HOST          "mqtt.local"
#undef MQTT_PORT
#define MQTT_PORT          1883              
#undef MQTT_USER
#define MQTT_USER          ""
#undef MQTT_PASS
#define MQTT_PASS          ""
#undef MQTT_TOPIC
#define MQTT_TOPIC         "teleinfo"
#undef MQTT_FULLTOPIC
#define MQTT_FULLTOPIC     "%topic%/%prefix%/"
#undef FRIENDLY_NAME
#define FRIENDLY_NAME      "Teleinfo"

// disable serial log
#undef SERIAL_LOG_LEVEL 
#define SERIAL_LOG_LEVEL   LOG_LEVEL_NONE

#define MDNS_ENABLE false                     // disable multicast DNS

// ----------------------
// Common ESP8266 & ESP32
// ----------------------

#define MQTT_DATA_STRING                      // Enable use heap instead of fixed memory for TasmotaGlobal.mqtt_data

#define RULE_MAX_EVENTSZ        256           // increase rules buffer

#undef USE_ARDUINO_OTA                        // support for Arduino OTA
#undef USE_WPS                                // support for WPS as initial wifi configuration tool
#undef USE_SMARTCONFIG                        // support for Wifi SmartConfig as initial wifi configuration tool
#undef USE_MQTT_TLS                           // TLS support won't work as the MQTTHost is not set

#undef USE_DOMOTICZ                           // Disable official Domoticz

#undef USE_TASMOTA_DISCOVERY                  // Enable Tasmota discovery
#undef USE_HOME_ASSISTANT                     // Disable historic Home Assistant

#undef USE_KNX                                // KNX IP Protocol Support
#undef USE_KNX_WEB_MENU                       // Enable KNX WEB MENU (+8.3k code, +144 mem)

//#undef USE_WEBSERVER                        // Webserver
#undef USE_EMULATION_HUE                      // Hue Bridge emulation for Alexa (+14k code, +2k mem common)
#undef USE_EMULATION_WEMO                     // Belkin WeMo emulation for Alexa (+6k code, +2k mem common)
#undef USE_CUSTOM                             // Custom features

//#define USE_DISCOVERY                          // Discovery services for both MQTT and web server
//#define WEBSERVER_ADVERTISE                    // Provide access to webserver by name <Hostname>.local/
#undef MQTT_HOST_DISCOVERY                    // Find MQTT host server (overrides MQTT_HOST if found)

#undef USE_TIMERS                             // support for up to 16 timers
#undef USE_TIMERS_WEB                         // support for timer webpage
#undef USE_SUNRISE                            // support for Sunrise and sunset tools

//#undef USE_UNISHOX_COMPRESSION                // Add support for string compression in Rules or Scripts

#undef USE_SCRIPT                             // Add support for script (+17k code)

#define USE_RULES                             // Support for rules
  #define USE_EXPRESSION                      // Add support for expression evaluation in rules (+3k2 code, +64 bytes mem)  
    #define SUPPORT_IF_STATEMENT              // Add support for IF statement in rules (+4k2 code, -332 bytes mem)  

#undef ROTARY_V1                              // Add support for Rotary Encoder as used in MI Desk Lamp (+0k8 code)
#undef USE_SONOFF_RF                          // Add support for Sonoff Rf Bridge (+3k2 code)
#undef USE_RF_FLASH                           // Add support for flashing the EFM8BB1 chip on the Sonoff RF Bridge.
#undef USE_SONOFF_SC                          // Add support for Sonoff Sc (+1k1 code)
#undef USE_TUYA_MCU                           // Add support for Tuya Serial MCU
#undef USE_ARMTRONIX_DIMMERS                  // Add support for Armtronix Dimmers (+1k4 code)
#undef USE_PS_16_DZ                           // Add support for PS-16-DZ Dimmer (+2k code)
#undef USE_SONOFF_IFAN                        // Add support for Sonoff iFan02 and iFan03 (+2k code)
#undef USE_BUZZER                             // Add support for a buzzer (+0k6 code)
#undef USE_ARILUX_RF                          // Add support for Arilux RF remote controller (+0k8 code, 252 iram (non 2.3.0))
#undef USE_SHUTTER                            // Add Shutter support for up to 4 shutter with different motortypes (+11k code)
//#undef USE_DEEPSLEEP                          // Add support for deepsleep (+1k code)
#undef USE_EXS_DIMMER                         // Add support for ES-Store WiFi Dimmer (+1k5 code)
#undef USE_DEVICE_GROUPS                      // Add support for device groups (+5k5 code)
#undef USE_DEVICE_GROUPS_SEND                 // Add support for the DevGroupSend command (+0k6 code)
#undef USE_PWM_DIMMER                         // Add support for MJ-SD01/acenx/NTONPOWER PWM dimmers (+2k2 code, DGR=0k4)
#undef USE_PWM_DIMMER_REMOTE                  // Add support for remote switches to PWM Dimmer (requires USE_DEVICE_GROUPS) (+0k9 code)
#undef USE_SONOFF_D1                          // Add support for Sonoff D1 Dimmer (+0k7 code)
#undef USE_SHELLY_DIMMER                      // Add support for Shelly Dimmer (+3k code)
#undef SHELLY_CMDS                            // Add command to send co-processor commands (+0k3 code)
#undef SHELLY_FW_UPGRADE                      // Add firmware upgrade option for co-processor (+3k4 code)

#undef USE_MY92X1                             // Add support for MY92X1 RGBCW led controller as used in Sonoff B1, Ailight and Lohas
#undef USE_SM16716                            // Add support for SM16716 RGB LED controller (+0k7 code)
#undef USE_SM2135                             // Add support for SM2135 RGBCW led control as used in Action LSC (+0k6 code)
#undef USE_SONOFF_L1                          // Add support for Sonoff L1 led control
#undef USE_ELECTRIQ_MOODL                     // Add support for ElectriQ iQ-wifiMOODL RGBW LED controller (+0k3 code)
#undef USE_LIGHT_PALETTE                      // Add support for color palette (+0k7 code)
#undef USE_LIGHT_VIRTUAL_CT                   // Add support for Virtual White Color Temperature (+1.1k code)
#undef USE_DGR_LIGHT_SEQUENCE                 // Add support for device group light sequencing (requires USE_DEVICE_GROUPS) (+0k2 code)

//#undef USE_COUNTER                            // Enable inputs as counter (+0k8 code)

//#define USE_ADC_VCC                            // display analog input as VCC

#undef USE_SPI                                // Disable all SPI devices

#undef USE_SERIAL_BRIDGE                      // Disable support for software Serial Bridge (+0k8 code)

#ifdef USE_TELEINFO
#define USE_DS18x20                            // Add support for DS18x20 sensors with id sort, single scan and read retry (+2k6 code)
#define USE_DHT                                // Disable internal DHT sensor

#define USE_I2C                               // Enable all I2C sensors and devices
#define USE_SHT3X                             // Enable SHT30 and SHT40
#define USE_SHT                                // [I2cDriver8] Enable SHT1X sensor (+1k4 code)
#define USE_HTU                                // [I2cDriver9] Enable HTU21/SI7013/SI7020/SI7021 sensor (I2C address 0x40) (+1k5 code)
#define USE_BMP                                // [I2cDriver10] Enable BMP085/BMP180/BMP280/BME280 sensors (I2C addresses 0x76 and 0x77) (+4k4 code)

#define USE_ENERGY_SENSOR                      // Disable energy sensors
#endif    // USE_TELEINFO

#undef USE_ENERGY_MARGIN_DETECTION            // Add support for Energy Margin detection (+1k6 code)
#undef USE_ENERGY_POWER_LIMIT                 // Add additional support for Energy Power Limit detection (+1k2 code)
#undef USE_PZEM004T                           // Add support for PZEM004T Energy monitor (+2k code)
#undef USE_PZEM_AC                            // Add support for PZEM014,016 Energy monitor (+1k1 code)
#undef USE_PZEM_DC                            // Add support for PZEM003,017 Energy monitor (+1k1 code)
#undef USE_MCP39F501                          // Add support for MCP39F501 Energy monitor as used in Shelly 2 (+3k1 code)
#undef USE_ENERGY_DUMMY                       // Add support for dummy Energy monitor allowing user values (+0k7 code)
#undef USE_HLW8012                            // Add support for HLW8012, BL0937 or HJL-01 Energy Monitor for Sonoff Pow and WolfBlitz
#undef USE_CSE7766                            // Add support for CSE7766 Energy Monitor for Sonoff S31 and Pow R2
#undef USE_SDM72                              // Add support for Eastron SDM72-Modbus energy monitor (+0k3 code)
#undef USE_SDM120                             // Add support for Eastron SDM120-Modbus energy monitor (+1k1 code)
#undef USE_SDM630                             // Add support for Eastron SDM630-Modbus energy monitor (+0k6 code)
#undef USE_DDS2382                            // Add support for Hiking DDS2382 Modbus energy monitor (+0k6 code)
#undef USE_DDSU666                            // Add support for Chint DDSU666 Modbus energy monitor (+0k6 code)
#undef USE_SOLAX_X1                           // Add support for Solax X1 series Modbus log info (+3k1 code)
#undef USE_LE01MR                             // Add support for F&F LE-01MR Modbus energy monitor (+1k code)
#undef USE_BL09XX                             // Add support for various BL09XX Energy monitor as used in Blitzwolf SHP-10 or Sonoff Dual R3 v2 (+1k6 code)
//#undef USE_TELEINFO                           // Add support for Teleinfo via serial RX interface (+5k2 code, +168 RAM + SmartMeter LinkedList Values RAM)
#undef USE_IEM3000                            // Add support for Schneider Electric iEM3000-Modbus series energy monitor (+0k8 code)
#undef USE_ADE7953                            // support for ADE7953 energy sensor 
#undef USE_SONOFF_SPM

#undef USE_AC_ZERO_CROSS_DIMMER

#undef USE_IR_REMOTE                          // Send IR remote commands using library IRremoteESP8266 and ArduinoJson (+4k3 code, 0k3 mem, 48 iram)
#undef USE_IR_REMOTE_FULL                     // complete integration of IRremoteESP8266 for Tasmota
#undef USE_IR_SEND_NEC                        // Support IRsend NEC protocol
#undef USE_IR_SEND_RC5                        // Support IRsend Philips RC5 protocol
#undef USE_IR_SEND_RC6                        // Support IRsend Philips RC6 protocol
#undef USE_IR_RECEIVE                         // Support for IR receiver (+7k2 code, 264 iram)

#undef USE_ZIGBEE                             // Enable serial communication with Zigbee CC2530 flashed with ZNP (+49k code, +3k mem)
#undef USE_ZIGBEE_ZNP                         // Enable ZNP protocol, needed for CC2530 based devices
#undef USE_ZIGBEE_EZSP                        // Enable EZSP protocol, needed for EFR32 EmberZNet based devices, like Sonoff Zigbee bridge                                             
#undef USE_ZIGBEE_EEPROM                      // Use the EEPROM from the Sonoff ZBBridge to save Zigbee configuration and data
#undef USE_ZBBRIDGE_TLS                       // TLS support for zbbridge

#undef USE_TM1638                             // Add support for TM1638 switches copying Switch1 .. Switch8 (+1k code)
#undef USE_HX711                              // Add support for HX711 load cell (+1k5 code)
#undef USE_HX711_GUI                          // Add optional web GUI to HX711 as scale (+1k8 code)
#undef USE_TX20_WIND_SENSOR                   // Add support for La Crosse TX20 anemometer (+2k6/0k8 code)
#undef USE_TX23_WIND_SENSOR                   // Add support for La Crosse TX23 anemometer (+2k7/1k code)
#undef USE_WINDMETER                          // Add support for analog anemometer (+2k2 code)
#undef USE_FTC532                             // Add support for FTC532 8-button touch controller (+0k6 code)
#undef USE_RC_SWITCH                          // Add support for RF transceiver using library RcSwitch (+2k7 code, 460 iram)
#undef USE_RF_SENSOR                          // Add support for RF sensor receiver (434MHz or 868MHz) (+0k8 code)
#undef USE_THEO_V2                            // Add support for decoding Theo V2 sensors as documented on https://sidweb.nl using 434MHz RF sensor receiver (+1k4 code)
#undef USE_ALECTO_V2                          // Add support for decoding Alecto V2 sensors like ACH2010, WS3000 and DKW2012 weather stations using 868MHz RF sensor receiver (+1k7 code)
#undef USE_HRE                                // Add support for Badger HR-E Water Meter (+1k4 code)
#undef USE_A4988_STEPPER                      // Add support for A4988/DRV8825 stepper-motor-driver-circuit (+10k5 code)
#undef USE_PROMETHEUS                         // Add support for https://prometheus.io/ metrics exporting over HTTP /metrics endpoint
#undef USE_NEOPOOL                            // Add support for Sugar Valley NeoPool Controller - also known under brands Hidrolife, Aquascenic, Oxilife, Bionet, Hidroniser, UVScenic, 

#undef USE_THERMOSTAT                         // Add support for Thermostat
#undef USE_GPS
#undef USE_WINDMETER
#undef USE_OPENTHERM

#undef USE_TIMEPROP                           // Add support for the timeprop feature (+9k1 code)
#undef USE_PID                                // Add suport for the PID  feature (+11k2 code)

#undef USE_DRV_FILE_JSON_DEMO

//#define USE_TLS                               // for safeboot and BearSSL

// ----------------------
//    ESP32 specific
// ----------------------


#ifdef ESP32

// berry and autoconf
//#undef USE_AUTOCONF                           // Enable Esp32 autoconf feature
//#undef USE_BERRY                              // Enable Berry scripting langage

// display
#define USE_I2C                                // All I2C sensors and devices
#define USE_DISPLAY                             // Add Display support
#undef  USE_DISPLAY_TM1621_SONOFF

//#undef USE_ESP32_SENSORS

#define USE_TLS                               // for safeboot and BearSSL
#define USE_LIB_SSL_ENGINE

// conso LED status
#define USE_LIGHT                              // Add support for light control
#define USE_WS2812                             // WS2812 Led string using library NeoPixelBus (+5k code, +1k mem, 232 iram) - Disable by //
#define USE_ADC                                // Add support for ADC on GPIO32 to GPIO39

#undef USE_BLE_ESP32
#undef USE_MI_ESP32
#undef USE_IBEACON

#undef USE_SR04

#undef USE_WEBCAM

#undef USE_M5STACK_CORE2
#undef USE_I2S_AUDIO
#undef USE_TTGO_WATCH

#undef USE_ALECTO_V2
#undef USE_RF_SENSOR
#undef USE_HX711
#undef USE_MAX31855

#undef USE_MHZ19
#undef USE_SENSEAIR   

#endif  // ESP32

