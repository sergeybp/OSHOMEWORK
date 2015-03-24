#include "helpers.h"

ssize_t read_(int fd, void *buf, size_t num) 
{
    size_t result = 0;
    size_t length = 0;
    if (num == 0)
    {
        return read(fd, buf, 0);
    }
    for(;;) 
    {
        if ((length = read(fd, buf + result, num)) == -1)
        {
            return -1;
        }
        result += length;
        num -= length;
        if (num <= 0)
        {
            return result;
        }
        if (length <= 0)
        {
            return result;
        }
    }
}
ssize_t write_(int fd, const void *buf, size_t num) 
{
    size_t result = 0;
    size_t length = 0;
    do 
    {
        if ((length = write(fd, buf + result, num)) == -1)
        {
            return -1;
        }
        num -= length;
        result += length;
    } 
    while (length > 0 && num > 0);
    return result;
}
ssize_t read_until(int fd, void * buf, size_t num, char delimiter) {
    size_t result = 0;
    size_t length = 0;
    if (num == 0)
    {
        return read(fd, buf, 0);
    }
    for(;;) 
    {
        if ((length = read(fd, buf + result, num)) == -1) 
        {
            return -1;
        }
        int i;
        for (i = 0; i < length; i++) 
        {
            if (((char*) buf)[result + i] == delimiter) 
            {
                result += length;
                num -= length;
                return result;
            }
        }
        result += length;
        num -= length;
        if (num <= 0)
        {
            return result;
        }
        if (length <= 0)
        {
            return result;
        }
    }
    return result;
}
