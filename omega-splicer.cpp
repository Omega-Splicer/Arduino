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

void	initializePlugin(t_plugin *plugin, char *name, char * parameters)
{
	strcpy(plugin->name, name);
	plugin->name[strlen(name)] = 0;
	strcpy(plugin->parameters, parameters);
	plugin->parameters[strlen(parameters)] = 0;
}

void	detect_plugin()
{
	//debug purpose, obviously
	initializePlugin(&plugins[0], "FrontMotor", "DIGIT[3]");
	initializePlugin(&plugins[1], "Rudder", "DIGIT[3]");
}

void	activateLED(int led)
{
	digitalWrite(led, HIGH);
}

void	desactivateLED(int led)
{
	digitalWrite(led, LOW);
}

void	signal()
{
	delay(1000);
	desactivateLED(pin_Paired);
	desactivateLED(pin_DataOk);
	desactivateLED(pin_DataError);
	delay(1000);
	activateLED(pin_Paired);
	activateLED(pin_DataOk);
	activateLED(pin_DataError);
	delay(1000);
	desactivateLED(pin_Paired);
	desactivateLED(pin_DataOk);
	desactivateLED(pin_DataError);
	Serial.println("Arduino succefully started");
}

void			run()
{
	bool		running = true;
	t_buffer	buffer;
	t_device	device;
	int			cmdSize;

	if (pair())
	{
		initializeBuffer(&buffer);
		while (!commandEnded(&buffer)) {
			if (!rcvData(&buffer))
			{
				/* error */
				Serial.println("Error no handshake and buffer full");
				respondKoFrimWare(); // add frimeware version
				unpair();
				return ;
			}
		}
		delay(100);
		if (handshake(&device, &buffer))
		{
			initializeBuffer(&buffer);
			respond("OK;");
			while (running)
			{
				while (!commandEnded(&buffer)) {
					if (!rcvData(&buffer))
					{
						/* error */
						Serial.println("Error : Buffer full, probably error with communication");
						respondKoFrimWare(); // add frimeware version
						unpair();
						return ;
					}
				}
				/*
					traitement de la commande
				*/
				cmdSize = doCmd(/*&running, &device, */&buffer);
				deleteFromBuffer(&buffer, cmdSize + 1);
			}
			unpair();
		}
		else
		{
			respondKoFrimWare();
			unpair();
			return ;
		}
	}
}

bool	rcvData(t_buffer *buffer)
{
	t_buffer	tmp;

	tmp.size = read(tmp.data, 128);
	return (addToBuffer(buffer, &tmp));
	//Simulation of reciving data from bluetooth, obliously this is a debug feature
}

void	respond(char *data, unsigned int size)
{
	//Obviously, with the communication module, this gonna change
	Serial.print(" -> ");
	Serial.println(data);
	delay(100);
}

void	respond(char *data) //careful, only debug
{
	//Obviously, with the communication module, this gonna change
	Serial.print(" -> ");
	Serial.println(data);
	delay(100);
}