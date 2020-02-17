/*
  xdrv_97_temperature.ino - Retreive Temperature thru MQTT
  
  Copyright (C) 2020  Nicolas Bernaerts
    15/01/2020 - v5.0 - Separate temperature driver and add remote MQTT sensor
                      
  Settings are stored using weighting scale parameters :
    - Settings.free_f03  = MQTT Instant temperature (powwer data|Temperature MQTT topic;Temperature JSON key)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*************************************************\
 *              Temperature
\*************************************************/

#ifdef USE_TEMPERATURE_MQTT

#define XDRV_97                     97
#define XSNS_97                     97

#define TEMPERATURE_BUFFER_SIZE     128
#define TEMPERATURE_TIMEOUT_5MN     300000

#define D_PAGE_TEMPERATURE          "temp"

#define D_CMND_TEMPERATURE_TOPIC    "ttopic"
#define D_CMND_TEMPERATURE_KEY      "tkey"

#define D_JSON_TEMPERATURE_VALUE    "State"
#define D_JSON_TEMPERATURE_TOPIC    "Topic"
#define D_JSON_TEMPERATURE_KEY      "Key"

// form strings
const char TEMPERATURE_FORM_START[] PROGMEM = "<form method='get' action='%s'>";
const char TEMPERATURE_FORM_STOP[] PROGMEM = "</form>";
const char TEMPERATURE_FIELDSET_START[] PROGMEM = "<fieldset><legend><b>&nbsp;%s&nbsp;</b></legend>";
const char TEMPERATURE_FIELDSET_STOP[] PROGMEM = "</fieldset><br />";

// temperature commands
enum TemperatureCommands { CMND_TEMPERATURE_TOPIC, CMND_TEMPERATURE_KEY };
const char kTemperatureCommands[] PROGMEM = D_CMND_TEMPERATURE_TOPIC "|" D_CMND_TEMPERATURE_KEY;

/*************************************************\
 *               Variables
\*************************************************/

// variables
float    temperature_mqtt_value       = NAN;          // current temperature
bool     temperature_topic_subscribed = false;        // flag for temperature subscription
uint32_t temperature_last_update      = 0;            // last time (in millis) temperature was updated

/**************************************************\
 *                  Accessors
\**************************************************/

// get current temperature MQTT topic
void TemperatureGetMqttTopic (String& str_topic)
{
  String str_setting, str_temperature;
  int position;

  // init
  str_topic = "";

  // extract temperature from settings
  str_setting = (char*)Settings.free_f03;
  position = str_setting.indexOf ('|');
  if (position > 0)
  {
    // extract temperature data
    str_temperature = str_setting.substring (position + 1);

    // extract topic from power data 
    position = str_temperature.indexOf(';');
    if (position > 0) str_topic = str_temperature.substring(0, position);
  }
}

// get current temperature JSON key
void TemperatureGetMqttKey (String& str_key)
{
  String str_setting, str_temperature;
  int position;

  // init
  str_key = "";

  // extract power data from settings
  str_setting = (char*)Settings.free_f03;
  position = str_setting.indexOf ('|');
  if (position >= 0)
  {
    // extract temperature data
    str_temperature = str_setting.substring (position + 1);
 
    // extract key from temperature data 
    position = str_temperature.indexOf (';');
    if (position > 0) str_key = str_temperature.substring (position + 1);
  }
}

// set current temperature MQTT topic
void TemperatureSetMqttTopic (char* str_topic)
{
  String str_setting, str_start, str_key;
  int position;

  // get key
  TemperatureGetMqttKey (str_key);

  // backup offloading data from settings
  str_setting = (char*)Settings.free_f03;
  position = str_setting.indexOf ('|');
  if (position >= 0) str_start = str_setting.substring (0, position);

  // save the full settings
  str_setting = str_start + "|" + String (str_topic) + ";" + str_key;
  strncpy ((char*)Settings.free_f03, str_setting.c_str (), 233);
}

// set current temperature JSON key
void TemperatureSetMqttKey (char* str_key)
{
  String str_setting, str_start, str_topic;
  int position;

  // get key
  TemperatureGetMqttTopic (str_topic);

  // backup offloading data from settings
  str_setting = (char*)Settings.free_f03;
  position = str_setting.indexOf ('|');
  if (position > 0) str_start = str_setting.substring(0, position);

  // save the full settings
  str_setting = str_start + "|" + str_topic + ";" + String (str_key);
  strncpy ((char*)Settings.free_f03, str_setting.c_str (), 233);
}

