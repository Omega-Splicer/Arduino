#ifndef		OMEGA_SPLICER_H_
# define	OMEGA_SPLICER_H_

# include "Arduino.h"

typedef struct
{
	unsigned int	size;
	char			data[128];
} t_buffer;

typedef struct s_plugin
{
	int		inPin;
	int 	outPin;
	char	name[32];
	char	parameters[32];
	void	(*control)(struct s_plugin *plugin, t_buffer *param);
	void	(*update)(struct s_plugin *plugin);
} t_plugin;

typedef struct
{
	int major;
	int minor;
} t_version;

typedef struct
{
	char		name[32];
	t_version	version;
	t_plugin	**plugins;
} t_device;

typedef struct
{
	char *cmdName;
	void (*cmd)(t_buffer *param, bool *running, t_device *device);
} t_cmdPtr;

static const int pin_Paired	= 2;
static const int pin_DataOk	= 3;
static const int pin_DataError	= 4;
static const t_version localVersion = {1, 0};

/* .ino */
void	loop();
void	setup();
/* .ino */

void	desactivateLED(int led);
void	activateLED(int led);

void	detect_plugin(t_device *device);
void	signal();
void	run();
bool	rcvData(t_buffer *buffer);
void	respond(char *data);
void	respond(char *data, unsigned int size);

bool	pair();
void	unpair();
unsigned int	read(char *buffer, int size, unsigned int timeout = 0, unsigned int timeRead = 100);


/* parser */

bool			commandEnded(t_buffer *buffer);
bool			handshake(t_device *device, t_buffer *buffer);
void			respondKoFrimWare();
void			respondKoBuffer();
void			respondOk();
void			respondKo();
int				doCmd(t_buffer *buffer, bool *running, t_device *device);

void			disconnect(t_buffer *param, bool *running, t_device *device);
void			firmwareUpdate(t_buffer *param, bool *running, t_device *device);
void			initialize(t_buffer *param, bool *running, t_device *device);
void			control(t_buffer *param, bool *running, t_device *device);
void			update(t_buffer *param, bool *running, t_device *device);

/* buffer.cpp */

bool	getsomething(t_buffer *toSearch, t_buffer *toStore, char token);
void	initializeBuffer(t_buffer *buffer);
bool	addToBuffer(t_buffer *to, t_buffer *from);
bool	addToBuffer(t_buffer *to, t_buffer *from, unsigned int size);
bool	addToBuffer(t_buffer *to, char *from, unsigned int size);
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

# include "plugin.h"

#endif
