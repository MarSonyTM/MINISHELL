#include "libft.h"

size_t ft_strnlen(const char *s, size_t maxlen)
{
    size_t len = 0;
    while (len < maxlen && s[len] != '\0')
    {
        len++;
    }
    return len;
}