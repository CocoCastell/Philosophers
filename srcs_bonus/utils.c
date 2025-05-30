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

#include "../includes/philo_bonus.h"

void	clear_all(t_data *data)
{
	sem_unlink("/philo_dead");
	sem_unlink("/forks");
	sem_unlink("/write");
	sem_unlink("/end_diner");
	free(data->philos);
	free(data->pid);
	free(data);
	exit(0);
}

void	clear_process(t_data *data)
{
	sem_close(data->philo_dead);
	sem_close(data->forks);
	sem_close(data->write);
	sem_close(data->end_diner);
	free(data->philos);
	free(data->pid);
	free(data);
	exit(0);
}

void    set_long(long value, long *target, t_mutex *mutex)
{
	safe_mutex_handler(MUTEX_LOCK, mutex);
	*target = value;
	safe_mutex_handler(MUTEX_UNLOCK, mutex);
}

time_t get_long(t_mutex *mutex, time_t *target)
{
	time_t value;

	safe_mutex_handler(MUTEX_LOCK, mutex);
	value = *target;
	safe_mutex_handler(MUTEX_UNLOCK, mutex);
	return (value);
}

time_t	get_time_in_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}