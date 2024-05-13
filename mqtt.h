#ifndef mqtt_h
#define mqtt_h

#include "config.h"
#include "includes.h"

#ifndef MQTT_ADDR
#error Missing configuration (MQTT_ADDR)
#endif

#ifndef MQTT_PORT
// default MQTT port can be used if not assigned
#define MQTT_PORT 1883
#endif

#ifndef MQTT_USER
#define MQTT_USER ""
#endif

#ifndef MQTT_PASS
#define MQTT_PASS ""
#endif

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ສ້າງ Function ໃນການປ່ຽນຄ່າ Mac Adress ມາເປັນຂໍ້ມູນຮູບແບບ String
String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], HEX);
    if (i < 5)
      result += ':';
  }
  return result;
}

int const connection_retries = 5;

// ສ້າງຟັງຊັ້ນໃນການ Callback Mqtt ໃຊ້ໄວ້ສໍາລັບສະແດງຂໍ້ຄວາມທີ່ບອດ Subscribe ຊໍ້ມູນມາ
void MQTTcallback(char* topic, byte* payload, unsigned int length) 
{
  // ສະແດງ Topic ທີ່ບອດ Subscribe ມາ
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message: "); 

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i]; 
  }

  // ສະແດງຂໍ້ຄວາມທີ່ບອດ Subscribe ມາຕາມ Topic ຢູ່ເທິງ
  Serial.print(message);
  Serial.println();
  Serial.println("-----------------------");
}

bool connectMqtt(){
  static int retry_count = 0;

  // ທໍາການເລີ່ມຕົ້ນໃຊ້ງານ Mqtt ຕາມ Server ແລະ Port ທີ່ເຮົາກໍານົດໄວ້ດ້ານເທິງ
  mqttClient.setServer(MQTT_ADDR, MQTT_PORT);

  // ທໍາການ Callback ເມື່ອບອດໄດ້ມີການຮັບຂໍ້ມູນ ຫຼື ກໍານົດໃຫ້ບອດເປັນ Payload
  mqttClient.setCallback(MQTTcallback);
 
  // ເມື່ອ Mqtt ຂາດການເຊື່ອມຕໍ່
  while (!mqttClient.connected()) {
    Serial.println("");
    Serial.println("Connecting to MQTT...");

    String clientName;  
    clientName += "board  || MacAddress is ";
    uint8_t mac[6];
    WiFi.macAddress(mac);           // ໃຫ້ Wifi ທີ່ເຊື່ອມຕໍ່ບອກຄ່າ MacAdress ຂອງບອດ
    clientName += macToStr(mac);    // ເອີ້ນໃຊ້ Function macToStr() ໃນການປ່ຽນຄ່າ Mac Adress ມາເປັນຂໍ້ມູນຮູບແບບ String
      
    Serial.print("Mqtt Server: ");
    Serial.println(MQTT_ADDR);
    Serial.print("Clientname: ");
    Serial.println(clientName);
      

    // ຖ້າບອດເຊື່ອມຕໍ່ Mqtt ສໍາເລັດ
    if (mqttClient.connect((char*) clientName.c_str())) {
    //if (client.connect((char*) clientName.c_str()), mqtt_user, mqtt_password)) {
      retry_count = 0;
      Serial.print("Mqtt Status: ");
      Serial.println("connected");
      digitalWrite(LED_BUILTIN, HIGH);    // ໃຫ້ເປີດດອກໄຟທີ່ຢູ່ນໍາບອດ
      return true;
    } 

    else {
      retry_count++;
      if( retry_count > connection_retries )
      {
        Serial.println( "Connection failed, will try again next loop" );
        retry_count = 0;
        return false;
      }
      Serial.println( "Connection failed, trying again..." );
      delay( 5000 );
      return mqttClient.state();
    }
  }
  return true;
}



#endif
