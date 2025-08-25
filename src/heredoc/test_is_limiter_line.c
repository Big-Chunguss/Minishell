#include "../../includes/minishell.h"

int	is_limiter_line(char *line, char *limiter)
{
	return (ft_strcmp(line, limiter) == 0);
}
