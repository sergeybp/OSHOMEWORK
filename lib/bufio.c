#include <string.h>

#include "bufio.h"

struct buf_t *buf_new(size_t capacity) 
{
    char *buf = (char *)malloc(capacity * sizeof(char));
    struct buf_t *new_buf = malloc(sizeof(struct buf_t));
    if (buf == NULL || new_buf == NULL) 
    {
        return NULL;
    }
    new_buf->size = 0;
    new_buf->capacity = capacity;
    new_buf->buf = buf;
    return new_buf;
}

size_t buf_capacity(struct buf_t *buf) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    return buf->capacity;
}

size_t buf_size(struct buf_t *buf) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    return buf->size;
}

void buf_free(struct buf_t *buf) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    free(buf->buf);
    free(buf);
}

ssize_t buf_fill(int fd, struct buf_t *buf, size_t required) 
{
#ifdef DEBUG
    if (buf == NULL || buf_capacity(buf) - buf_size(buf) < required) 
    {
        abort();
    }
#endif
    while (buf->size < required) 
    {
        ssize_t cnt = read(fd, buf->buf + buf->size, buf->capacity - buf->size);
        if (cnt == -1) 
        {
            return -1;
        }
        if (cnt == 0) 
        {
            break;
        }
        buf->size += cnt;
    }
    return buf->size;
}

ssize_t buf_flush(int fd, struct buf_t *buf, size_t required) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    ssize_t res = buf->size;
    while (required > 0) 
    {
        ssize_t cnt = write(fd, buf->buf, buf->size);
        if (cnt == -1) 
        {
            return -1;
        }
        memmove(buf->buf, buf->buf + cnt, buf->size - cnt);
        buf->size -= cnt;
        required -= cnt;
        if (buf->size == 0) 
        {
            break;
        }
    }
    return res - buf->size;
}

ssize_t buf_getline(int fd, struct buf_t* buf, char* dest) 
{
#ifdef DEBUG
    if (buf == NULL) 
    {
        abort();
    }
#endif
    int i;
    ssize_t res = 0;
    for(i = 0; i < buf->size; ++i) 
    {
        if(buf->buf[i] == '\n')
        {
            break;
        }
    }
    if (i == buf->size)
    {
        memmove(dest, buf->buf, buf->size);
        res = buf->size;
        buf->size = 0;
        int f = buf_fill(fd, buf, 1);
        if (f > 0)
        {
            res += buf_getline(fd, buf, dest + res);
        }
        return res;
    } 
    else 
    {
        memmove(dest, buf->buf, i + 1);
        memmove(buf->buf, buf->buf + i + 1, buf->size - i - 1);
        buf->size -= (i + 1);
        return i + 1;
    }
}

ssize_t buf_write(int fd, struct buf_t* buf, char* src, size_t len) 
{
    int k;
    if((buf->capacity - buf->size) < len)
    {
        k = buf->capacity - buf->size;
    }
    else
    {
        k = len;
    }
    memcpy(buf->buf + buf->size, src, k);
    buf->size += k;
    int r = buf_flush(fd, buf, buf_size(buf));
    if (r < 0)
    {
        return r;
    }
    if (k < len) 
    {
        return buf_write(fd, buf, src + k ,len - k);
    }
    return 0;
}
