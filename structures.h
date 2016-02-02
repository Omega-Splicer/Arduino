#ifndef		STRUCTURES_H_
# define	STRUCTURES_H_

typedef struct
{
	unsigned int	size;
	char			data[128];
} t_buffer;

typedef struct s_plugin
{
	int		inPin;
	int 	outPin;
	int		speed;
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

#endif