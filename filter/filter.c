#include <helpers.h>
#include <string.h>
#include <stdlib.h>

char ** args;

void init(int n, char * a[], char b[4096])
{
    args = malloc(sizeof(char *) * (n + 1));
	int i;
	for (i = 0; i < n - 1; ++i)
    {
		args[i] = a[i + 1];
    }
	args[n] = 0;
	args[n - 1] = b;
}

int main(int argc, char * argv []) {
	char buf[4096];
	char str[4096];
	size_t leng = 0;
    size_t pos = 0;
    char endl = '\n';
    init(argc,argv,str);
    int i;
	for(;;) 
    {
		leng = read_until(STDIN_FILENO, buf, sizeof(buf), endl);
		for (i = 0; i < leng; ++i) 
        {
			if (buf[i] == endl) 
            {
				str[pos] = 0;
				int sp;
				if (sp = spawn(args[0], args)== 0) 
                {
					write(STDOUT_FILENO, str, strlen(str));
					char ch = endl;
					write(STDOUT_FILENO, &ch, 1);
				}
				pos = 0;
			} 
            else 
            {
				str[pos] = buf[i];
				pos++;
			}
		}
		if (leng <= 0) 
        {
			if (pos > 0) 
            {
				str[pos] = 0;
				int sp; 
                if (sp = spawn(args[0], args) == 0) 
                {
                    write(STDOUT_FILENO, str, strlen(str));
                    write(STDOUT_FILENO, &endl, 1);
                }
			}
			break;
		}
	}
	free(args);
	return 0;
}
