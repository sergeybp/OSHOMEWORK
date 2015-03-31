#include "bufio.c"

#define BUF_CAPACITY 4096

int main() 
{
    struct buf_t* buf = buf_new(BUF_CAPACITY);
    for(;;)
    {
        ssize_t cnt = buf_fill(STDIN_FILENO, buf, buf_capacity(buf) - buf_size(buf));
        if (buf_size(buf) != buf_capacity(buf)) 
        {
            buf_flush(STDOUT_FILENO, buf, buf_size(buf));
            buf_free(buf);
            if (cnt == -1) 
            {
                return -1;
            }
            return 0;
        }
        if (buf_flush(STDOUT_FILENO, buf, buf_size(buf)) == -1)
        {
            exit(EXIT_FAILURE);
        }
    }
}
