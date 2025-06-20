/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_tracking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/04 15:46:03 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_action(t_philo *philo, const char *msg, bool is_action)
{
	time_t	timestamp;

	safe_mutex_handler(MUTEX_LOCK, &philo->infos->print_mutex);
	timestamp = get_time_in_ms() - philo->infos->time_start;
	if (is_action == true && get_bool(&philo->infos->is_end_mutex, \
				&philo->infos->is_end_sim, NULL) == false)
		printf(GREEN"[%ld] "BL"%d "DEF"%s \n", timestamp, philo->id_philo, msg);
	else if (is_action == false)
		printf("%s\n", msg);
	safe_mutex_handler(MUTEX_UNLOCK, &philo->infos->print_mutex);
}

bool	is_full(t_philo *philo, int *error)
{
	bool	full;

	full = false;
	*error = safe_mutex_handler(MUTEX_LOCK, &philo->meals_mutex);
	if (*error != 0)
		return (true);
	if (philo->infos->nb_of_meals != -1
		&& philo->meals_eaten >= philo->infos->nb_of_meals)
	{
		full = true;
		set_int(get_int(&philo->infos->data_mutex, &philo->infos->nb_full_ph, \
			error) + 1, &philo->infos->nb_full_ph, &philo->infos->data_mutex);
	}
	*error = safe_mutex_handler(MUTEX_UNLOCK, &philo->meals_mutex);
	if (*error != 0)
		return (true);
	return (full);
}

int	check_if_dead(t_philo *philo, int *error)
{
	return (get_bool(&philo->infos->is_end_mutex, \
				&philo->infos->is_end_sim, error));
}

static bool	is_dead(t_philo *philo, int *error)
{
	time_t	timestamp;
	time_t	last_meal_time;
	int		i;

	i = -1;
	while (++i < philo->infos->nb_of_philo)
	{
		timestamp = get_time_in_ms() - philo->infos->time_start;
		last_meal_time = get_int(&philo[i].meal_time, \
				&philo[i].time_last_meal, error);
		if (*error != 0)
			return (true);
		if (timestamp - last_meal_time >= philo[i].infos->time_to_die && \
				!is_full(&philo[i], error))
		{
			safe_mutex_handler(MUTEX_LOCK, &philo->infos->print_mutex);
			printf(RED"[%ld] %d died\n"DEF, timestamp, philo->id_philo);
			if (set_bool(true, &philo->infos->is_end_sim, \
					&philo->infos->is_end_mutex) != 0)
				*error = 1;
			safe_mutex_handler(MUTEX_UNLOCK, &philo->infos->print_mutex);
			return (true);
		}
	}
	return (false);
}

void	*watchdog(void *args)
{
	t_philo	*philo;
	int		error;

	philo = (t_philo *)args;
	error = 0;
	while (get_bool(&philo->infos->is_end_mutex, &philo->infos->is_end_sim, \
				&error) == false && error == 0)
	{
		if (is_dead(philo, &error) == true)
			break ;
		ft_usleep(2, philo->infos);
	}
	return ((void *)(intptr_t)error);
}
