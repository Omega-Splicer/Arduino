#include "omega-splicer.h"

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

bool	byteBetween(char b, char firstToken, char lastToken)
{
	if (b >= firstToken && b <= lastToken)
		return (true);
	return (false);
}

bool	onlyDigit(t_buffer *buffer)
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

bool	onlyAlpha(t_buffer *buffer)
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

bool	onlyAlphaNum(t_buffer *buffer)
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