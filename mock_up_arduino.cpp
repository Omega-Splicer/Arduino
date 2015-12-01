#include "Arduino.h"
#include "omega-splicer.h"

/*
int	main(int ac, char **av) {
	setup();
	while (true) {
		loop();
	}
	return (0);
}

void	pinMode(int pin, int mode) {
	(void)pin;
	(void)mode;
}

void	digitalWrite(int pin, int mode) {
	(void)pin;
	(void)mode;
}

void	SerialClass::begin(int hz) {
	(void)hz;
}

void	SerialClass::println(std::string str) {
	std::cout << str << std::endl;
}

void	SerialClass::print(std::string str) {
	std::cout << str;
}

void	SerialClass::println(int str) {
	std::cout << str << std::endl;
}

void	SerialClass::print(int str) {
	std::cout << str;
}

void	delay(unsigned int utime) {
	(void)utime;
}
*/

unsigned int	read(char *buffer, int size, unsigned int timeout, unsigned int timeRead)
{
	unsigned int i = 0;
	unsigned int timeSpend = 0;

	while (Serial.available() <= 0 && (timeout == 0 || timeSpend < timeout))
	{
		timeSpend += timeRead;
		delay(timeRead);
	}
	while (Serial.available() > 0)
	{
		buffer[i] = Serial.read();
		i++;
		if (i >= size + 1)
		{
			buffer[i] = 0;
			return (i);
		}
	}
	buffer[i] = 0;
	return (i);
	/*
	unsigned int i = 0;
	std::string str;

	std::cin >> str;
	if (size > str.size()) {
		size = str.size();
	}
	strncpy(buffer, str.c_str(), size);
	buffer[size] = 0;
	return (size);
	*/
}

bool	pair()
{
	char	buffer[32];

	read(buffer, 32);
	if (!strcmp(buffer, "pair")) {
		Serial.println("Device succefuly paired");
		//activateLED(pin_Paired);
		return (true);
	}
	//obviously we need to pair a device, but I need to debug this
	return (false);
}

void	unpair()
{
	Serial.println("Error, unpair with the device");
	delay(100);
}