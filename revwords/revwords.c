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

void reverse_word(char* word, int pos) {
    int i;
    for(i = 0; i < pos / 2; i++) {
        char ch = word[i];
        word[i] = word[pos - i - 1];
        word[pos - i - 1] = ch;
    }
}

int main() {
    char buf[4096];
    char word[4096];
    size_t pos = 0;
    size_t length;
    while(length = read_until(STDIN_FILENO, buf, sizeof(buf), ' ')) 
    {
        if (length == -1) 
        {
            print_error();
            return 1;
        }
        int i = 0;
        int pos = 0;
        while(i!=length)
        {
			if(buf[i] == ' ')
			{
				if (pos != 0) 
				{
					reverse_word(word, pos);
					write_(STDOUT_FILENO, word, pos);
				}
				char ch = ' ';
				write_(STDOUT_FILENO, &ch, (size_t)1);
				pos = 0;
			}
            else
            {
				word[pos] = buf[i];
				pos++;
			}
			i++;
		}
        if (length <= 0) 
        {
			if (pos != 0) 
			{
				reverse_word(word, pos);
				write_(STDOUT_FILENO, word, (size_t)pos);
			}           
			return 0;
		}   
			
	}
	return 0;
}
