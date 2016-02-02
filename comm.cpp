#include "comm.h"

unsigned int	read(char *buffer, int size, unsigned int timeout, unsigned int timeRead)
{
	unsigned int i = 0;
	unsigned int timeSpend = 0;

	while (Serial.available() <= 0 && (timeout == 0 || timeSpend < timeout))
	{
		timeSpend += timeRead;
		delay(timeRead);
	}
	while (Serial.available() > 0)
	{
		buffer[i] = Serial.read();
		i++;
		if (i >= size + 1)
		{
			buffer[i] = 0;
			return (i);
		}
	}
	buffer[i] = 0;
	return (i);
}

bool	rcvData(t_buffer *buffer)
{
	t_buffer	tmp;

	tmp.size = read(tmp.data, 128);
	return (addToBuffer(buffer, &tmp));
}

void	respond(t_buffer *buffer)
{
	Serial.write(buffer->data, buffer->size);
}

void	respond(char *data)
{
	Serial.print(data);
}

void		disconnect(t_buffer *param, bool *running, t_device *device)
{
	*running = false;
}

int	respondKoFrimWare(const char *error)
{
	t_buffer buffer;

	initializeBuffer(&buffer);
	if (!addToBuffer(&buffer, "KO:", 3) ||
		!addToBuffer(&buffer, localVersion.major) ||
		!addToBuffer(&buffer, ".", 1) ||
		!addToBuffer(&buffer, localVersion.minor) ||
		!addToBuffer(&buffer, ";", 1) ||
		!addToBuffer(&buffer, error, strlen(error)))
	{
		respondKoBuffer();
	}
	else
		respond(&buffer);
	return true;
}

void	respondKoBuffer()
{
	respond("KO:Error;");
}

void	respondKo()
{
	respond("KO;");
}

void	respondOk()
{
	respond("OK;");
}