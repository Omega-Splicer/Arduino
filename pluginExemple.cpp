#include "plugin.h"

void	updateTest(struct s_plugin *plugin)
{
	Serial.println("//plugin update Ok");
	respondOk();
}

void	controlTest(struct s_plugin *plugin, t_buffer *param)
{
	Serial.println("//plugin control Ok");
	respondOk();
}