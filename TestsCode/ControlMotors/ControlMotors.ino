
int motor1Pin1 = 4;    // pin 2 on L293D
int motor1Pin2 = 3;    // pin 7 on L293D
int enablePin = 9;    // pin 1 on L293D

/* Bluetooth */

char junk;
String inputString="";


void setup() {

  // set all the other pins you're using as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // set enablePin high so that motor can turn on:
  digitalWrite(enablePin,HIGH);

  Serial.begin(9600);            // set the baud rate to 9600, same should be of your Serial Monitor
}

void loop() {
  if(Serial.available()){
    while(Serial.available())
      {
        char inChar = (char)Serial.read(); //read the input
        inputString += inChar;        //make a string of the characters coming on serial
      }
      Serial.println(inputString);
      while (Serial.available() > 0)  
      {
        junk = Serial.read() ;
      }      // clear the serial buffer
    }
    if(inputString == "a")
    {         //in case of 'a' turn the LED on
    // if the switch is high, motor will turn on one direction:
      digitalWrite(motor1Pin1, LOW);   // set pin 2 on L293D low
      digitalWrite(motor1Pin2, HIGH);  // set pin 7 on L293D high
      digitalWrite(13, HIGH);
    }
    else if (inputString == "b")
    {   //incase of 'b' turn the LED off
      digitalWrite(motor1Pin1, HIGH);   // set pin 2 on L293D low
      digitalWrite(motor1Pin2, LOW);  // set pin 7 on L293D high
      digitalWrite(13, LOW);
    }
    else if (inputString == "c")
    {   //incase of 'c' turn the LED off
      digitalWrite(motor1Pin1, LOW);   // set pin 2 on L293D low
      digitalWrite(motor1Pin2, LOW);  // set pin 7 on L293D high
      digitalWrite(13, LOW);
    }
    inputString = "";
}
