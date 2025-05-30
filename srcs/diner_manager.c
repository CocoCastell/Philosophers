/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	eating_routine(t_philo *philo)
{
	time_t	timestamp;
	int			error;

	error = 0;
	if (philo->id_philo % 2 == 0 && take_fork(philo->left_fork, philo->right_fork, philo) != 0)
		return (1);
	else if (philo->id_philo % 2 != 0 && take_fork(philo->right_fork, philo->left_fork, philo) != 0)
		return (1);
	if (check_if_dead(philo, &error) != 0)
	{
		return (safe_mutex_handler(MUTEX_UNLOCK, philo->left_fork) || safe_mutex_handler(MUTEX_UNLOCK, philo->right_fork) || error);
	}
	timestamp = get_time_in_ms() - philo->infos->time_start;
	philo->time_last_meal = timestamp;// mettre mutex
	printf(BL"%ld %d is eating\n"DEF, timestamp, philo->id_philo);
	ft_usleep(philo->infos->time_to_eat);
	set_int(philo->meals_eaten + 1, &philo->meals_eaten, &philo->meals_mutex);
	return (safe_mutex_handler(MUTEX_UNLOCK, philo->left_fork) || safe_mutex_handler(MUTEX_UNLOCK, philo->right_fork));
}

void	sleeping_routine(t_philo *philo)
{
	time_t	timestamp;

	timestamp = get_time_in_ms() - philo->infos->time_start;
	printf(GREEN"%ld %d is sleeping\n"DEF, timestamp ,philo->id_philo);
	ft_usleep(philo->infos->time_to_sleep);
}

void	thinking_routine(t_philo *philo)
{
	time_t	time_to_think;
	time_t	timestamp;

	timestamp = get_time_in_ms() - philo->infos->time_start;
	printf(YELLOW"%ld %d is thinking\n"DEF, timestamp ,philo->id_philo);
	if (philo->infos->nb_of_philo % 2 == 0)
		return ;
	time_to_think = philo->infos->time_to_eat * 2 - philo->infos->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think * 0.3);
}

void	*philo_routine(void *args)
{
	t_philo *philo;
	int			error;

	error = 0;
	philo = (t_philo*)args;
	if (init_meal_time(philo) != 0)
		error = 1;
	synchronise_threads(philo->infos);
	de_synchronise_threads(philo);
	while (check_if_dead(philo, &error) == false && error != 1)
	{
		if (eating_routine(philo) == 1)
		{
			error = 1;
			break ;
		}
		if (is_full(philo, &error) == true)
			break ;
		if (check_if_dead(philo, &error) != 0)
			return ((void *)(intptr_t)error);
		sleeping_routine(philo);
		if (check_if_dead(philo, &error) != 0)
			return ((void *)(intptr_t)error);
		thinking_routine (philo);
	}
	return ((void *)(intptr_t)error);
}

int	diner_table(t_data *data)
{
	pthread_t	th_state;
	int				th_created;
	int				error[2];

	error[0] = 0;
	error[1] = 0;
	th_created = -1;
	if (safe_thread_handler(PTHREAD_CREATE, &th_state, data->philos, state_tracker) != 0)
		return (1);
	data->time_start = get_time_in_ms() + (data->nb_of_philo * 8 / 5);
	while (++th_created < data->nb_of_philo)
	{
		error[0] = safe_thread_handler(PTHREAD_CREATE, &data->philos[th_created].thread, &data->philos[th_created], philo_routine);
		if (error[0] == 1)
			break ;
	}
	while (--th_created >= 0)
	{
		error[1] = safe_thread_handler(PTHREAD_JOIN, &data->philos[th_created].thread, &data->philos[th_created], philo_routine);
		if (error[1] == 1)
			break ;
	}
	set_bool(true, &data->is_end_sim, &data->is_end_mutex);
	pthread_join(th_state, NULL);
	return (destroy_forks(data->forks, data->nb_of_philo) || error[0] || error[1]);
}