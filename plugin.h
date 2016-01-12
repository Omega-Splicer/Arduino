#ifndef		PLUGIN_H_
# define	PLUGIN_H_

# include "omega-splicer.h"

/* define here the prototype for all plugin */

void	updateTest(struct s_plugin *plugin);
void	controlTest(struct s_plugin *plugin, t_buffer *param);

#endif