#ifndef Sensor_Module_h
#define Sensor_Module_h

#include <stdlib.h>
#include <pcf8574.h>
#include "mqtt-module.h"
#include "config.h"
#include "topic-parser.h"
#include "logger.h"


class SensorModule {
  private:
    uint8_t logicAddress; 
    Config* pConfig;
    PCF8574 * pExpander;
    boolean isRevert;
    MqttModule* pMqtt;
    TopicParser* pTopicParser;
    Logger* pLog;
    long time = -1;
    bool state = false;
    uint8_t lastState[8];

  public:
    bool isrFlag = false;
    SensorModule(uint8_t address, uint8_t logicAddress, boolean isRevert, MqttModule* pMqtt, Config* pConfig, Logger* pLog) {
      this->logicAddress = logicAddress;
      this->pConfig = pConfig;
      this->pMqtt = pMqtt;
      this->isRevert = isRevert;
      this->pTopicParser = new TopicParser(pConfig);
      this->pLog = pLog;
      pExpander = new PCF8574(address);
    }

    void begin() {
      for (int i=0; i<8; i++) {
        pinMode(*pExpander, i, INPUT_PULLUP);
        lastState[i] = digitalRead(*pExpander, i);
      } 
      time = millis();
    }

    void loop() {
      long lastLoop = millis() - time;
      if (lastLoop > 10) {
        time = millis();
        tick();
      } 

      //int state = digitalRead(*pExpander, 0);
	    //if(state == -1) Serial.println("PCF8574 not detected");
	    //Serial.println(String(state));
	    //delay(100);

    }

    void tick() {
      if (isrFlag) {
        isrFlag = false;
        for (int i=0; i<8; i++) {
          int state = digitalRead(*pExpander, i);
          if (state != lastState[i]) {
              lastState[i] = state;
              Serial.print("Module: ");
              Serial.print(logicAddress);  
              Serial.print(" Pin: ");
              Serial.print(i);
              Serial.print(" state: ");
              Serial.println(String(state));  
              sendState(i, state);
          }
        }
      }
    }
    void sendState(int i, int state) {
      char buff[50];
      sprintf(buff, pConfig->mqtt_topic_state_sensor, pConfig->mgtt_topic, 8*logicAddress + i);
      if (state) {
        Serial.print("Sensor on");
        pMqtt->getClient()->publish(buff, pConfig->sensor_on, true);
      } else {
        Serial.print("Sensor off");
        pMqtt->getClient()->publish(buff, pConfig->sensor_off, true);
      }
      pLog->setBlinkOnce();
    }
};

#endif