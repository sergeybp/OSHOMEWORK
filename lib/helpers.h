#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>

ssize_t read_(int fd, void *buf, size_t num);
ssize_t write_(int fd, const void *buf, size_t num);
ssize_t read_until(int fd, void * buf, size_t num, char delimiter);

#endif
