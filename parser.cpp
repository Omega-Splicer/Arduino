#include "omega-splicer.h"

void	initializeBuffer(t_buffer *buffer)
{
	buffer->size = 0;
	buffer->data[0] = 0;
}

bool	addToBuffer(t_buffer *to, t_buffer *from)
{
	if (to->size + from->size >= 128)
		return (false);
	memcpy(to->data + to->size, from->data, from->size);
	to->size += from->size;
	to->data[to->size] = 0;
	return (true);
}

bool	addToBuffer(t_buffer *to, t_buffer *from, unsigned int size)
{
	if (to->size + size >= 128)
		return (false);
	memcpy(to->data + to->size, from->data, size);
	to->size += size;
	to->data[to->size] = 0;
	return (true);
}

bool	addToBuffer(t_buffer *to, char *from, unsigned int size)
{
	if (to->size + size >= 128)
		return (false);
	memcpy(to->data + to->size, from, size);
	to->size += size;
	to->data[to->size] = 0;
	return (true);
}

bool	addToBuffer(t_buffer *to, int number)
{
	char	tmp[10];

	sprintf(tmp, "%d", number);
	if (to->size + strlen(tmp) >= 128)
		return (false);
	memcpy(to->data + to->size, tmp, strlen(tmp));
	to->size += strlen(tmp);
	to->data[to->size] = 0;
	return (true);
}

void	deleteFromBuffer(t_buffer *buffer, unsigned int size)
{
	unsigned int	bufferLeft = buffer->size - size;

	if (bufferLeft <= 0 || bufferLeft >= 128) {
		buffer->size = 0;
		buffer->data[0] = 0;
	}
	else {
		memcpy(buffer->data, buffer->data + size, bufferLeft);
		buffer->size = bufferLeft;
		buffer->data[buffer->size] = 0;
	}
}

bool	getsomething(t_buffer *toSearch, t_buffer *toStore, char token)
{
	unsigned int i = 0;
	t_buffer	data;

	initializeBuffer(&data);
	initializeBuffer(toStore);
	addToBuffer(&data, toSearch);
	while (i != toSearch->size)
	{
		if (toSearch->data[i] == token)
		{
			addToBuffer(toStore, toSearch, i);
			toStore->data[i] = 0;
			toStore->size = i;
			return (true);
		}
		i++;
	}
	return (false);
}

bool	commandEnded(t_buffer *buffer)
{
	unsigned int i = 0;

	while (i != buffer->size) {
		if (buffer->data[i] == ';') {
			return (true);
		}
		i++;
	}
	return (false);
}

bool		byteBetween(char b, char firstToken, char lastToken)
{
	if (b >= firstToken && b <= lastToken)
		return (true);
	return (false);
}

bool		onlyDigit(t_buffer *buffer)
{
	unsigned int i = 0;

	if (buffer->size == 0)
		return (false);
	if (buffer->size > 0 && buffer->data[0] == '-')
		i++;
	while (i != buffer->size)
	{
		if (!byteBetween(buffer->data[i], '0', '9'))
			return (false);
		i++;
	}
	return (true);
}

bool		onlyAlpha(t_buffer *buffer)
{
	unsigned int i = 0;

	if (buffer->size == 0)
		return (false);
	while (i != buffer->size)
	{
		if (!(byteBetween(buffer->data[i], 'A', 'Z') || byteBetween(buffer->data[i], 'a', 'z')))
			return (false);
		i++;
	}
	return (true);
}

bool		onlyAlphaNum(t_buffer *buffer)
{
	unsigned int i = 0;

	if (buffer->size == 0)
		return (false);
	while (i != buffer->size)
	{
		if (!(byteBetween(buffer->data[i], '0', '9') || byteBetween(buffer->data[i], 'A', 'Z') || byteBetween(buffer->data[i], 'a', 'z')))
			return (false);
		i++;
	}
	return (true);
}

void		disconnect(t_buffer *param, bool *running, t_device *device)
{
	*running = false;
}

void		firmwareUpdate(t_buffer *param, bool *running, t_device *device)
{
	if (localVersion.major > device->version.major ||
		(localVersion.major >= device->version.major &&
		 localVersion.minor > device->version.minor)) {
		respondKoFrimWare();
	}
	else {
		respondOk();
	}
}

