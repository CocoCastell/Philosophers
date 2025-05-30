/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	take_fork(t_mutex *first_fork, t_mutex *second_fork, t_philo *philo)
{
	time_t	timestamp;

	if (safe_mutex_handler(MUTEX_LOCK, first_fork) != 0)
		return (1); 
	timestamp = get_time_in_ms() - philo->infos->time_start;
	printf("%ld %d has taken a fork\n", timestamp, philo->id_philo);
	timestamp = get_time_in_ms() - philo->infos->time_start;
	if (safe_mutex_handler(MUTEX_LOCK, second_fork) != 0)
		return (safe_mutex_handler(MUTEX_UNLOCK, first_fork), 1);
	printf("%ld %d has taken a fork\n", timestamp, philo->id_philo);
	return (0);
}

int	destroy_forks(t_mutex *forks, int nb_of_forks)
{
	int	i;

	i = -1;
	while (++i < nb_of_forks)
	{
		if (safe_mutex_handler(MUTEX_DESTROY, &forks[i]) != 0)
			return (1);
	}
	return (0);
}

void	synchronise_threads(t_data *data)
{
	while (data->time_start > get_time_in_ms())
		continue ;
}

void	de_synchronise_threads(t_philo *philo)
{
	if (philo->infos->nb_of_philo % 2 == 0)
	{
		if (philo->id_philo % 2 == 0)
			ft_usleep(30);
	}
	else
	{
		if (philo->id_philo % 2)
			thinking_routine(philo);
	}
}

void	one_philo_routine(t_data *data)
{
	t_philo	*philo;

	philo = &data->philos[0];
	safe_mutex_handler(MUTEX_LOCK, philo->left_fork);
	printf("%d %d has taken a fork\n", 0, philo->id_philo);
	ft_usleep(data->time_to_die);
  printf(RED"%d %d died\n"DEF, data->time_to_die , philo->id_philo);
	safe_mutex_handler(MUTEX_UNLOCK, philo->left_fork);
}