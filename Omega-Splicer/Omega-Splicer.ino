/*
The sketch receives a Bluetooth Low Energy 4 connection,
then uses PWM on the RGB shield to approximate the color
selected.

This sketch is supposed to be used with the rfduinoColorWheel
iPhone application.
*/

#include <RFduinoBLE.h>
#include <stdlib.h>

int led1 = 2;
int led2 = 3;
int led3 = 4;

#define SIGNAL_INTERVAL 150 // in loops
char ret[10];

void setup() {
  // setup the leds for output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);  
  pinMode(led3, OUTPUT);
  Serial.begin(9600);
  analogReference(VBG);
  RFduinoBLE.deviceName = "OmegaSplicer"; 

  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName) max = 15
  // 'OmegaSplicer' + 'rgb' = 12 + 3 = 15
  RFduinoBLE.advertisementData = "cmd";
send_temp(CELSIUS);
send_battery();
  RFduinoBLE.advertisementInterval = 500; // in ms
  //RFduinoBLE.customUUID = "58409710-D5E2-4A7D-B439-10CF9C59E89F";
  // start the BLE stack
  Serial.println("Starting Omega-Splicer.");
  RFduinoBLE.begin();
}

void loop() {
  // switch to lower power mode
  RFduino_ULPDelay(INFINITE);
}

void RFduinoBLE_onConnect() {
  // the default starting color on the iPhone is white
  analogWrite(led1, 15);
  analogWrite(led2, 125);
  analogWrite(led3, 200);
}

void RFduinoBLE_onDisconnect() {
  // turn all leds off on disconnect and stop pwm
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
}

// Send signal strength
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

/*
char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}*/

/* get the RGB values
    uint8_t r = data[0];
    uint8_t g = data[1];

    // set PWM for each led
    analogWrite(led1, r);
    analogWrite(led2, g); */
