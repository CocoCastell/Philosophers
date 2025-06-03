/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/03 16:36:10 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	exit_error(t_philo *philo, const char *message)
{
	if (message != NULL)
	{
		sem_wait(philo->infos->write);
		printf("%s\n", message);
		sem_post(philo->infos->write);
	}
	clear_all(philo->infos, 1);
	exit(EXIT_FAILURE);
}

void	clear_all(t_data *data, int flag)
{
	int	i;

	i = -1;
	sem_close(data->philo_dead);
	sem_close(data->forks);
	sem_close(data->write);
	sem_close(data->end_diner);
	sem_close(data->limit);
	if (flag == 0)
	{
		while (++i < data->nb_of_philo)
			safe_mutex_handler(MUTEX_DESTROY, &data->philos[i].meal_time);
	}
	free(data->philos);
	free(data->pid);
	free(data);
}

int	set_long(long value, long *target, t_mutex *mutex)
{
	if (safe_mutex_handler(MUTEX_LOCK, mutex) != 0)
		return (1);
	*target = value;
	if (safe_mutex_handler(MUTEX_UNLOCK, mutex) != 0)
		return (1);
	return (0);
}

time_t	get_long(t_mutex *mutex, time_t *target)
{
	time_t	value;

	safe_mutex_handler(MUTEX_LOCK, mutex);
	value = *target;
	safe_mutex_handler(MUTEX_UNLOCK, mutex);
	return (value);
}

time_t	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
