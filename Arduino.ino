#include "omega-splicer.h"
void		setup()
{
	detect_plugin();
	pinMode(pin_Paired		, OUTPUT);
	pinMode(pin_DataOk		, OUTPUT);
	pinMode(pin_DataError , OUTPUT);
	Serial.begin(9600);
}
void		loop() {
	signal();
	run();
}