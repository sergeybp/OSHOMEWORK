#include <bufio.c>
#include <helpers.c>
#include <stdio.h>

int main()
{
	int p = 50;
	struct execargs_t* progs[p];
	size_t pnum = 0;
	char* line;
	struct buf_t* buf = buf_new(4096);
	if (buf == NULL)
		return -1;
	ssize_t num = 0;
	while (1)
	{
		line = (char*) malloc(4096*2);
		ssize_t l = write(STDOUT_FILENO, "$", 1);
		if (l < 0)
			return -1;
		num = buf_getline(STDIN_FILENO, buf, line);
		if (num < 0)
			return -1;
		else if (num == 0)
			break;
	        int i;
	        int j;
		char prog[4096];
		char* args[p];
		for (i = 0; i < p; i++)
			args[i] = malloc(4096);
		int argc = 0;
		char prog_last = 0;
		size_t size = 0;
		int start = 0;
		pnum = 0;
		while (line[start] == ' ')
			start++;
		for (i = start; i < num; i++)
		{
			if (line[i] == '|')
			{
				struct execargs_t* exec = exec_new(prog, args, argc);
				progs[pnum] = exec;
				pnum++;
				if (!prog_last) 
				{
					prog[size] = 0;
					args[argc][size] = 0;
					argc++;
				}
				else
				{
					args[argc][size] = 0;
					argc++;
				}
				while (line[i + 1] == ' ')
				{
					i++;
				}	
				prog_last = 0;
				argc = 0;
				size = 0;
			}
			else if (line[i] == ' ' || line[i] == 0 || line[i] == '\n' )
			{
				if (!prog_last)
				{
					prog[size] = 0;
					args[argc][size] = 0;
					argc++;
					prog_last = 1;
				}
				else
				{
					args[argc][size] = 0;
					argc++;
				}
				size = 0;
				while (line[i + 1] == ' ')
					i++;
			}
			else
			{
				if (!prog_last)
				{
					prog[size] = line[i];
					if (argc == 0)
						args[argc][size] = line[i];
					size++;
				}
				else
				{
					args[argc][size] = line[i];
					size++;
				}
			}
		}
		struct execargs_t* exec = exec_new(prog, args, argc);
		progs[pnum] = exec;
		pnum++;
		int res = runpiped(progs, pnum);
		if (res < 0)
			return -1;
	}
	buf_free(buf);
	return 0;
}

