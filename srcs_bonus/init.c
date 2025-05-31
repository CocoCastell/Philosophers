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
		if (safe_mutex_handler(MUTEX_INIT, &philos[i].meal_time) != 0)
			return (free(philos), NULL);
	}
	return (philos);
}

int	init_semaphores(t_data *data)
{
	int	philo_nb;
	
	philo_nb = data->nb_of_philo;
	data->forks = sem_open("/forks", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, philo_nb);
	data->philo_dead = sem_open("/philo_dead", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	data->write = sem_open("/write", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
	data->end_diner = sem_open("/end_diner", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
	data->limit = sem_open("/limit", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, philo_nb / 2);
	sem_unlink("/forks");
	sem_unlink("/philo_dead");
	sem_unlink("/write");
	sem_unlink("/end_diner");
	sem_unlink("/limit");
	if (data->limit == SEM_FAILED || data->end_diner == SEM_FAILED || data->write == SEM_FAILED || data->philo_dead == SEM_FAILED || data->forks == SEM_FAILED)
		return (printf("Error: failed to create semaphore.\n"), 1);
	return (0);
}

t_data	*init_data(char *argv[], int nb_of_arg)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	data->nb_of_philo = (int)ft_atol(argv[1], NULL);
	data->time_to_die = (int)ft_atol(argv[2], NULL);
	data->time_to_eat = (int)ft_atol(argv[3], NULL);
	data->time_to_sleep = (int)ft_atol(argv[4], NULL);
	data->nb_of_meals = -1;
	if (nb_of_arg == 5)
		data->nb_of_meals = (int)ft_atol(argv[5], NULL);
	if (data->nb_of_meals == 0)
		return (free(data), printf("Error: Number of meals must be greater than 0.\n"), NULL);
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