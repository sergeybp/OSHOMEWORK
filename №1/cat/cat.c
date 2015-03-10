#include <helpers.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

const size_t LENG = 4096;

void print_error() 
{
	char *msg = strerror(errno);
    write_(STDERR_FILENO, msg, strlen(msg));
}

int main() 
{
	char buf[LENG];
	size_t len = 0;
	size_t num = sizeof(buf);
	for(;;)
	{
		if (write_(STDOUT_FILENO, buf, len) < len) 
		{
			print_error();
			exit(1);
		}
		if ((len = read_(STDIN_FILENO, buf, num)) == -1) 
		{
			print_error();
			exit(1);
		}
		if (len != num)
		{
			return 0;
		}
	}
}
