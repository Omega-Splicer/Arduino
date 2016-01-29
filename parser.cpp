#include "omega-splicer.h"

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
	respond(&respondBuffer);
}

void		control(t_buffer *param, bool *running, t_device *device)
{
	int i;
	t_buffer pluginName;
	bool done = false;

	initializeBuffer(&pluginName);
	if (getsomething(param, &pluginName, ',')) {
		deleteFromBuffer(param, pluginName.size + 1);
	}
	else {
		addToBuffer(&pluginName, param);
	}

	i = 0;
	while (device->plugins && device->plugins[i]) {
		if (device->plugins[i]->control) {
			if (pluginName.size == strlen(device->plugins[i]->name) && !strncmp(pluginName.data, device->plugins[i]->name, pluginName.size)) {
				device->plugins[i]->control(device->plugins[i], param);
				done = true;
			}
		}
		i++;
	}
	if (!done)
		respondKo();
}

void		update(t_buffer *param, bool *running, t_device *device)
{
	int i;
	bool done = false;

	i = 0;
	while (device->plugins && device->plugins[i]) {
		if (device->plugins[i]->update) {
			if (param->size == strlen(device->plugins[i]->name) && !strncmp(param->data, device->plugins[i]->name, param->size)) {
				device->plugins[i]->update(device->plugins[i]);
				done = true;
			}
		}
		i++;
	}
	if (!done)
		respondKo();
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
						Serial.print("Handhake done : ");
						Serial.print(device->name);
						Serial.print(" : ");
						Serial.print(device->version.major);
						Serial.print(".");
						Serial.println(device->version.minor);
						activateLED(pin_DataOk);
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
	//Serial.println("Handshake error");
	//activateLED(pin_DataError);
	/* end log */
	return (false);
}

bool		handshake(t_device *device, t_buffer *buffer)
{
	if (handshakeDataCheck(device, buffer)) {
		if (device->version.major != localVersion.major ||
			device->version.minor != localVersion.minor) {
			//Serial.println("Wrong version");
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
	respond(&buffer);
}

void	respondKoBuffer()
{
	respond("KO:Error;", 9);
}

void	respondKo()
{
	respond("KO;", 9);
}

void	respondOk()
{
	respond("OK;", 3);
}