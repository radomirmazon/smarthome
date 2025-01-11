# My DIY modumes for Home Assistamt. 
## Features

Base on ESP8266 and MQTT communication
- 40 digital input
- 16 digital output
- 1 LED status indicator 
- And thats all...

## HA automation cofig for digital input setting up.

After restart modules wait for HA booting up, and send request on MQTT for configuration (few times). It sends message on topic getConfig/input/{number} and HA should responds on topic config/input/{number} with payload bell or switch.

### HA automation:

```sh
- id: '1736203646894'
  alias: Module A input config.
  description: ''
  triggers:
  - trigger: homeassistant
    event: start
  - trigger: mqtt
    topic: getConfig/input/+
  conditions: []
  actions:
  - action: mqtt.publish
    metadata: {}
    data:
      evaluate_payload: true
      qos: 0
      retain: false
      topic: config/input/{{ trigger.topic.split('/')[-1] }}
      payload: >
        {% if trigger.topic.split('/')[-1] | int in [1, 3, 4, 6] %}
          bell
        {% else %}
          switch
        {% endif %}
  mode: single
```
