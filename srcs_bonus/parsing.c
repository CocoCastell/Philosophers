/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/03 11:53:40 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

long	ft_atol(const char *str, int *error)
{
	long	result;
	int		sign;

	sign = 1;
	result = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		if (error != NULL)
			*error = 0;
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

int	parse_args(char *argv[])
{
	int		i;
	int		error;
	long	nb;

	i = 1;
	nb = 0;
	while (argv[i] != NULL)
	{
		error = 1;
		nb = ft_atol(argv[i], &error);
		if (error == 1)
			return (printf("Error: numeric argument required.\n"), 1);
		if (nb > INT_MAX || nb < INT_MIN)
			return (printf("Error: valid 32-bit integer required.\n"), 1);
		if (nb < 0)
			return (printf("Error: positive integer required.\n"), 1);
		i++;
	}
	return (0);
}
