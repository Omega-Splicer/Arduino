#include "omega-splicer.h"

t_plugin		*initializePlugin(char *name, char * parameters, int inPinDir, int outPinDir, int speedControl,void (*control)(struct s_plugin *plugin, t_buffer *param), void (*update)(struct s_plugin *plugin))
{
	t_plugin	*plugin;

	plugin = (t_plugin*)malloc(sizeof(t_plugin));
	if (plugin) {
		strcpy(plugin->name, name);
		plugin->name[strlen(name)] = 0;
		strcpy(plugin->parameters, parameters);
		plugin->parameters[strlen(parameters)] = 0;
		plugin->inPin = inPinDir;
		plugin->outPin = outPinDir;
		plugin->speedControl = speedControl;
		plugin->control = control;
		plugin->update = update;

		pinMode(inPinDir, OUTPUT);
		pinMode(outPinDir, OUTPUT);
		pinMode(speedControl, OUTPUT);
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
	//debug purpose (leak memory if the first attempt fail, and second pass)
	device->plugins = (t_plugin**)malloc(3 * sizeof(t_plugin *));
	if (device->plugins) {
		device->plugins[0] = initializePlugin("Left", "DIGIT[3]", 3, 9, 8, controlTest, updateTest);
    	device->plugins[1] = initializePlugin("Right", "DIGIT[3]", 5, 11, 12, controlTest, updateTest);
    	device->plugins[2] = initializePlugin("Rudder", "DIGIT[3]", 3, 4, 0, controlTest, updateTest);
		device->plugins[3] = NULL;
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
	/*
	Some debug with LEDs
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
	*/
	Serial.println("// (Signal) Arduino succesfully started");
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
