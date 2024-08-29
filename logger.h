#ifndef p_logger_h
#define p_logger_h

class Logger {
  public:

  void begin() {
    pinMode(16, OUTPUT);
    time = millis();
  }

  void loop() {
    long lastLoop = millis() - time;
    if (lastLoop > wait) {
      time = millis();
      tick();
    } 
  }

  void setBlinkOnce() {
    ledState = false;
    digitalWrite(16, false);
    state = 5;
    wait = 2;
    loop();
  }

  void error() {
    state = 3;
  }

  void fast() {
    state = 2;
  }

  void slow() {
    state = 1;
  }

  void off() {
    state = 0;
    ledState = false;
    tick();
  }

  void on() {
    state = 4;
    ledState = false;
    tick();
  }

  private:
  long time = -1;
  int wait = 500;
  int counter = 0;
  bool ledState = false;

  //0 - off
  //1 - 500ms blink
  //2 - 100ms blink
  //3 - 2x blink / pause
  //4 - on

  int state = 0;

  void tick() {
    switch(state) {
      case 0:
        digitalWrite(16, false);
      break;
      case 1:
        wait = 500;
        ledState = !ledState;
        digitalWrite(16, ledState);
      break;
      case 2:
        wait = 100;
        ledState = !ledState;
        digitalWrite(16, ledState);
      break;
      case 3:
        counter++;
        wait = 80;
        if (counter > 2) {
          ledState = !ledState;
          digitalWrite(16, ledState);
        }
        if (counter > 6) {
          counter = 0;
          digitalWrite(16, false);
        }
      break;
      case 4:
        digitalWrite(16, true);
      break;
      case 5:
        ledState = !ledState;
        digitalWrite(16, ledState);
        if (!ledState) {
          off();
        }
      break;
      default:
        state = 0;
        off();
    }
    
  }
};

#endif