#include "omega-splicer.h"

void		setup()
{
  
  	pinMode(STBY, OUTPUT);
	pinMode(pin_Paired		, OUTPUT);
	pinMode(pin_DataOk		, OUTPUT);
	pinMode(pin_DataError , OUTPUT);
	signal();
	Serial.begin(9600);
}

void		loop() {
	run();
}

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
}

bool	pair()
{
	/*
	char	buffer[32];

	read(buffer, 32);
	if (!strcmp(buffer, "pair")) {
		Serial.println("//Device succefuly paired");
		return (true);
	}
	//obviously we need to pair a device, but I need to debug this
	return (false);
	*/
	return (true);
	// Auto-Pair
}

void	unpair()
{
	Serial.println("//Device Unpaired");
	delay(100);
}


bool	rcvData(t_buffer *buffer)
{
	t_buffer	tmp;

	tmp.size = read(tmp.data, 128);
	return (addToBuffer(buffer, &tmp));
}

void	respond(t_buffer *buffer)
{
	Serial.write(buffer->data, buffer->size);
}

void	respond(char *data, unsigned int size)
{

	Serial.write(data, size);
}
