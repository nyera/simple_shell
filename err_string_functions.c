#include "shell.h"

/**
 *_eputs - prints an input string
 * @err: the string to be printed
 *
 * Return: Nothing
 */
void _eputs(char *err)
{
	int x = 0;

	if (!err)
		return;
	while (err[x] != '\0')
	{
		_eputchar(err[x]);
		x++;
	}
}

/**
 * _eputchar - writes the character c to stderr
 * @e: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _eputchar(char e)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (e == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = e;
	return (1);
}

/**
 * _writefd - writes the character c to given fd
 * @c: The character to print
 * @fd: The filedescriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _writefd(char c, int fd)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(fd, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}

/**
 *_writesfd - prints an input string
 * @str: the string to be printed
 * @fd: the filedescriptor to write to
 *
 * Return: the number of chars put
 */
int _writesfd(char *str, int fd)
{
	int i = 0;

	if (!str)
		return (0);
	while (*str)
	{
		i += _writfd(*str++, fd);
	}
	return (i);
}
