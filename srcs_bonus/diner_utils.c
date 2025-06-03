/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/03 12:04:32 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < milliseconds)
		usleep(500);
}

void	wait_philos(t_data *data)
{
	int	i;
	int	status;

	i = -1;
	while (++i < data->nb_of_philo)
		waitpid(data->pid[i], &status, 0);
}

void	kill_all(t_data *data, int limit)
{
	int	i;

	i = -1;
	while (++i < limit)
	{
		if (data->pid[i] > 0)
		{
			kill(data->pid[i], SIGKILL);
			waitpid(data->pid[i], NULL, 0);
		}
	}
}

void	*kill_philos(void *args)
{
	int		i;
	t_philo	*philo;

	i = -1;
	philo = (t_philo *)args;
	sem_wait(philo->infos->end_diner);
	while (++i < philo->infos->nb_of_philo)
	{
		if (philo->infos->pid[i] > 0)
		{
			kill(philo->infos->pid[i], SIGKILL);
			waitpid(philo->infos->pid[i], NULL, 0);
		}
	}
	return (NULL);
}

void	print_action(t_philo *philo, char *action)
{
	time_t	timestamp;

	sem_wait(philo->infos->write);
	timestamp = get_time_in_ms() - philo->time_start;
	printf(GREEN"[%ld] "BL"%d "DEF"%s \n", timestamp, philo->id_philo, action);
	sem_post(philo->infos->write);
}