/**************************************************\
 *                  Functions
\**************************************************/

// get current temperature
float TemperatureGetValue ( )
{
  uint32_t time_now = millis ();

  // if no update for 5 minutes, temperature not available
  if (time_now - temperature_last_update > TEMPERATURE_TIMEOUT_5MN) temperature_mqtt_value = NAN;

  return temperature_mqtt_value;
}

// update temperature
void TemperatureSetValue (float new_temperature)
{
  // read temperature from sensor
  temperature_mqtt_value = new_temperature;

  // temperature updated
  temperature_last_update = millis ();
}

// Show JSON status (for MQTT)
void TemperatureShowJSON (bool append)
{
  String str_json, str_topic, str_key;
  float  temperature;

  // read data
  TemperatureGetMqttTopic (str_topic);
  TemperatureGetMqttKey (str_key);
  temperature = TemperatureGetValue ();

  // start message  -->  {  or message,
  if (append == false) str_json = "{";
  else str_json = String (mqtt_data) + ",";

  // temperature  -->  "Temperature":{"Value":18.5,"Topic":"mqtt/topic/of/temperature","Key":"Temp"}
  str_json += "\"" + String (D_JSON_TEMPERATURE) + "\":{";
  str_json += "\"" + String (D_JSON_TEMPERATURE_VALUE) + "\":" + String (temperature) + ",";
  str_json += "\"" + String (D_JSON_TEMPERATURE_TOPIC) + "\":\"" + str_topic + "\",";
  str_json += "\"" + String (D_JSON_TEMPERATURE_KEY) + "\":\"" + str_key + "\"}";

  // if not in append mode, add last bracket 
  if (append == false) str_json += "}";

  // add json string to MQTT message
  snprintf_P (mqtt_data, sizeof(mqtt_data), str_json.c_str ());

  // if not in append mode, publish message 
  if (append == false) MqttPublishPrefixTopic_P (TELE, PSTR(D_RSLT_SENSOR));
}

// Handle Temperature MQTT commands
bool TemperatureMqttCommand ()
{
  int  command_code;
  char command [CMDSZ];

  // check MQTT command
  command_code = GetCommandCode (command, sizeof(command), XdrvMailbox.topic, kTemperatureCommands);

  // handle command
  switch (command_code)
  {
    case CMND_TEMPERATURE_TOPIC:  // set mqtt temperature topic 
      TemperatureSetMqttTopic (XdrvMailbox.data);
      break;
    case CMND_TEMPERATURE_KEY:  // set mqtt temperature key 
      TemperatureSetMqttKey (XdrvMailbox.data);
      break;
  }

  // send MQTT status
  TemperatureShowJSON (false);
  
  return true;
}

// MQTT connexion update
void TemperatureCheckMqttConnexion ()
{
  bool   is_connected;
  String str_topic;

  // get temperature MQTT topic
  TemperatureGetMqttTopic (str_topic);

  // if topic defined, check MQTT connexion
  if (str_topic.length () > 0)
  {
    // if connected to MQTT server
    is_connected = MqttIsConnected();
    if (is_connected)
    {
      // if still no subsciption to power topic
      if (temperature_topic_subscribed == false)
      {
        // subscribe to power meter
        MqttSubscribe(str_topic.c_str ());

        // subscription done
        temperature_topic_subscribed = true;

        // log
        AddLog_P2(LOG_LEVEL_INFO, PSTR("MQT: Subscribed to %s"), str_topic.c_str ());
      }
    }

    // else disconnected : topic not subscribed
    else temperature_topic_subscribed = false;
  }
}

// read received MQTT data to retrieve temperature
bool TemperatureMqttData ()
{
  bool    data_handled = false;
  int     idx_value;
  String  str_topic, str_key;
  String  str_mailbox_topic, str_mailbox_data, str_mailbox_value;

  // get topics to compare
  str_mailbox_topic = XdrvMailbox.topic;
  TemperatureGetMqttKey (str_key);
  TemperatureGetMqttTopic (str_topic);

  // get temperature (removing SPACE and QUOTE)
  str_mailbox_data  = XdrvMailbox.data;
  str_mailbox_data.replace (" ", "");
  str_mailbox_data.replace ("\"", "");

  // if topic is the temperature
  if (str_mailbox_topic.compareTo(str_topic) == 0)
  {
    // if a temperature key is defined, find the value in the JSON chain
    if (str_key.length () > 0)
    {
      str_key += ":";
      idx_value = str_mailbox_data.indexOf (str_key);
      if (idx_value >= 0) idx_value = str_mailbox_data.indexOf (':', idx_value + 1);
      if (idx_value >= 0) str_mailbox_value = str_mailbox_data.substring (idx_value + 1);
    }

    // else, no power key provided, data holds the value
    else str_mailbox_value = str_mailbox_data;

    // convert and update instant power
    TemperatureSetValue (str_mailbox_value.toFloat ());

    // data from message has been handled
    data_handled = true;
  }

  return data_handled;
}

