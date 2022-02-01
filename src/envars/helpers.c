#include "libft.h"
#include "utils.h"
#include "envars.h"

/**
 * @brief Find the start of a variable name in a string
 *
 * @param str The string to search through
 * @param var_start A pointer which will be set to the next start of a variable
 * @return Returns 0 on unclosed single quote, 1 on success (also if no var!)
 */
int	find_var_name_start(char *str, char **var_start)
{
	char	*dollar;

	dollar = ft_strschr(str, "$\'");
	while (dollar && *dollar != '$')
	{
		if (!*dollar)
			break ;
		if (dollar && *dollar == '\'' && !skip_over_single_quotes(&dollar))
			return (0);
		dollar = ft_strschr(dollar + 1, "$\'");
	}
	if (dollar && *(dollar + 1) == '{')
		dollar++;
	*var_start = dollar;
	return (1);
}

/**
 * @brief Find the end of a variable name in a string
 *
 * @param str the start of a variable name in the string, right after the $
 * @return char* the end of said variable, points to the char after the var
 */
char	*find_var_name_end(char *str)
{
	char	*chr;

	if (*(str - 1) == '{')
	{
		chr = ft_strchr(str, '}');
		if (chr)
			chr++;
		return (chr);
	}
	chr = ft_strschr(str, " $=\"");
	if (chr)
		return (chr);
	return (ft_strchr(str, '\0'));
}

int	set_mini_paths(t_mini *mini, t_ditem *list_item)
{
	if (mini->paths)
		ft_free_double_ptr((void **)mini->paths);
	mini->paths = ft_split(((t_envar *)list_item->content)->val, ':');
	if (!mini->paths)
	{
		ft_dlstdelone(list_item, &free_envar);
		return (0);
	}
	return (1);
}

void	set_mini_status(t_mini *mini, int status_code)
{
	t_envar	*quest;

	mini->status = status_code;
	quest = get_envar(mini->envars, "?");
	if (quest)
	{
		ft_free(quest->val);
		quest->val = ft_itoa(status_code);
	}
}

int	is_valid_env_name(char *name)
{
	size_t	i;

	if (*name != '_' && !ft_isalpha(*name))
		return (0);
	i = 1;
	while (name[i])
	{
		if (name[i] != '_' && !ft_isalnum(name[i]))
			return (0);
		i++;
	}
	return (1);
}
