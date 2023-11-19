#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for convert_number() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *					allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the file descriptor fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} inf_a;


#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(inf_a *);
} builtin_table;


/* hsh.c */
int hsh(inf_a *, char **);
int find_builtin(inf_a *);
void find_cmd(inf_a *);
void fork_cmd(inf_a *);

/* path.c */
int is_cmd(inf_a *, char *);
char *dup_chars(char *, int, int);
char *find_path(inf_a *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* err_string_functions.c */
void _eputs(char *);
int _eputchar(char);
int _writefd(char c, int fd);
int _writesfd(char *str, int fd);

/* string_functions.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* string_functions2.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* string_functions3.c */
char *_strncpy(char *, char *, int);
char *_strncat(char *, char *, int);
char *_strchr(char *, char);

/* string_functions4.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* memory_functions */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_realloc(void *, unsigned int, unsigned int);

/* memory_functions2.c */
int bfree(void **);

/* more_functions.c */
int interactive(inf_a *);
int is_delim(char, char *);
int _alpha(int);
int _atoi(char *);

/* more_functions2.c */
int _erratoi(char *);
void print_error(inf_a *, char *);
int print_d(int, int);
char *convert_number(long int, int, int);
void remove_comment(char *);

/* builtin_emulators.c */
int _myexit(inf_a *);
int _mycd(inf_a *);
int _myhelp(inf_a *);

/* builtin_emulators2.c */
int _myhistory(inf_a *);
int _myalias(inf_a *);

/* getline.c module */
ssize_t get_input(inf_a *);
int _getline(inf_a *, char **, size_t *);
void sigintHandler(int);

/* info.c module */
void c_info(inf_a *);
void set_info(inf_a *, char **);
void f_info(inf_a *, int);

/* env.c module */
char *_getenv(inf_a *, const char *);
int _myenv(inf_a *);
int _mysetenv(inf_a *);
int _myunsetenv(inf_a *);
int populate_env_list(inf_a *);

/* env2.c module */
char **get_environ(inf_a *);
int _unsetenv(inf_a *, char *);
int _setenv(inf_a *, char *, char *);

/* file_io_functions.c */
char *get_history_file(inf_a *info);
int write_history(inf_a *info);
int read_history(inf_a *info);
int build_history_list(inf_a *info, char *buf, int linecount);
int renumber_history(inf_a *info);

/* liststr.c module */
list_t *add_node(list_t **, const char *, int);
list_t *add_node_end(list_t **, const char *, int);
size_t print_list_str(const list_t *);
int delete_node_at_index(list_t **, unsigned int);
void free_list(list_t **);

/* liststr2.c module */
size_t list_len(const list_t *);
char **list_to_strings(list_t *);
size_t print_list(const list_t *);
list_t *node_starts_with(list_t *, char *, char);
ssize_t get_node_index(list_t *, list_t *);

/* chain.c */
int is_chain(inf_a *, char *, size_t *);
void check_chain(inf_a *, char *, size_t *, size_t, size_t);
int replace_alias(inf_a *);
int replace_vars(inf_a *);
int replace_string(char **, char *);

#endif
