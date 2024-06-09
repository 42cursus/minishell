#include "test.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) != NULL)
		printf("%s\n", cwd);
	else
	{
		perror("getcwd error");
		return (1);
	}
	return (0);
}

int	main(void)
{
	ft_pwd();
}

