#include "../../inc/minishell.h"

/* should we also handle 

NAME=variable
export NAME

or only

export NAME=variable

-->probably only the second one, since the first one is just setting a variable and not exporting it */


/* possibilities of the export command:

no arguments - like the env command v

one argument without '=' - sets an empty variable with that name v

one argument with '=' - sets a variable with key before = and value after v

one argument with "VAR=$VAR:" appends part after : to existing variable v

if there is at least one argument, export first checks if a variable with
that name already exists and, if so, replaces its value with the new one v */

static t_env   *add_env_node(t_env **env, char *key, char *value)
{
    t_env *new;
    t_env *tmp;

    new = malloc(sizeof(t_env));
    new->key = key;
    new->value = value;
    new->next = NULL;
    if (env == NULL)
        return (new);
    tmp = *env;
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = new;
    return (*env);
}

static void    add_empty_env_var(char *cmd, t_env **env)
{
    t_env *tmp;
    char *key;
    char *value;

    key = ft_strdup(cmd);
    value = ft_strdup("");
    tmp = *env;
    while (tmp != NULL)
    {
        if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
        {
            free(tmp->key);
            free(tmp->value);
            break ;
        }
        tmp = tmp->next;
    }
    if (!tmp)
        *env = add_env_node(env, key, value);
}

static void    add_new_env_var(char *cmd, t_env **env, int j)
{
    t_env *tmp;
    char *key;
    char *value;

    while (cmd[j] && cmd[j] != '=')
        j++;
    key = ft_substr(cmd, 0, j);
    value = ft_strdup(cmd + j + 1);
    tmp = *env;
    while (tmp != NULL)
    {
        if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
        {
            free(tmp->key);
            free(tmp->value);
            tmp->key = key;
            tmp->value = value;
            break ;
        }
        tmp = tmp->next;
    }
    if (!tmp)
        *env = add_env_node(env, key, value);
}

static void    concatenate_env_var(char *cmd, t_env **env, int j)
{
    t_env *tmp;
    char *key;
    char *value;

    if (ft_strchr(cmd, ':') == NULL)
        add_empty_env_var(cmd, env);    
    while(cmd[j] && cmd[j] != '=')
        j++;
    key = ft_substr(cmd, 0, j);
    j = j + 2;
    if (ft_strncmp(cmd + j, (*env)->key, ft_strlen((*env)->key)))
    {
        /* error ERR_ARG*/
    }
    else
    { 
        while (cmd[j] != ':')
                j++;
        value = ft_substr(cmd, j + 1, ft_strlen(cmd) - j - 1);
        tmp = *env;
        while (tmp != NULL)
        {
            if (!ft_strncmp(tmp->key, key, ft_strlen(key)))
            {
                tmp->value = ft_strjoin(tmp->value, value);
                break ;
            }
            tmp = tmp->next;
        }
        if (!tmp)
            *env = add_env_node(env, key, value);
    }
}

void    export_cmd(t_cmd *cmd, t_env *env)
{
    int i;
    int j;

    i = 1;
    if (!cmd->cmd_arr[1]) //export command with no argument
    {
        env_cmd(cmd, env);
        return ;
    }
    if (cmd->cmd_arr[1][0] == '=' || ft_isdigit(cmd->cmd_arr[1][0]))
    {
        /* error ERR_ARG*/
    }
    while (cmd->cmd_arr[i])
    {
        if (ft_strchr(cmd->cmd_arr[i], '='))
        {
            j = 0;
            if (ft_strchr(cmd->cmd_arr[i], '$'))
                concatenate_env_var(cmd->cmd_arr[i], &env, j);
            else
                add_new_env_var(cmd->cmd_arr[i], &env, j);
        }
        else
            add_empty_env_var(cmd->cmd_arr[i], &env);
        i++;
    }
}
