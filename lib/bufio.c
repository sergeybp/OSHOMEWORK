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