void		initialize(t_buffer *param, bool *running, t_device *device)
{
	t_buffer	respondBuffer;
	int			i;

	detect_plugin(device);
	initializeBuffer(&respondBuffer);
	if (!addToBuffer(&respondBuffer, device->name, strlen(device->name)) ||
		!addToBuffer(&respondBuffer, ",", 1)) {
		respondKoBuffer();
		return ;
	}
	i = 0;
	while (device->plugins && device->plugins[i])
		i++;
	if (!addToBuffer(&respondBuffer, i)) {
		respondKoBuffer();
		return ;
	}
	i = 0;
	while (device->plugins && device->plugins[i]) {
		if (!addToBuffer(&respondBuffer, ",", 1) ||
			!addToBuffer(&respondBuffer, device->plugins[i]->name, strlen(device->plugins[i]->name)) ||
			!addToBuffer(&respondBuffer, ",", 1) ||
			!addToBuffer(&respondBuffer, device->plugins[i]->parameters, strlen(device->plugins[i]->parameters))) {
			respondKoBuffer();
			return ;
		}
		i++;
	}
	if (!addToBuffer(&respondBuffer, ";", 1)) {
		respondKoBuffer();
		return ;
	}
	respond(respondBuffer.data, respondBuffer.size);
	//todo
}

void		control(t_buffer *param, bool *running, t_device *device)
{
	//todo
}

void		update(t_buffer *param, bool *running, t_device *device)
{
	//todo
}

void		executeCmd(t_buffer *cmd, t_buffer *param, bool *running, t_device *device)
{
	int i = 0;
	while (cmdPtr[i].cmd != NULL) {
		if (!strncmp(cmdPtr[i].cmdName, cmd->data, cmd->size)) {
			cmdPtr[i].cmd(param, running, device);
			return ;
		}
		i++;
	}
	respondKoFrimWare();
}

int			doCmd(t_buffer *buffer, bool *running, t_device *device)
{
	t_buffer	cmd;
	t_buffer	parser;
	int			cmdSize;

	getsomething(buffer, &parser, ';');
	cmdSize = parser.size;
	if (getsomething(&parser, &cmd, ':')) {
		deleteFromBuffer(&parser, cmd.size + 1);
		executeCmd(&cmd, &parser, running, device);
	}
	else {
		executeCmd(&parser, NULL, running, device);
	}
	return (cmdSize);
}

bool		handshakeDataCheck(t_device *device, t_buffer *buffer)
{
	t_buffer parser;
	t_buffer subParser;
	unsigned int size;

	if (getsomething(buffer, &parser, ';')) {
		size = parser.size;
		if (getsomething(&parser, &subParser, ':') && !strcmp(subParser.data, "Handshake"))
		{
			deleteFromBuffer(&parser, subParser.size + 1);
			if (getsomething(&parser, &subParser, ',') && onlyAlpha(&subParser))
			{
				memcpy(device->name, subParser.data, subParser.size);
				device->name[subParser.size] = 0;
				deleteFromBuffer(&parser, subParser.size + 1);
				if (getsomething(&parser, &subParser, '.') && onlyDigit(&subParser)) {
					device->version.major = atoi(subParser.data);
					deleteFromBuffer(&parser, subParser.size + 1);
					if (onlyDigit(&subParser)) {
						device->version.minor = atoi(parser.data);
						/* Print log + debug */
						/*
						Serial.print("//Handhake done : ");
						Serial.print(device->name);
						Serial.print(" : ");
						Serial.print(device->version.major);
						Serial.print(".");
						Serial.println(device->version.minor);
						activateLED(pin_DataOk);
						delay(100);
						desactivateLED(pin_DataOk);
						*/
						/* end log */
						deleteFromBuffer(buffer, size);
						return (true);
					}
				}
			}
		}
	}
	/* Print log + debug */
	//Serial.println("//Handshake error");
	//activateLED(pin_DataError);
	/* end log */
	return (false);
}

bool		handshake(t_device *device, t_buffer *buffer)
{
	if (handshakeDataCheck(device, buffer)) {
		if (device->version.major != localVersion.major ||
			device->version.minor != localVersion.minor) {
			//Serial.println("//Wrong version");
			return (false);
		}
		return (true);
	}
	return (false);
}

void	respondKoFrimWare()
{
	t_buffer buffer;

	initializeBuffer(&buffer);
	if (!addToBuffer(&buffer, "KO:", 3) ||
		!addToBuffer(&buffer, localVersion.major) ||
		!addToBuffer(&buffer, ".", 1) ||
		!addToBuffer(&buffer, localVersion.minor) ||
		!addToBuffer(&buffer, ";", 1)) {
		respondKoBuffer();
		return ;
	}
	respond(buffer.data, buffer.size);
}

void	respondKoBuffer()
{
	respond("KO:Error;", 9);
}

void	respondOk()
{
	respond("OK;", 3);
}
