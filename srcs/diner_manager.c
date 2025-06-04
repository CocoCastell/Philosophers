/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/04 15:43:16 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	eating_routine(t_philo *philo)
{
	time_t	timestamp;

	if (philo->id_philo % 2 == 0 && take_fork(philo->left_fork, \
				philo->right_fork, philo) != 0)
		return (1);
	else if (philo->id_philo % 2 != 0 && take_fork(philo->right_fork, \
				philo->left_fork, philo) != 0)
		return (1);
	timestamp = get_time_in_ms() - philo->infos->time_start;
	set_int(timestamp, &philo->time_last_meal, &philo->meal_time);
	print_action(philo, CYAN"is eating"DEF, true);
	ft_usleep(philo->infos->time_to_eat, philo->infos);
	set_int(philo->meals_eaten + 1, &philo->meals_eaten, &philo->meals_mutex);
	return (safe_mutex_handler(MUTEX_UNLOCK, philo->left_fork) || \
			safe_mutex_handler(MUTEX_UNLOCK, philo->right_fork));
}

void	sleeping_routine(t_philo *philo)
{
	print_action(philo, PURPLE"is sleeping"DEF, true);
	ft_usleep(philo->infos->time_to_sleep, philo->infos);
}

void	thinking_routine(t_philo *philo)
{
	time_t	time_to_think;

	print_action(philo, YELLOW"is thinking"DEF, true);
	if (philo->infos->nb_of_philo % 2 == 0)
		return ;
	time_to_think = philo->infos->time_to_eat * 2 - philo->infos->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think * 0.4, philo->infos);
}

void	*philo_routine(void *args)
{
	t_philo	*philo;
	int		error;

	error = 0;
	philo = (t_philo *)args;
	if (init_meal_time(philo) != 0)
		error = 1;
	while (check_if_dead(philo, &error) == false && error != 1)
	{
		if (eating_routine(philo) == 1)
		{
			error = 1;
			break ;
		}
		if (is_full(philo, &error) == true)
			break ;
		sleeping_routine(philo);
		thinking_routine (philo);
	}
	return ((void *)(intptr_t)error);
}

int	diner_table(t_data *data, int t)
{
	pthread_t	th_state;
	int			error[2];

	error[0] = 0;
	error[1] = 0;
	data->time_start = get_time_in_ms();
	while (++t < data->nb_of_philo)
	{
		error[0] = safe_th_handle(PTHREAD_CREATE, &data->philos[t].thread, \
				&data->philos[t], philo_routine);
		if (error[0] == 1)
			break ;
		usleep(50);
	}
	if (safe_th_handle(PTHREAD_CREATE, &th_state, data->philos, watchdog) != 0)
		return (1);
	while (--t >= 0)
	{
		error[1] = safe_th_handle(PTHREAD_JOIN, &data->philos[t].thread \
				, &data->philos[t], philo_routine);
		if (error[1] == 1)
			break ;
	}
	set_bool(true, &data->is_end_sim, &data->is_end_mutex);
	return (pthread_join(th_state, NULL) || error[0] || error[1]);
}
