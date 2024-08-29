#ifndef Switch_Module_h
#define Switch_Module_h

#include <stdlib.h>
#include <pcf8574.h>
#include "mqtt-module.h"
#include "config.h"
#include "topic-parser.h"
#include "logger.h"

class SwitchModule {
  private:
    uint8_t logicAddress; 
    Config* pConfig;
    PCF8574 * pExpander;
    bool isRevert;
    MqttModule* pMqtt;
    TopicParser* pTopicParser;
    Logger* pLog;

  public:
    SwitchModule(uint8_t address, uint8_t logicAddress, bool isRevert, MqttModule* pMqtt, Config* pConfig, Logger* pLog) {
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
        pinMode(*pExpander, i, OUTPUT);
        setPort(i, LOW);
      }
      pMqtt->registeSubscribers([this](PubSubClient* subProvider) {
        char buff[50];
        for (int i=0; i<8; i++) {
          sprintf(buff, pConfig->mqtt_topic_command_relay, pConfig->mgtt_topic, 8*logicAddress + i);
          subProvider->subscribe(buff);
          Serial.print("Subscribe topic: ");
          Serial.println(buff);
        }
      });
      pMqtt->registeCallback([this](char* topic, uint8_t* message, 
                              unsigned int length, PubSubClient* pMqttClient) 
      {
        int address = pTopicParser->getCommandAddressTopic(topic);
        int pinNumber = decodeAddress(address);
        if (pinNumber == -1) {
          return;
        }

        Serial.print("Wiadomosc odebrana: ");
        Serial.print(topic);
        Serial.print(". Message: ");

        String messageTemp;
        for (int i=0; i<length; i++) {
          Serial.print((char) message[i]);
          messageTemp += (char) message[i];
        }
        Serial.println();
        
        Serial.println("Address: " + String(address));
        Serial.println("Pin: " + String(pinNumber));

        char buff[50];
        sprintf(buff, pConfig->mqtt_topic_state_relay, pConfig->mgtt_topic, address);
        
        Serial.print("Changing output to ");
        if(messageTemp == pConfig->state_on) {
          Serial.println("on");
          setPort(pinNumber, true);
          pMqttClient->publish(buff, pConfig->state_on, true);
        }
        else if(messageTemp == pConfig->state_off) {
          Serial.println("off");
          setPort(pinNumber, false);
          pMqttClient->publish(buff, pConfig->state_off, true);
        }

        pLog->setBlinkOnce();
        
      });
    }

    int decodeAddress(int address) {
      if (address == -1 ) {
        return -1;
      }
      int p = address/8;
      if (logicAddress != p) {
        return -1;
      }

      return address%8;
    }

    void loop() {
      
    }

  private:
    void setPort(int index, bool state) {
      digitalWrite(*pExpander, index, isRevert ^ state );
    }
  
};


#endif
