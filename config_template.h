#ifndef MqttModule_h
#define MqttModule_h

class Config {
  public:

    // WiFi settings
    const char *ssid = "SSID";             // Replace with your WiFi name
    const char *password = "*********";   // Replace with your WiFi password

    // MQTT Broker settings
    const char *mqtt_broker = "192.168.???.???";

    const char *mqtt_topic_state_relay = "state/%s/relay/%d";  // MQTT topic
    const char *mqtt_topic_command_relay = "command/%s/relay/%d/command";  // MQTT topic
    
    const char *mqtt_topic_state_sensor = "state/%s/sensor/%d";  // MQTT topic
    
    const char *mgtt_topic = "module_test";

    const char *mqtt_username = "mqtt_user";  // MQTT username for authentication
    const char *mqtt_password = "*******";  // MQTT password for authentication
    const int mqtt_port = 1883;  // MQTT port (TCP)

    const char *state_on = "1";  // state ON
    const char *state_off = "0";  // state OFF

    const char *sensor_off = "0";
    const char *sensor_on = "1";
    const char *sensor_double = "2";
    const char *sensor_long = "3";

};

#endif