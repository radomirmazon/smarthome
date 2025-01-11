#ifndef Switch_Module_h
#define Switch_Module_h

#include <stdlib.h>
#include "lib/pcf8574.h"
#include "shared/mqtt_module.h"
#include "config.h"
#include "topic-parser.h"
#include "logger.h"
#include "switch.h"
#include "switchSetter.h"

class SwitchModule :public SwitchSetter {
  private:
    uint8_t logicAddress; 
    Config* pConfig;
    PCF8574 * pExpander;
    bool isRevert;
    MqttModule* pMqtt;
    Logger* pLog;
    Switch* switches[8];


  public:
    SwitchModule(uint8_t address, uint8_t logicAddress, bool isRevert, MqttModule* pMqtt, Config* pConfig, Logger* pLog) {
      this->logicAddress = logicAddress;
      this->pConfig = pConfig;
      this->pMqtt = pMqtt;
      this->isRevert = isRevert;
      this->pLog = pLog;
      pExpander = new PCF8574(address);
    }

    void begin() {
      for (uint8_t i=0; i<8; i++) {
        pinMode(*pExpander, i, OUTPUT);
        setPort(i, LOW);
      }
      char buffer[5];
      for (uint8_t i=0; i<8; i++) {
        switches[i] = new Switch(itoa(decodeAddress(i), buffer, 10), pConfig, pMqtt, this, i);
        switches[i]->begin();
      }
    }

    uint8_t decodeAddress(uint8_t address) {
      return logicAddress*8 + address;
    }

    void loop() {
    }

    void setPort(int index, bool state) {
      digitalWrite(*pExpander, index, isRevert ^ state );
      pLog->setBlinkOnce();
    }
  
};


#endif
