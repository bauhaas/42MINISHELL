#include "includes/minishell.h"

/*
clang -I includes/ expansion.c srcs/expansion.c -L libft/ -lft -lncurses -o test

*/
int		main(int argc, char **argv)
{

	if (argv[1] && argv[2])
	{
		int i = ft_atoi(argv[2]);

		printf("\nis_in_quotes(%s,%d) = %s\n", argv[1], i, (is_in_quotes(argv[1],i)?"True":"False"));
		//printf("quote_in_quotes = %s\n", (quote_in_quotes(str, i)?"True":"False"));

		char	**words;

		words = parse(argv[1]);
		while(*words)
		{
			printf("<%s>\n", *words++);
		}
	}
	else if (argv[1])
	{
		//int i = ft_atoi(argv[1]);
		int i = 0;
		char	*str = "dd dd  \"fdf\" \" d\'f \'\\\" coucou \\\"\" rer \\ \' df \\\'\'";
		//char *str = "\"  e \"";
		printf("valid_quotes(%s) = %s\n",str,(valid_quotes(str)?"ok":"KO"));
		while (str[i])
		{
			is_in_quotes(str,i)? printf("0"):printf(".");
			i++;
		}
		//printf("\nis_in_quotes(%s,%d) = %s\n", str, i, (is_in_quotes(str,i)?"True":"False"));
	}
	else if (argc == 1)
	{
		int i = 0;
		char *str = "echo \"essais\"";
		while (str[i])
		{
			if (quote_in_quotes(str, i))
				printf("\"");
			else
				printf("_");
			i++;
		}
		//printf("quote_in_quotes = %s\n", (quote_in_quotes(str, i)?"True":"False"));
	}

	return (0);
}
