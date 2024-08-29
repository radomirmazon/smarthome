#include <pcf8574.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "switch-module.h"
#include "sensor-module.h"
#include "mqtt-module.h"
#include "config.h"
#include "logger.h"


Config config;
Logger logger;
WiFiClient espClient;
MqttModule* pMqtt = new MqttModule(&espClient, &config);

SensorModule sensor0(0x20, 0, false, pMqtt, &config, &logger);
SensorModule sensor1(0x21, 1, false, pMqtt, &config, &logger);
SensorModule sensor2(0x22, 2, false, pMqtt, &config, &logger);
SensorModule sensor3(0x23, 3, false, pMqtt, &config, &logger);
SensorModule sensor4(0x24, 4, false, pMqtt, &config, &logger);
SwitchModule switch0(0x26, 0, true, pMqtt, &config, &logger);
SwitchModule switch1(0x27, 1, true, pMqtt, &config, &logger);

void ICACHE_RAM_ATTR isr0() {
  sensor0.isrFlag = true;
}

void ICACHE_RAM_ATTR isr1() {
  sensor1.isrFlag = true;
}

void ICACHE_RAM_ATTR isr2() {
  sensor2.isrFlag = true;
}

void ICACHE_RAM_ATTR isr3() {
  sensor3.isrFlag = true;
}

void ICACHE_RAM_ATTR isr4() {
  sensor4.isrFlag = true;
}



void setup()
{
  //Serial.begin(9600);
  Serial.println("\nHello\n");
  logger.begin();
  delay(200);
  
  logger.on();
  setup_wifi();
  logger.off();

  pMqtt->begin();
  switch0.begin();
  switch1.begin();
  sensor0.begin();
  sensor1.begin();
  sensor2.begin();
  sensor3.begin();
  sensor4.begin();
  //setup interrupts for sensors:
  /**
  attachInterrupt(digitalPinToInterrupt(GPIO), ISR, mode);
  GPIO: wszystkie za wyjątkiem 16
  MODE:
    LOW:	Triggers the interrupt whenever the pin is LOW
    HIGH:	Triggers the interrupt whenever the pin is HIGH
    CHANGE:	Triggers the interrupt whenever the pin changes value, from HIGH to LOW or LOW to HIGH
    FALLING:	Triggers the interrupt when the pin goes from HIGH to LOW
    RISING:	Triggers the interrupt when the pin goes from LOW to HIGH

  **/

  pinMode(13, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(10, INPUT);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(13), isr0, FALLING);
  attachInterrupt(digitalPinToInterrupt(12), isr1, FALLING);
  attachInterrupt(digitalPinToInterrupt(14), isr2, FALLING);
  attachInterrupt(digitalPinToInterrupt(10), isr3, FALLING);
  attachInterrupt(digitalPinToInterrupt(2), isr4, FALLING);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(config.ssid);

  WiFi.begin(config.ssid, config.password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  pMqtt->loop();
  switch0.loop();
  switch1.loop();
  sensor0.loop();
  sensor1.loop();
  sensor2.loop();
  sensor3.loop();
  sensor4.loop();
  logger.loop();
}