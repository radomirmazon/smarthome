#ifndef Sensor_Module2_h
#define Sensor_Module2_h

#include <stdlib.h>
#include "lib/pcf8574.h"
#include "shared/mqtt_module.h"
#include "sensor.h"
#include "config.h"
#include "topic-parser.h"
#include "logger.h"


class SensorModule2 {
  private:
    Logger* pLog;
    uint8_t logicAddress; 
    Config* pConfig;
    PCF8574 * pExpander;
    MqttModule* pMqtt;
    TopicParser* pTopicParser;
    Sensor* sensors[8];
    long time = -1;
    

  public:
    bool isrFlag = false;
    SensorModule2(uint8_t address, uint8_t logicAddress, boolean isRevert, MqttModule* pMqtt, Config* pConfig, Logger* pLog) {
      this->pLog = pLog;
      this->logicAddress = logicAddress;
      this->pConfig = pConfig;
      this->pMqtt = pMqtt;
      pExpander = new PCF8574(address);
    }

    void begin() {
      time = millis();  
      for (int i=0; i<8; i++) {
        pinMode(*pExpander, i, INPUT_PULLUP);
      }
      for (int i=0; i<8; i++) {
        char buffer[5];
        sensors[i] = new Sensor(
          digitalRead(*pExpander, i), 
          itoa(i, buffer, 10),
          pConfig->sensorDeviceName, 
          pConfig->mqtt_topic_template, 
          pMqtt);
        sensors[i]->begin();  
      } 
    }

    void loop() {
      //calculate timeout or interrupt flag...
      long lastLoop = millis() - time;
      bool isTimeout = false;
      if (lastLoop > 100 || isrFlag) {
        time = millis();
        isTimeout = true;
        isrFlag = false;
      }

      //loop
      for (int i=0; i<8; i++) {
        int state = -1;
        if (isTimeout) {
          state = digitalRead(*pExpander, i);
        }
        sensors[i]->loop(state);
      }
    }

    /*
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
    }*/
    
};

#endif