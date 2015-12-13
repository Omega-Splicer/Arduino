#include "omega-splicer.h"

t_plugin		*initializePlugin(char *name, char * parameters)
{
	t_plugin	*plugin;

	plugin = (t_plugin*)malloc(sizeof(t_plugin));
	if (plugin) {
		strcpy(plugin->name, name);
		plugin->name[strlen(name)] = 0;
		strcpy(plugin->parameters, parameters);
		plugin->parameters[strlen(parameters)] = 0;
	}
	return (plugin);
}

void	detect_plugin(t_device *device)
{
	int i = 0;

	while (device->plugins && device->plugins[i]) {
		free(device->plugins[i]);
		i++;
	}
	if (device->plugins)
		free(device->plugins);
	//debug purpose, obviously (leak memory ifm I don't know why, the first attempt fail, and second pass, here for debug)
	device->plugins = (t_plugin**)malloc(3 * sizeof(t_plugin *));
	if (device->plugins) {
		device->plugins[0] = initializePlugin("FrontMotor", "DIGIT[3]");
		device->plugins[1] = initializePlugin("Rudder", "DIGIT[3]");
		device->plugins[2] = NULL;
	}
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
	Serial.println("//Arduino succefully started");
}

void			run()
{
	bool		running = true;
	t_buffer	buffer;
	t_device	device;
	int			cmdSize;
	int			i;

	device.plugins = NULL;
	if (pair())
	{
		initializeBuffer(&buffer);
		while (!commandEnded(&buffer)) {
			if (!rcvData(&buffer))
			{
				/* Error no handshake and buffer full */
				respondKoFrimWare();
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
						/* Error : Buffer full, probably error with communication */
						respondKoFrimWare();
						unpair();
						return ;
					}
				}
				cmdSize = doCmd(&buffer, &running, &device);
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
	i = 0;
	while (device.plugins && device.plugins[i]) {
		free(device.plugins[i]);
		i++;
	}
	if (device.plugins)
		free(device.plugins);
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
	//Obviously, with the communication module, this need to be changed
	Serial.println(data);
	delay(100);
}

void	respond(char *data) //careful, only debug
{
	//Obviously, with the communication module, this need to be changed
	Serial.println(data);
	delay(100);
}