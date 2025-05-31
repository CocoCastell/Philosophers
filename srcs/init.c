/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_meal_time(t_philo *philo)
{
	return (set_int(get_time_in_ms() - philo->infos->time_start, &philo->time_last_meal, &philo->meal_time));
}

t_philo	*init_philo(int nb_of_philo, t_mutex *forks, t_data *infos)
{
	t_philo	*philos;
	int	i;

	philos = malloc(sizeof(t_philo) * nb_of_philo);
	if (philos == NULL)
		return (printf("Memory allocation failed"), NULL);
	i = -1;
	while (++i < nb_of_philo)
	{
		philos[i].id_philo = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].time_last_meal = 0;
		philos[i].infos = infos;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % nb_of_philo];
		if (safe_mutex_handler(MUTEX_INIT, &philos[i].meals_mutex) != 0 || safe_mutex_handler(MUTEX_INIT, &philos[i].meal_time) != 0)
			return (NULL);
	}
	return (philos);
}

t_mutex	*init_fork(int nb_of_forks)
{
	t_mutex	*forks;
	int	i;

	forks = malloc(sizeof(t_mutex) * nb_of_forks);
	if (forks == NULL)
		return (printf("Memory allocation failed"), NULL);
	i = -1;
	while (++i < nb_of_forks)
	{
		if (safe_mutex_handler(MUTEX_INIT, &forks[i]) != 0)
			return (NULL);
	}
	return (forks);
}

t_data	*init_data(char *argv[], int nb_of_arg)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	if (safe_mutex_handler(MUTEX_INIT, &data->is_end_mutex) != 0 || safe_mutex_handler(MUTEX_INIT, &data->print_mutex) != 0 || safe_mutex_handler(MUTEX_INIT, &data->data_mutex) != 0)
		return (NULL);
	data->nb_of_philo = (int)ft_atol(argv[1], NULL);
	data->time_to_die = (int)ft_atol(argv[2], NULL);
	data->time_to_eat = (int)ft_atol(argv[3], NULL);
	data->time_to_sleep = (int)ft_atol(argv[4], NULL);
	data->nb_of_full_philo = 0;
	data->is_end_sim = false;
	data->nb_of_meals = -1;
	if (nb_of_arg == 5)
		data->nb_of_meals = (int)ft_atol(argv[5], NULL);
	if (data->nb_of_meals == 0)
		return (printf("Error: Number of meals must be greater than 0.\n"), NULL);
	data->forks = init_fork(data->nb_of_philo);
	if (data->forks == NULL)
		return (NULL);
	data->philos = init_philo(data->nb_of_philo, data->forks, data);
	if (data->philos == NULL)
		return (NULL);
	return (data);
}

