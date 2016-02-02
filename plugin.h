#ifndef		PLUGIN_H_
# define	PLUGIN_H_

# include "Arduino.h"
# include "structures.h"
# include "comm.h"

/* define here the prototype for all plugin */

void	updateTest(struct s_plugin *plugin);
void	motorControl(struct s_plugin *plugin, t_buffer *param);

#endif