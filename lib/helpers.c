#include "helpers.h"
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

struct execargs_t
{
        char** args;
	int argc;
	char* prog;
};

char return_zero = 0;

void print_error() 
{
    char *msg = strerror(errno);
    write_(STDERR_FILENO, msg, strlen(msg));
}

ssize_t read_(int fd, void *buf, size_t num) 
{
    size_t res = 0;
    size_t len = 0;
    if (num == 0)
    {
        return read(fd, buf, 0);
    }
    for(;;) 
    {
        if ((len = read(fd, buf + res, num)) == -1)
        {
            return -1;
        }
        res += len;
        num -= len;
        if (num <= 0)
        {
            return res;
        }
        if (len <= 0)
        {
            return res;
        }
    }
}
ssize_t write_(int fd, const void *buf, size_t num) 
{
    size_t res = 0;
    size_t len = 0;
    do 
    {
        if ((len = write(fd, buf + res, num)) == -1)
        {
            return -1;
        }
        num -= len;
        res += len;
    } 
    while (len > 0 && num > 0);
    return res;
}
ssize_t read_until(int fd, void * buf, size_t num, char delimiter) {
    size_t res = 0;
    size_t len = 0;
    if (num == 0)
    {
        return read(fd, buf, 0);
    }
    for(;;) 
    {
        if ((len = read(fd, buf + res, num)) == -1) 
        {
            return -1;
        }
        int i;
        for (i = 0; i < len; i++) 
        {
            if (((char*) buf)[res + i] == delimiter) 
            {
                res += len;
                num -= len;
                return res;
            }
        }
        res += len;
        num -= len;
        if (num <= 0)
        {
            return res;
        }
        if (len <= 0)
        {
            return res;
        }
    }
    return res;
}

int spawn(const char *file, char *const argv []) {
    pid_t p = fork();
    if (p == -1) 
    {
        print_error();
        return -1;
    }
    if (p == 0) 
    {
        if (execvp(file, argv) == -1) 
        {
            print_error();
            return -1;
        }
    } 
    else 
    {
        int status;
        if (waitpid(p, &status, 0) == -1) 
        {
            print_error();
            return -1;
        }
        if (!WIFEXITED(status))
        {
            return -1;
        }
        else
        {
            return WEXITSTATUS(status);
        }
    }
    return -1;
}

void handler(int num)
{
	if (num == SIGINT)
	{
		while (wait(NULL) != -1)
		{
		}
		return_zero = 1;
	}
}

int runpiped(struct execargs_t** progs, size_t n)
{
	signal(SIGINT, handler);
	int (*pipefd)[2] = (int(*)[2]) malloc(sizeof(int[2]) * n - 1);
	int* child = (int*) malloc(sizeof(int*) * n);
	int read = -1; 
	int in = dup(STDIN_FILENO);
	int write = 0;
	int out = dup(STDOUT_FILENO);
        size_t i;
	for (i = 0; i < n; i++)
	{
		if (i > 0)
		{
			if ((dup2(pipefd[read][0], STDIN_FILENO) < 0) | (close(pipefd[read][0]) < 0))
			{
				free(pipefd);
				free(child);
				return (return_zero ? 0 : -1);
			}
		}
		if (i < n - 1)
		{
			if ((pipe(pipefd[write]) < 0) | (dup2(pipefd[write][1], STDOUT_FILENO) < 0) | (close(pipefd[write][1]) < 0))
			{
				free(pipefd);
				free(child);
				return (return_zero ? 0 : -1);			
			}
		}
		else
		{
			if (dup2(out, STDOUT_FILENO) < 0)
			{
				free(pipefd);
				free(child);
				return (return_zero ? 0 : -1);
			}
		}
		child[i] = exec(progs[i]);
		if (child[i] < 0)
		{
			free(pipefd);
			free(child);
			return (return_zero ? 0 : -1);
		}
		read++;
		write++;
	}
	if (dup2(in, STDIN_FILENO) < 0)
	{
		free(pipefd);
		free(child);
		return (return_zero ? 0 : -1);
	}
        size_t num;
	for (num = 0; num < n; num++)
	{
		int tmp = wait(NULL);
		if (tmp < 0)
		{
			free(pipefd);
			free(child);
			return (return_zero ? 0 : -1);
		}
		if (num == 0)
		{
                        size_t i;
			for (i = 0; i < n; i++)
				if (tmp != child[i])
					kill(child[i], SIGINT);
		}
	}	
	free(pipefd);
	free(child);
	return 0;
}

int exec(struct execargs_t* args)
{
	args -> args[args -> argc] = NULL;
	args -> argc++;
	pid_t pid = fork();
	if (pid < 0)
		return -1;
	else if (pid == 0)
	{
		execvp(args -> prog, args -> args);
		return pid;
	}
	else
		return pid;
}

struct execargs_t* exec_new(char* prog, char** args, int argc)
{
        int j;
	struct execargs_t* ans = (struct execargs_t*) malloc(sizeof(struct execargs_t));
	if (ans == NULL)
		return ans;
	ans -> prog = (char*) malloc(4096);
	if (ans -> prog == NULL)
		return NULL;
	int i = 0;
	while (1)
	{
		ans -> prog[i] = prog[i];
		if (prog[i] == 0)
			break;
		i++;
	}
	ans -> args = (char**) malloc(sizeof(char*) * (argc + 1));
	for (j = 0; j < argc + 1; j++)
		ans -> args[j] = (char*) malloc(4096);
	if (ans -> args == NULL)
		return NULL;
	for (j = 0; j < argc; j++)
	{
		int k = 0;
		while (1)
		{
			ans -> args[j][k] = args[j][k];
			if (args[j][k] == 0)
				break;
			k++;
		}
	}
	ans -> argc = argc;
	return ans;
}