/***********************************************\
 *                    Web
\***********************************************/

#ifdef USE_WEBSERVER

// Temperature configuration button
void TemperatureWebButton ()
{
  WSContentSend_P (PSTR ("<p><form action='%s' method='get'><button>%s</button></form></p>"), D_PAGE_TEMPERATURE, D_TEMPERATURE_CONFIGURE);
}

// Pilot Wire web page
void TemperatureWebPage ()
{
  bool   state_pullup;
  char   argument[TEMPERATURE_BUFFER_SIZE];
  String str_topic, str_key, str_pullup;

  // if access not allowed, close
  if (!HttpCheckPriviledgedAccess()) return;

  // page comes from save button on configuration page
  if (WebServer->hasArg("save"))
  {
    // set MQTT topic according to 'ttopic' parameter
    WebGetArg (D_CMND_TEMPERATURE_TOPIC, argument, TEMPERATURE_BUFFER_SIZE);
    TemperatureSetMqttTopic (argument);

    // set JSON key according to 'tkey' parameter
    WebGetArg (D_CMND_TEMPERATURE_KEY, argument, TEMPERATURE_BUFFER_SIZE);
    TemperatureSetMqttKey (argument);
  }

  // beginning of form
  WSContentStart_P (D_TEMPERATURE_CONFIGURE);
  WSContentSendStyle ();
  WSContentSend_P (TEMPERATURE_FORM_START, D_PAGE_TEMPERATURE);

  // remote sensor section  
  // --------------
  WSContentSend_P (TEMPERATURE_FIELDSET_START, D_TEMPERATURE_REMOTE);

  // remote sensor mqtt topic
  TemperatureGetMqttTopic (str_topic);
  WSContentSend_P (PSTR ("<br/>%s<br/>"), D_TEMPERATURE_TOPIC);
  WSContentSend_P (PSTR ("<input name='%s' value='%s'>"), D_CMND_TEMPERATURE_TOPIC, str_topic.c_str ());
  WSContentSend_P (PSTR ("<br/>"));

  // remote sensor json key
  TemperatureGetMqttKey (str_key);
  WSContentSend_P (PSTR ("<br/>%s<br/>"), D_TEMPERATURE_KEY);
  WSContentSend_P (PSTR ("<input name='%s' value='%s'>"), D_CMND_TEMPERATURE_KEY, str_key.c_str ());
  WSContentSend_P (PSTR ("<br/>"));

  WSContentSend_P (TEMPERATURE_FIELDSET_STOP);

  // save button
  WSContentSend_P (PSTR ("<button name='save' type='submit' class='button bgrn'>%s</button>"), D_SAVE);
  WSContentSend_P (TEMPERATURE_FORM_STOP);

  // configuration button
  WSContentSpaceButton(BUTTON_CONFIGURATION);

  // end of page
  WSContentStop();
}

#endif  // USE_WEBSERVER

/***********************************************************\
 *                      Interface
\***********************************************************/

bool Xdrv97 (uint8_t function)
{
  bool result = false;

  // main callback switch
  switch (function)
  { 
   case FUNC_MQTT_INIT:
      TemperatureCheckMqttConnexion ();
      break;
   case FUNC_MQTT_DATA:
      result = TemperatureMqttData ();
      break;
    case FUNC_COMMAND:
      result = TemperatureMqttCommand ();
      break;
    case FUNC_EVERY_SECOND:
      TemperatureCheckMqttConnexion ();
      break;
  }
  
  return result;
}

bool Xsns97 (uint8_t function)
{
  bool result = false;

  // main callback switch
  switch (function)
  { 
    case FUNC_JSON_APPEND:
      TemperatureShowJSON (true);
      break;

#ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_HANDLER:
      WebServer->on ("/" D_PAGE_TEMPERATURE, TemperatureWebPage);
      break;
    case FUNC_WEB_ADD_BUTTON:
      TemperatureWebButton ();
      break;
#endif  // USE_WEBSERVER

  }
  
  return result;
}

#endif // USE_TEMPERATURE
