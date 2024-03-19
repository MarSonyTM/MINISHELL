#include "../../inc/minishell.h"

char    *ft_getenv(const char *name, t_env *env)
{
    t_env   *tmp;

    tmp = env;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, name) == 0)
            return (tmp->value);
        tmp = tmp->next;
    }
    return (NULL);
}
