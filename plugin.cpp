#include "omega-splicer.h"

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
		device->plugins[0] = initializePlugin("FrontMotor", "DIGIT[3]", 0, 0, NULL, NULL);
		device->plugins[1] = initializePlugin("Rudder", "DIGIT[3]", 0, 0, controlTest, updateTest);
		device->plugins[2] = NULL;
	}
}