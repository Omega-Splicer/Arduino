/*
This is the Arduino code for Omega-Splicer
usage :
  m1:int[;m2:int] -> controls motor 1 [and 2] 0 to 255

The module sends every SIGNAL_INTERVAL (defaults to (~ 8 seconds) :
  Approximate battery level in percentage from 3 to 4.2V eg : "b:16" for 3.2V. In int format
  Approximate signal strength eg : "s:-51" Value in int format
  Approximate temperature in Celsius by default eg : "t:25" Value in int format


Sketch uses 28,212 bytes (21%) of program storage space. Maximum is 131,072 bytes.
Global variables use 1,912 bytes of dynamic memory.
*/

#include <RFduinoBLE.h>
#include <stdlib.h>
#include <string>

int led1 = 2;
int led2 = 3;
int led3 = 4;
#define SIGNAL_INTERVAL 150 // in loops, 8 seconds

void setup() {
  RFduinoBLE.deviceName = "OmegaSplicer"; 
  Serial.begin(9600);
  analogReference(VBG); // for battery

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName) max = 15
  // 'OmegaSplicer' + 'cmd' = 12 + 3 = 15
  RFduinoBLE.advertisementData = "cmd";
  RFduinoBLE.advertisementInterval = 500; // in ms
  send_temp(CELSIUS);
  send_battery();
  RFduinoBLE.customUUID = "58409710-D5E2-4A7D-B439-10CF9C59E89F"; // comment for debug
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
  char *cmd;
  Serial.print("->");
  Serial.print(data);
  Serial.println("<-");
  if (len > 3) {
    cmd = strtok(data, "/");
    while (cmd != NULL) {
      if (!strncmp(cmd, "m1:", 3)) { // refaire un parser
       Serial.print("Pushing m1 to ");
       Serial.println(cmd + 3);
       analogWrite(led2, atoi(cmd + 3));
      } else if (!strncmp(cmd, "m2:", 3)) {
       Serial.print("Pushing m2 to ");
       Serial.println(cmd + 3);
       analogWrite(led3, atoi(cmd + 3));
      }
      cmd = strtok(NULL, "/");
    }
  }
  memset(data, 0, len);
}

// Sends signal strength + temp + battery every SIGNAL_INTERVAL loops
void RFduinoBLE_onRSSI(int rssi){
  static unsigned int total = 0;
  if (!(total % SIGNAL_INTERVAL)) {
    display_sig(rssi);
    send_temp(CELSIUS);
    send_battery();
    Serial.println(" ");
  }
  total++;
}

void display_sig(int rssi) {
  String sig = String("s:");
  sig += rssi;
  Serial.println(sig);
}

void send_temp(int scale) {
  char buffer[6];
  String ret = String("t:");

  ret += String((int)(RFduino_temperature(scale)));
  ret.toCharArray(buffer, 6);
  
  Serial.println(buffer);
  RFduinoBLE.send(buffer, 6);
//  RFduinoBLE.send(floatToString(buffer, tmp));
}

void send_battery() {
  char buffer[6];
  String ret = String("b:");
  float minimum = 3.0;
  float maximum = 4.2;
  float exact_level = analogRead(1)/1023.0*5.0;
  
  int bat = ((int) (exact_level - minimum) / (maximum - minimum) * 100);
  if (bat > 100)
    bat = 100;
  else if (bat < 0)
    bat = 0;
  ret += String(bat);
  ret.toCharArray(buffer, 6);
  
  Serial.println(buffer);
  RFduinoBLE.send(buffer, 6);
}

