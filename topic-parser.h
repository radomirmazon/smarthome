#ifndef p_topic_h
#define p_topic_h

#include "config.h"

class TopicParser {
  
  Config* pConfig;

  public:
    TopicParser(Config* pConf) {
      pConfig = pConf;
    }

    int getCommandAddressTopic(char* topic) {
      String startWith = String("command/") + String(pConfig->mgtt_topic) 
                            + String("/relay/");
      if(strncmp(topic, startWith.c_str(), startWith.length()) != 0) {
        return -1;
      }

      String address;
      for (int i=0; i<3; i++) {
        char a = topic[startWith.length()+i];
        if (!(a >= '0' && a <= '9')) {
          break;
        }
        address += a;
      }
    
      return atoi(address.c_str());
    }
};

#endif