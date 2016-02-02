#ifndef		COMM_H_
# define	COMM_H_

#include	"Arduino.h"
#include	"omega-splicer.h"
#include 	"structures.h"

unsigned int	read(char *buffer, int size, unsigned int timeout = 0, unsigned int timeRead = 100);

void			respond(t_buffer *buffer);
bool			commandEnded(t_buffer *buffer);
bool			handshake(t_device *device, t_buffer *buffer);
int				respondKoFrimWare(const char* error);
void			respondOk();
void			respondKo();
int				doCmd(t_buffer *buffer, bool *running, t_device *device);
void			respondKoBuffer();
bool			rcvData(t_buffer *buffer);
void			respond(t_buffer *buffer);
void			respond(char *data);

#endif