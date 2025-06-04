/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/04 15:41:58 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	take_fork(t_mutex *first_fork, t_mutex *second_fork, t_philo *philo)
{
	if (safe_mutex_handler(MUTEX_LOCK, first_fork) != 0)
		return (1);
	print_action(philo, "has taken a fork", true);
	if (safe_mutex_handler(MUTEX_LOCK, second_fork) != 0)
		return (safe_mutex_handler(MUTEX_UNLOCK, first_fork), 1);
	print_action(philo, "has taken a fork", true);
	return (0);
}

void	ft_usleep(size_t milliseconds, t_data *data)
{
	size_t	start;
	int		error;

	start = get_time_in_ms();
	while (get_bool(&data->is_end_mutex, &data->is_end_sim, &error) == false)
	{
		if (get_time_in_ms() - start >= milliseconds)
			return ;
		usleep(500);
	}
}

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
