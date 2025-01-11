#ifndef Switch__h
#define Switch__h

#include <stdlib.h>
#include "lib/pcf8574.h"
#include "shared/base_device.h"
#include "config.h"
#include "shared/mqtt_module.h"
#include "switchSetter.h"

class Switch :public BaseDevice {
  private:
  Config* pConfig;
  SwitchSetter* pSwitchSetter;
  int pinNumber;

  public:
  Switch(const char * channel, Config* pConfig, MqttModule* pMqtt, SwitchSetter* pSwitchSetter, int pinNumber) 
    : BaseDevice(pMqtt, channel, pConfig->switchDeviceName, pConfig->mqtt_topic_template, true, false) {
      this->pConfig = pConfig;
      this->pSwitchSetter = pSwitchSetter;
      this->pinNumber = pinNumber;
  }

  void begin() {
    BaseDevice::begin();
  }
  
  bool onState(String state) {
    Serial.print("Changing output to ");
    if(state == pConfig->state_on) {
      Serial.println("on");
      pSwitchSetter->setPort(pinNumber, true);
    }
    else if(state == pConfig->state_off) {
      Serial.println("off");
      pSwitchSetter->setPort(pinNumber, false);
    }
    return 1;
  }

  void onConfig(String state) {
    //nop
  }

};


#endif
