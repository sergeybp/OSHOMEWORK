#include "helpers.h"


ssize_t read_(int fd, void *buf, size_t num) {
	size_t res = 0;
	size_t len = 0;
	if (num == 0)
	{
		return read(fd, buf, 0);
	}
	for(;;)
	{
		if ((len = read(fd, buf + res, num)) == -1)
		{
			return -1;
		}
		if (len == num)
		{
			return res + len;
		}
		if (len == 0)
		{
			return res;
		}
		res += len;
		num -= len;
		buf += len;
	}
}


ssize_t write_(int fd, const void *buf, size_t num) {
	size_t res = 0;
	size_t len = 0;
	do 
	{
		if ((len = write(fd, buf + res, num)) == -1)
		{
			return -1;
		}
		res += len;
		num -= len;
	} 
	while (len > 0 && num > 0);
	
	return res;
}
