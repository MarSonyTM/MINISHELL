#include "libft.h"

char *ft_strndup(const char *s, size_t n)
{
    size_t len = ft_strnlen(s, n);
    char *new = malloc(len + 1);

    if(new == NULL)
        return NULL;

    new[len] = '\0';
    return ft_memcpy(new, s, len);
}