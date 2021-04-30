#include "includes.h"

const char fwVersion[] = "4/28/21_MCP9808HA";

char temperatureString[5];//where the temperature in F/C is stored
boolean getMCP9808();

// C or F in = Message when Button Pressed
//temp is added to end of message

void setup() {
  pinMode(ESPlatchPin, OUTPUT);
  digitalWrite(ESPlatchPin, HIGH);
  pinMode(LEDpin, OUTPUT);
  Serial.begin(115200);
  checkWakeupPins();
  loadConfiguration(filename, config);
  rtcInit(config.timerCountDown, false);
  Serial.println(getBattery(), 2);


  if (pushLogic()) { //decide if push will occur or nt and what message will be
    if (getMCP9808()) { //get temperature
      strcat(pushMessage, ",");//comma separated
      strcat(pushMessage, temperatureString);
    }

    if (wiFiNeeded) {
      if (connectWiFi()) {
        pushOver();
        pushSafer();
        ifttt();
        mqtt();
      }
    }
    udp();
    tcp();
  }
  killPower();
  waitForButton();
  initBluetooth();
}

void loop() {
  if (!OTAsetup)
    serviceBluetooth();
  else
    checkOTA();
}
