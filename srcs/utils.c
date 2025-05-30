/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	clear_all(t_data *data)
{
	free(data->forks);
	free(data->philos);
	free(data);
}

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < milliseconds)
		usleep(500);
}

bool get_bool(t_mutex *mutex, bool target, int *error)
{
	int tmp;
	bool value;

	tmp = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (true);
	};
	value = target;
	tmp = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (tmp!= 0)
	{
		*error = 1;
		return (true);
	};
	return (value);
}

int	set_bool(bool value, bool *target, t_mutex *mutex)
{
	int	error;

	error = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (error != 0)
		return (1);
	*target = value;
	error = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (error != 0)
		return (1);
	return (0);
}

int get_int(t_mutex *mutex, int target, int *error)
{
	int tmp;
	int value;

	tmp = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (0);
	};
	value = target;
	tmp = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (0);
	};
	return (value);
}

int	set_int(int value, int *target, t_mutex *mutex)
{
	int	error;

	error = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (error != 0)
		return (1);
	*target = value;
	error = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (error != 0)
		return (1);
	return (0);
}

time_t	get_time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}