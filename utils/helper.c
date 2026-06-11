#include "../codexion.h"

static void ft_bzero(void *str, size_t len)
{
	unsigned char *ptr;

	ptr = (unsigned char *)str;
	while (len > 0)
	{
		*ptr = '\0';
		ptr++;
		len--;
	}
}

void *ft_calloc(size_t nmemb, size_t size)
{
	void *res;

	if (nmemb == 0 || size == 0)
		return (malloc(1));
	if (nmemb != 0 && size > (size_t)-1 / nmemb)
		return (NULL);
	res = malloc(nmemb * size);
	if (!res)
		return (NULL);
	ft_bzero(res, nmemb * size);
	return (res);
}

long long get_ms()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	long long milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	// if need milisec of this sec tv.tv_sec /1000
	return milliseconds;
}