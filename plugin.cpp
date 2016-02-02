#include "omega-splicer.h"

t_plugin		*initializePlugin(char *name, char * parameters, int inPinDir, int outPinDir, int speed,void (*control)(struct s_plugin *plugin, t_buffer *param), void (*update)(struct s_plugin *plugin))
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
		plugin->speed = speed;
		plugin->control = control;
		plugin->update = update;

		pinMode(inPinDir, OUTPUT);
		pinMode(outPinDir, OUTPUT);
		pinMode(speed, OUTPUT);
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
		device->plugins[0] = initializePlugin("Left", "DIGIT[3]", 9, 8, 3, motorControl, updateTest);
    	device->plugins[1] = initializePlugin("Right", "DIGIT[3]", 11, 12, 5, motorControl, updateTest);
		device->plugins[2] = NULL;
	}
}
