#include "omega-splicer.h"

using namespace std;

void	activateLED(int led)
{
	digitalWrite(led, HIGH);
}

void	desactivateLED(int led)
{
	digitalWrite(led, LOW);
}

int			run()
{
	bool		running = true;
	t_buffer	buffer;
	t_device	device;
	int			cmdSize;
	int			i;

	device.plugins = NULL;
	initializeBuffer(&buffer);
	while (!commandEnded(&buffer)) {
		if (!rcvData(&buffer))
			return respondKoFrimWare("No handshake, or buffer full");
	}
	if (handshake(&device, &buffer))
	{
		initializeBuffer(&buffer);
		respondOk();
		detect_plugin(&device);
		while (running)
		{
    

			while (!commandEnded(&buffer))
				if (!rcvData(&buffer))
					return respondKoFrimWare("communication error, or buffer full");
				cmdSize = doCmd(&buffer, &running, &device);
				deleteFromBuffer(&buffer, cmdSize + 1);
			}
		}
		else
			return respondKoFrimWare("Wrong handshake");
		i = 0;
		while (device.plugins && device.plugins[i]) {
			free(device.plugins[i]);
			i++;
		}
		if (device.plugins)
			free(device.plugins);
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
		respondKoFrimWare("Cannot execute all commands");
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
		else
			executeCmd(&parser, NULL, running, device);
		return (cmdSize);
	}
