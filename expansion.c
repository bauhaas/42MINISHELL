#include "includes/minishell.h"

/*
clang -I includes/ expansion.c srcs/expansion.c -L libft/ -lft -lncurses -o test

*/
int		main(int argc, char **argv)
{
	if (argv[1] && argv[2])
	{
		int i = ft_atoi(argv[2]);
		printf("valid_quote(%s) = %s\n",argv[1],(valid_quote(argv[1])?"ok":"KO"));
		char	*str = "dd dd  \"fdf\" \" d\'f \'\"rer";
		printf("\nis_in_quote(%s,%d) = %s\n", str, i, (is_in_quote(str,i)?"True":"False"));
		printf("quote_in_quote = %s\n", (quote_in_quote(str, i)?"True":"False"));
		// int j = 0;
		// while(argv[1][j])
		// {
		// 	if (is_in_quote(str, j))
		// 		printf("O");
		// 	else
		// 		printf(".");
		// 	j++;
		// }
		char	**words;
		printf("\n");
		words = parse(str);
		while(*words)
		{
			printf("%s\n", *words++);
		}
	}
	return (0);
}
