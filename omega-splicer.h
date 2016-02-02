#ifndef		OMEGA_SPLICER_H_
# define	OMEGA_SPLICER_H_

# include "Arduino.h"
# include "structures.h"
# include "comm.h"
# include "plugin.h"

static const t_version localVersion = {1, 0};

/* Motor control */
static const int STBY = 10; //standby

/* .ino */
void			loop();
void			setup();

/* plugin.cpp */

void	detect_plugin(t_device *device);

/* parser */

void	desactivateLED(int led);
void	activateLED(int led);
void	signal();
int		run();

void			disconnect(t_buffer *param, bool *running, t_device *device);
void			firmwareUpdate(t_buffer *param, bool *running, t_device *device);
void			initialize(t_buffer *param, bool *running, t_device *device);
void			control(t_buffer *param, bool *running, t_device *device);
void			update(t_buffer *param, bool *running, t_device *device);
t_plugin		*initializePlugin(char *name, char * parameters, int inPin, int outPin, int speedControl, void (*control)(struct s_plugin *plugin, t_buffer *param), void (*update)(struct s_plugin *plugin));


/* buffer.cpp */

bool	getsomething(t_buffer *toSearch, t_buffer *toStore, char token);
void	initializeBuffer(t_buffer *buffer);
bool	addToBuffer(t_buffer *to, t_buffer *from);
bool	addToBuffer(t_buffer *to, t_buffer *from, unsigned int size);
bool	addToBuffer(t_buffer *to, const char *from, unsigned int size);
bool	addToBuffer(t_buffer *to, int number);
void	deleteFromBuffer(t_buffer *buffer, unsigned int size);
bool	byteBetween(char b, char firstToken, char lastToken);
bool	onlyDigit(t_buffer *buffer);
bool	onlyAlpha(t_buffer *buffer);
bool	onlyAlphaNum(t_buffer *buffer);

static const t_cmdPtr cmdPtr[] = {
	{"Disconnect", &disconnect},
	{"FirmwareUpdate", &firmwareUpdate},
	{"Initialize", &initialize},
	{"Control", &control},
	{"Update", &update},
	{NULL, NULL}
};

#endif
// getsomething
// token : ,