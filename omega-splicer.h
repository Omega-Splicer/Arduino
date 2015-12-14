#ifndef		OMEGA_SPLICER_H_
# define	OMEGA_SPLICER_H_

# include "Arduino.h"

typedef struct
{
	//in pin
	//out pin
	char name[32];
	char parameters[32];
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
	unsigned int	size;
	char			data[128];
} t_buffer;

typedef struct
{
	char *cmdName;
	void (*cmd)(t_buffer *param, bool *running, t_device *device);
} t_cmdPtr;

static const int pin_Paired	= 2;
static const int pin_DataOk	= 3;
static const int pin_DataError	= 4;
static const t_version localVersion = {1, 0};

void	loop();
void	setup();

void	desactivateLED(int led);
void	activateLED(int led);

void	detect_plugin(t_device *device);
void	signal();
void	run();
bool	rcvData(t_buffer *buffer);
void	respond(char *data);
void	respond(char *data, unsigned int size);

bool			pair();
void			unpair();
unsigned int	read(char *buffer, int size, unsigned int timeout = 0, unsigned int timeRead = 100);


void			initializeBuffer(t_buffer *buffer);
bool			addToBuffer(t_buffer *to, t_buffer *from);
bool			addToBuffer(t_buffer *to, int number, unsigned int size);
bool			addToBuffer(t_buffer *to, char *from);
bool			addToBuffer(t_buffer *to, t_buffer *from, unsigned int size);
void			deleteFromBuffer(t_buffer *buffer, unsigned int size);
bool			commandEnded(t_buffer *buffer);
bool			handshake(t_device *device, t_buffer *buffer);
void			respondKoFrimWare();
void			respondKoBuffer();
void			respondOk();
int				doCmd(t_buffer *buffer, bool *running, t_device *device);

void			disconnect(t_buffer *param, bool *running, t_device *device);
void			firmwareUpdate(t_buffer *param, bool *running, t_device *device);
void			initialize(t_buffer *param, bool *running, t_device *device);
void			control(t_buffer *param, bool *running, t_device *device);

static const t_cmdPtr cmdPtr[] = {
	{"Disconnect", &disconnect},
	{"FirmwareUpdate", &firmwareUpdate},
	{"Initialize", &initialize},
	{"Control", &control},
	{NULL, NULL}
};

#endif
