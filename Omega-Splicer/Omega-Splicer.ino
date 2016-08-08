/*
This is the Arduino code for Omega-Splicer
usage :
  m1:int[;m2:int] -> controls motor 1 [and 2]

The module sends every SIGNAL_INTERVAL :
  Approximate battery level in volts eg : "b:3.33" float
  Approximate signal strength eg : "s:-51" int
  Approximate temperature in Celsius by default eg : "t:25.50" float


Sketch uses 28,212 bytes (21%) of program storage space. Maximum is 131,072 bytes.
Global variables use 1,912 bytes of dynamic memory.
*/

#include <RFduinoBLE.h>
#include <stdlib.h>

int led1 = 2;
#define SIGNAL_INTERVAL 150 // in loops

void setup() {
  RFduinoBLE.deviceName = "OmegaSplicer"; 
  Serial.begin(9600);
  analogReference(VBG); // for battery

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName) max = 15
  // 'OmegaSplicer' + 'rgb' = 12 + 3 = 15
  RFduinoBLE.advertisementData = "cmd";
  RFduinoBLE.advertisementInterval = 500; // in ms
  send_temp(CELSIUS);
  send_battery();
  //RFduinoBLE.customUUID = "58409710-D5E2-4A7D-B439-10CF9C59E89F"; // comment for debug
  // start the BLE stack
  RFduinoBLE.begin();
}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onConnect() {
  // Led slightly lights up on connect
  analogWrite(led1, 100);
}

void RFduinoBLE_onDisconnect() {
  // turn led off
  digitalWrite(led1, LOW);
}

void RFduinoBLE_onReceive(char *data, int len) {
  if (len > 3) {
    if (!strncmp(data, "m1:", 3)) {
      Serial.print("Pushing m1 to ");
      Serial.println(data + 3);
      if (char *m2 = strchr(data, ';')) {
        Serial.print("Pushing m2 to ");
        Serial.println(data + 3);
      }
    }
  }
}

// Sends signal strength + temp + battery every SIGNAL_INTERVAL loops
void RFduinoBLE_onRSSI(int rssi){
  static unsigned int total = 0;
  if (!(total % SIGNAL_INTERVAL)) {
    String sig = String("s:");
    sig += rssi;
    Serial.println(sig);
    send_temp(CELSIUS);
    send_battery();
  }
  total++;
}

void send_temp(int scale) {
  float tmp = RFduino_temperature(scale);
  Serial.print("t:");
  Serial.println(tmp);
  RFduinoBLE.send("t:",2);
  RFduinoBLE.sendFloat(tmp);
}

void send_battery() {
  Serial.print("b:");
  RFduinoBLE.send("b:", 2);
  float b = analogRead(1)/1023.0*5.0;
  Serial.println(b);
  RFduinoBLE.sendFloat(b);
}


