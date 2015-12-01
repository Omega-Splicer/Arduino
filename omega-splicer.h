#ifndef		OMEGA_SPLICER_H_
# define	OMEGA_SPLICER_H_

# include "Arduino.h"

typedef struct
{
	//in pin
	//out pin
	char name[32];
	char parameters[64];
} t_plugin;

typedef struct
{
	int major;
	int minor;
} t_version;

typedef struct
{
	char		name[32];
	t_version version;
} t_device;

typedef struct
{
	unsigned int	size;
	char			data[128];
} t_buffer;

static const int pin_Paired	= 2;
static const int pin_DataOk	= 3;
static const int pin_DataError	= 4;
static const t_version localVersion = {1, 0};

static t_plugin plugins[4];
static const int nbPlugins = 2;

/* .ino */
void	loop();
void	setup();
/* .ino */

void	desactivateLED(int led);
void	activateLED(int led);

void	detect_plugin();
void	signal();
void	run();
bool	rcvData(t_buffer *buffer);
void	respond(char *data);
void	respond(char *data, unsigned int size);

bool	pair();
void	unpair();

/* parser */

void			initializeBuffer(t_buffer *buffer);
bool			addToBuffer(t_buffer *to, t_buffer *from);
bool			addToBuffer(t_buffer *to, int number, unsigned int size);
bool			addToBuffer(t_buffer *to, char *from);
bool			addToBuffer(t_buffer *to, t_buffer *from, unsigned int size);
void			deleteFromBuffer(t_buffer *buffer, unsigned int size);
bool			commandEnded(t_buffer *buffer);
bool			handshake(t_device *device, t_buffer *buffer);
void			respondKoFrimWare();
int				doCmd(t_buffer *buffer);

unsigned int	read(char *buffer, int size, unsigned int timeout = 0, unsigned int timeRead = 100);

#endif
