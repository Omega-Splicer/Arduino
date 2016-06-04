#include "omega-splicer.h"

void		setup()
{
  pinMode(STBY, OUTPUT);
	//Serial.begin(250000);
	Serial.begin(9600);
}

void		loop() {
	run();
}
