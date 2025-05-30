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

#include "../includes/philo_bonus.h"

void	init_meal_time(t_philo *philo)
{
	return (set_long(get_time_in_ms() - philo->infos->time_start, &philo->time_last_meal, &philo->meal_time));
}

t_philo	*init_philo(int nb_of_philo, t_data *infos)
{
	t_philo	*philos;
	int			i;

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
	}
	return (philos);
}

int	init_semaphores(t_data *data)
{
	int	philo_nb;

	philo_nb = data->nb_of_philo;
	sem_unlink("/forks");
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, philo_nb);
	if (data->forks == SEM_FAILED)
			return (printf("Error: failed to create semaphore for forks.\n"), 1);
	sem_unlink("/philo_dead"); //inutile ?
	data->philo_dead = sem_open("/philo_dead", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	if (data->philo_dead == SEM_FAILED)
		return (printf("Error: failed to create semaphore for dead philo.\n"), 1);
	sem_unlink("/write");
	data->write = sem_open("/write", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	if (data->write == SEM_FAILED)
		return (printf("Error: failed to create semaphore for write.\n"), 1);
	sem_unlink("/end_diner");
	data->end_diner = sem_open("/end_diner", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
	if (data->end_diner == SEM_FAILED)
		return (printf("Error: failed to create semaphore for end diner.\n"), 1);
	return (0);
}

t_data	*init_data(char *argv[], int nb_of_arg)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	if (safe_mutex_handler(MUTEX_INIT, &data->is_end_mutex) != 0)
		return (free(data), NULL);
	data->nb_of_philo = (int)ft_atol(argv[1], NULL);
	data->time_to_die = (int)ft_atol(argv[2], NULL);
	data->time_to_eat = (int)ft_atol(argv[3], NULL);
	data->time_to_sleep = (int)ft_atol(argv[4], NULL);
	data->nb_of_meals = -1;
	if (nb_of_arg == 5)
		data->nb_of_meals = (int)ft_atol(argv[5], NULL);
	data->pid = malloc(sizeof(pid_t) * data->nb_of_philo);
	if (data->pid == NULL )
		return (free(data), NULL);
	if (init_semaphores(data) != 0)
		return (free(data), NULL); //free sem
	data->philos = init_philo(data->nb_of_philo, data);
	if (data->philos == NULL)
		return (free(data->pid), free(data), NULL);
	return (data);
}