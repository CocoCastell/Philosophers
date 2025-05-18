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

t_data	*init_data(char *argv[], int nb_of_arg)
{
	t_data	*philo_data;

	philo_data = malloc(sizeof(t_data));
	if (philo_data == NULL)
		return (NULL);
	philo_data->nb_of_philo = (int)ft_atol(argv[1], NULL);
	philo_data->time_to_die = (int)ft_atol(argv[2], NULL);
	philo_data->time_to_eat = (int)ft_atol(argv[3], NULL);
	philo_data->time_to_sleep = (int)ft_atol(argv[4], NULL);
	if (nb_of_arg == 5)
		philo_data->nb_of_meals = (int)ft_atol(argv[5], NULL);
	philo_data->forks = NULL; 
	philo_data->philos = NULL;
	return (philo_data);
}

int	main(int argc, char *argv[])
{
	t_data	*philo_data;

	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments.\n"), 1);
	if (parse_args(argv) == 1)
		return (1);
	philo_data = init_data(argv, --argc);
	thread_manager(philo_data);
	return (0);
}
