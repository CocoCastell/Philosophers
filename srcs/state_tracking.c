/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_tracking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/philo.h"

void    print_action(t_philo *philo, const char *message, bool is_action)
{
    time_t  timestamp;

    safe_mutex_handler(MUTEX_LOCK, &philo->infos->print_mutex);
    timestamp = get_time_in_ms() - philo->infos->time_start;
    if (is_action == true && get_bool(&philo->infos->is_end_mutex, &philo->infos->is_end_sim, NULL) == false)
        printf(GREEN"[%ld] "BL"%d "DEF"%s \n", timestamp, philo->id_philo, message);
    else if (is_action == false)
        printf("%s\n", message);
    safe_mutex_handler(MUTEX_UNLOCK, &philo->infos->print_mutex);
}

bool	is_full(t_philo *philo, int *error)
{
	bool    full;
	
	full = false;
	*error = safe_mutex_handler(MUTEX_LOCK, &philo->meals_mutex);
	if (*error != 0)
		return (true);
	if (philo->infos->nb_of_meals != -1
		&& philo->meals_eaten >= philo->infos->nb_of_meals)
    {
		full = true;
        set_int(philo->infos->nb_of_full_philo + 1, &philo->infos->nb_of_full_philo, &philo->infos->data_mutex);
    }
	*error = safe_mutex_handler(MUTEX_UNLOCK, &philo->meals_mutex);
	if (*error != 0)
		return (true);
	return (full);
}

int check_if_dead(t_philo *philo, int *error)
{
    return (get_bool(&philo->infos->is_end_mutex, &philo->infos->is_end_sim, error));
}

static bool is_dead(t_philo *philo, int *error)
{
    time_t  timestamp;
    time_t  last_meal_time;
    int     i;
    
    i = -1;
    while (++i < philo->infos->nb_of_philo)
    {
        timestamp = get_time_in_ms() - philo->infos->time_start;
        last_meal_time = get_int(&philo[i].meal_time, &philo[i].time_last_meal, error);
        if (*error != 0)
            return (true);
        if (timestamp - last_meal_time >= philo[i].infos->time_to_die && !is_full(&philo[i], error))
        {
            print_action(&philo[i], RED"died"DEF, true);
            if (set_bool(true, &philo->infos->is_end_sim, &philo->infos->is_end_mutex) != 0)
                *error = 1;
            return (true);
        }
    }
    return (false);
}

void    *state_tracker(void *args)
{
    t_philo *philo;
    int     error;

    philo = (t_philo *)args;
    error = 0;
    synchronise_threads(philo->infos);
    while (get_bool(&philo->infos->is_end_mutex, &philo->infos->is_end_sim, &error) == false && error == 0)
    {
        if (is_dead(philo, &error) == true)
            break ;
        ft_usleep(2, philo->infos);
    }
    return ((void *)(intptr_t)error);
}