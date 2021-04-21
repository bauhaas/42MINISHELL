#include "../includes/minishell.h"

static int			escaped(char *str, int pos)
{
	if (pos > 0 && str[pos - 1] == '\\')
		return (TRUE);
	return (FALSE);
}

t_list				*parse_bloc(char *str, t_ms *ms)
{
	t_list			*block;
	int				i;
	char			*word;

	i = 0;
	word = NULL;
	block = NULL;
	while (str[i])
	{
		if (str[i] != ';' || valid_quotes(str, i) || (str[i] == ';' && escaped(str, i)))
			word = ft_add_char(word, str[i]);
		else
		{
			ft_lstadd_back(&block, ft_lstnew(ft_strdup(word)));
			ft_strdel(&word);
		}
		i++;
	}
	if (word)
	{
		ft_lstadd_back(&block, ft_lstnew(ft_strdup(word)));
		ft_strdel(&word);		
	}
	return(block);
}
