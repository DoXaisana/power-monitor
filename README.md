# power-monitor
Collection of scripts for monitor power usage


config.h
```C++
#ifndef config_h
#define config_h

// Configuration to be used by pzem monitoring
//MQTT Topic
const char* mqttTopic = "Enter your MQTT Topic";
// MQTT Server
#define MQTT_ADDR "mqtt_url"
// MQTT port
#define MQTT_PORT 1883
// MQTT username
#define MQTT_USER ""
// MQTT password
#define MQTT_PASS ""

#endif
```