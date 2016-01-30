#include "plugin.h"

void	updateTest(struct s_plugin *plugin)
{
	Serial.println("//plugin update Ok");
	respondOk();
}

void	controlTest(struct s_plugin *plugin, t_buffer *param)
{
	Serial.println(param->data);
 	Serial.println("--");
 	Serial.println(plugin->inPin);
 	Serial.println("\n");
 	digitalWrite(plugin->inPin,atoi(param->data));
	Serial.println("//plugin control Ok");
	respondOk();
}

void motorControl(struct s_plugin *plugin, t_buffer *param){
	
 	digitalWrite(STBY, HIGH); //disable standby
 	Serial.println("//Motor control Ok !!");
	Serial.println(param->data);
	Serial.println("--");
 	Serial.println(plugin->inPin);
 	Serial.println("\n");
 	// digitalWrite(plugin->inPin,atoi(param->data));
 	// analogWrite(plugin->speedControl, atoi(param->data))

	// move(255, 1); //motor 1, full speed, left
	// move(255, 1); //motor 2, full speed, left

	// delay(1000); //go for 1 second
	// stop(); //stop
 // 	delay(250); //hold for 250ms until move again

 // 	move(128, 0); //motor 1, half speed, right
 // 	move(128, 0); //motor 2, half speed, right

 // 	delay(1000);
 // 	stop();
 // 	delay(250);
	respondOk();
}

void move(int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise

  // digitalWrite(STBY, HIGH); //disable standby

  // boolean inPin1 = LOW;
  // boolean inPin2 = HIGH;

  // if(direction == 1){
  //   inPin1 = HIGH;
  //   inPin2 = LOW;
  // }
  //   digitalWrite(AIN1, inPin1);
  //   digitalWrite(AIN2, inPin2);
  //   analogWrite(PWMA, speed);
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}
