/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void print_single_philo(t_philo *philos)
{
	int i = 0;

	printf(RED"   philo data\n"DEF);
	while (i < philos->infos->nb_of_philo)
	{
		printf("	philo id: %d\n", philos->id_philo);
		printf("	nb of meals eaten: %d\n", philos->meals_eaten);
		printf("	time last meal: %d\n", philos->time_last_meal);
		printf("\n");
		i++;
	}
}

void print_all_philo(t_philo *philos)
{
	int i = 0;

	printf(RED"   philo data\n"DEF);
	while (i < philos->infos->nb_of_philo)
	{
		printf("	philo id: %d\n", philos[i].id_philo);
		printf("	nb of meals eaten: %d\n", philos[i].meals_eaten);
		printf("	time last meal: %d\n", philos[i].time_last_meal);
		printf("\n");
		i++;
	}
}

void print_info(t_data *data)
{
	printf(GREEN"General data\n"DEF);
	printf(" - Nb of philos: %d\n", data->nb_of_philo);
	printf(" - Time start: %ld\n", data->time_start);
	printf(" - Time to die: %d\n", data->time_to_die);
	printf(" - Time to eat: %d\n", data->time_to_eat);
	printf(" - Time to sleep: %d\n", data->time_to_sleep);
	printf(" - Nb of meals: %d\n", data->nb_of_meals);
	printf("\n");
	// print_all_philo(data->philos);	
}

t_philo	*init_philo(int nb_of_philo, t_mutex *forks, t_data *infos)
{
	t_philo	*philos;
	int	i;

	philos = malloc(sizeof(t_philo) * nb_of_philo);
	if (philos == NULL)
		return (perror("Memory allocation failed"), NULL);
	i = 0;
	while (i < nb_of_philo)
	{
		philos[i].id_philo = i + 1;
		philos[i].meals_eaten = 0;
		philos[i].time_last_meal = 0;
		philos[i].infos = infos;
		philos[i].left_fork = &forks[i];
		if (i == nb_of_philo - 1)
			philos[i].right_fork = &forks[0];
		else
			philos[i].right_fork = &forks[i + 1];
		i++;
	}
	return (philos);
}

t_mutex	*init_fork(int nb_of_forks)
{
	t_mutex	*forks;
	int	i;

	forks = malloc(sizeof(t_mutex) * nb_of_forks);
	if (forks == NULL)
		return (perror("Memory allocation failed"), NULL);
	i = 0;
	while (i < nb_of_forks)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
			return (perror("Mutex initialization failed"), NULL);
		i++;
	}
	return (forks);
}

t_data	*init_data(char *argv[], int nb_of_arg)
{
	t_data	*philo_data;

	philo_data = malloc(sizeof(t_data));
	if (philo_data == NULL)
		return (NULL);
	philo_data->time_start = get_time_in_ms();
	philo_data->nb_of_philo = (int)ft_atol(argv[1], NULL);
	philo_data->time_to_die = (int)ft_atol(argv[2], NULL);
	philo_data->time_to_eat = (int)ft_atol(argv[3], NULL);
	philo_data->time_to_sleep = (int)ft_atol(argv[4], NULL);
	if (nb_of_arg == 5)
		philo_data->nb_of_meals = (int)ft_atol(argv[5], NULL);
	philo_data->forks = init_fork(philo_data->nb_of_philo);
	if (philo_data->forks == NULL)
		return (free(philo_data), NULL);
	philo_data->philos = init_philo(philo_data->nb_of_philo, philo_data->forks, philo_data);
	if (philo_data->philos == NULL)
		return (free(philo_data->forks), free(philo_data), NULL);
	return (philo_data);
}

void	clear_all(t_data *philo_data)
{
	free(philo_data->forks);
	free(philo_data->philos);
	free(philo_data);
}

int	main(int argc, char *argv[])
{
	t_data	*philo_data;

	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments.\n"), 1);
	if (parse_args(argv) == 1)
		return (2);
	philo_data = init_data(argv, --argc);
	if (philo_data == NULL)
		return (3);
	// print_info(philo_data);
	if (thread_manager(philo_data) != 0)
		return (clear_all(philo_data), 4);
	clear_all(philo_data);
	return (EXIT_SUCCESS);
}
