#include "plugin.h"

void	updateTest(struct s_plugin *plugin)
{
	Serial.println("//plugin update Ok");
	respondOk();
}

void	motorControl(struct s_plugin *plugin, t_buffer *param)
{
	int	speed = atoi(param->data);
	// Serial.print("Motor : \ndata:");
	// Serial.print(param->data);
	// Serial.print("; In pin :");
	// Serial.print(plugin->inPin);
	// Serial.print("; Out pin :");
	// Serial.print(plugin->outPin);
	// Serial.print("; speed control pin :");
	// Serial.print(plugin->speed);

	if (speed == 0) {
 		digitalWrite(STBY, LOW); //enable standby
 	} else if (speed < 0) {
		digitalWrite(STBY, HIGH); //disable standby
		digitalWrite(plugin->inPin, LOW);
		digitalWrite(plugin->outPin, HIGH);
		analogWrite(plugin->speed, abs(atoi(param->data)));
	} else { // speed > 0
 		digitalWrite(STBY, HIGH); //disable standby
 		digitalWrite(plugin->inPin, HIGH);
 		digitalWrite(plugin->outPin, LOW);
 		analogWrite(plugin->speed, abs(atoi(param->data)));
 	}
 	respondOk();
}
