#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <helpers.h>

void print_error() 
{
	char *msg = strerror(errno);
    write_(STDERR_FILENO, msg, strlen(msg));
}

int main(int argc, char *argv[]) {
    char buf[4096];
    char word[4096];
    int num = sizeof(buf);
    size_t length;
    
	length = read_(STDIN_FILENO, buf, num);
        if (length == -1) 
        {
            print_error();
            return 1;
        }
        int i;
        int d = 0;
        for(i = 0 ; i < length;i++)
        {
			if(d>1)
			{
				d--;
				continue;
			}
			if(buf[i] == argv[1][0])
			{
				int j;
				int u = 0;
				for(j = 0 ; j < strlen(argv[1]);j++)
				{
					if(argv[1][j]!=buf[i+j])
					{
						u =1;
						break;
					}
					
				}
				if(u==1)
				{
					write_(STDOUT_FILENO, &buf[i], (size_t)1);
					continue;
				}
				d = strlen(argv[1]);
			}else{
				write_(STDOUT_FILENO, &buf[i], (size_t)1);
			}
		}	
	char ch = '\n';
	write_(STDOUT_FILENO, &ch, (size_t)1);
	
	return 0;
}
