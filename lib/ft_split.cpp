#include "lib.hpp"

static int	num_words(char const *s, char c)
{
	int	num;
	int i;

	num = 0;
	i = 0;
	while (*(s + i))
	{
		if (!(*(s + i) == c) && ((*(s + i - 1) == c) || i == 0))
			num++;
		i++;
	}
	return (num);
}

static int	word_len(char const *str, char c)
{
	int i;

	i = 0;
	while (*(str + i) && *(str + i) != c)
		i++;
	return (i);
}

static void	ft_strncpy(char *dest, char const *str, int n)
{
	int i;
	
	i = -1;
	while (++i < n)
		*(dest + i) = *(str + i);
	*(dest + i) = 0;
}

static char	**free_all(char **str_set, int n)
{
	int i;

	i = -1;
	while (++i < n)
		free(*(str_set + i));
	free(str_set);
	return (0);
}

char		**ft_split(char const *s, char c)
{
	char	**str_set;
	int		i;
	int		j;

	if ((str_set = (char **)malloc(sizeof(char *) * (num_words(s, c) + 1))) == 0)
		return (0);
	i = 0;
	j = 0;
	while (*(s + i))
	{
		if (!(*(s + i) == c) && ((*(s + i - 1) == c) || i == 0))
		{
			if ((*(str_set + j) = \
						(char *)malloc(sizeof(char) * (word_len(s + i, c) + 1))) == 0)
				return (free_all(str_set, j));
			ft_strncpy(*(str_set + j), (s + i), word_len(s + i, c));
			j++;
		}
		i++;
	}
	*(str_set + j) = 0;
	return (str_set);
}