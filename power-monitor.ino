#include "config.h"           // Program configuration
#include "includes.h"         // Main includes

const char* mqttTopic = "Enter your mqtt toppic";

SoftwareSerial pzemSWSerial( D2, D5 ); // RX, TX ports for pzem

int const PZEM_COUNT = 3;       // Number of PZEM units
int const PZEM_ADDR = 0x45;     // Starting PZEM address
PZEM004Tv30 pzems[PZEM_COUNT];  // Initialize the PZEM


int const delay_timer = 1000 * 60; // 1 minute between polling

void readData(int count){
  for( int i = 0; i < count; i++ )
  {
    Serial.print( "PZEM: " );
    Serial.println( i );

    // Read the data from the sensor
    float voltage   = pzems[i].voltage();
    float current   = pzems[i].current();
    float power     = pzems[i].power();
    float energy    = pzems[i].energy();
    float frequency = pzems[i].frequency();
    float pf        = pzems[i].pf();
  
    // Check if the data is valid
    if( isnan( voltage ) )
      voltage = -1;
      
    if( isnan( current ) )
      current = -1;
      
    if( isnan( power ) )
      power = -1;
      
    if( isnan( energy ) )
      energy = -1;
      
    if( isnan( frequency ) )
      frequency = -1;
      
    if( isnan( pf ) )
      pf = -1;

    //Debug data
    Serial.print("voltage: ");
    Serial.println(voltage);
    Serial.print("current: ");
    Serial.println(current);
    Serial.print("power: ");
    Serial.println(power);
    Serial.print("energy: ");
    Serial.println(energy);
    Serial.print("frequency: ");
    Serial.println(frequency);
    Serial.print("pf: ");
    Serial.println(pf);
    Serial.println();

    String send_voltage   = String(pzems[i].voltage()).c_str();
    String send_current   = String(pzems[i].current()).c_str();
    String send_power     = String(pzems[i].power()).c_str();
    String send_energy    = String(pzems[i].energy()).c_str();
    String send_frequency = String(pzems[i].frequency()).c_str();
    String send_pf        = String(pzems[i].pf()).c_str();
    String num = String( i + 1 ).c_str();

    String pub_data = "{\"PZEM\":" + num + 
                        ",\"voltage\":" + send_voltage  + 
                        ",\"current\":" + send_current + 
                        ",\"current\":" + send_current + 
                        ",\"current\":" + send_current + 
                        ",\"current\":" + send_current +"}";
    Serial.print("Data of PZEM ");
    Serial.print(i+1);
    Serial.print("Has Sent to MQTT Broker");
    Serial.println();
  }
}

// Setup
void setup()
{
  Serial.begin( 115200 );
  while( !Serial ); // wait for serial port to connect
  
  Serial.println( "MBL AC Power meter monitor" );

  //Setup Wifi
  WiFiManager wm;

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("MBL AC Power meter monitor","password"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
  }

  DateTime.setServer( "ntp.laodc.com" );
  DateTime.setTimeZone( "UTC" );
  DateTime.begin();

  if( !DateTime.isTimeValid() )
    Serial.println( "Failed to get time from server.");
  else
    Serial.printf( "Date Now is %s\n", DateTime.toISOString().c_str() );

  // Connect to MySQL Database Server
  connectMqtt();

  // Connect to the PZEM units
  for( int i = 0; i < PZEM_COUNT; i++ )
  {
    uint8_t addr = PZEM_ADDR + i;
    Serial.printf( "Connecting to PZEM (0x%x)\n", addr );
    
    pzems[i] = PZEM004Tv30( pzemSWSerial, addr );

    Serial.printf( "Connected PZEM ID: (0x%x)\n", pzems[i].readAddress() );
  }
  
  Serial.println();
}

StackArray<pzemStruct*> pzemData;

void loop()
{
  // ຖ້າ Mqtt ເຊື່ອມຕໍ່ບໍ່ສໍາເລັດ ໃຫ້ທໍາການເຊື່ອມຕໍ່ໃໝ່
  if(!mqttClient.connected())
  {
    connectMqtt();
  }
  readData(PZEM_COUNT);
  Serial.println( "Waiting for next polling..." );
  delay( delay_timer );
}