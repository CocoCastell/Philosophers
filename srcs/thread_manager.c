/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	take_fork(t_mutex *first_fork, t_mutex *second_fork, int id_philo)
{
	int		timestamp;
	struct	timeval tv;

	timestamp = gettimeofday(&tv, NULL);
	pthread_mutex_lock(first_fork);
	printf("%d %d has taken fork\n", id_philo, timestamp);
	timestamp = gettimeofday(&tv, NULL);
	pthread_mutex_lock(second_fork);
	printf("%d %d has taken fork\n", id_philo, timestamp);
}

void	eating_routine(t_philo *philo)
{
	if (philo->id_philo % 2 == 0)
		take_fork(philo->left_fork, philo->right_fork, philo->id_philo);
	else	
		take_fork(philo->right_fork, philo->left_fork, philo->id_philo);
	printf("%d is eating\n", philo->id_philo);
	usleep(philo->infos->time_to_eat * 100);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	// philo->time_last_meal = ;
}

void	sleeping_routine(t_philo *philo)
{
	printf("%d is sleeping\n", philo->id_philo);
	usleep(philo->infos->time_to_sleep * 100);
}

void	thinking_routine(t_philo *philo)
{
	(void)philo;
}

void	*philo_thread(void *args)
{
	t_philo *philo = (t_philo*)args;

	eating_routine(philo);
	sleeping_routine(philo);
	thinking_routine(philo);
	
	// print_single_philo(philo);
	return (NULL);
}

int	thread_manager(t_data *philo_data)
{
	pthread_t	*threads;
	int		i;

	i = 0;
	threads = malloc(sizeof(pthread_t) * philo_data->nb_of_philo);
	if (threads == NULL)
		return (perror("Memory allocation failed"), 1);
	while (i < philo_data->nb_of_philo)
	{
		if (pthread_create(&threads[i], NULL, philo_thread, &philo_data->philos[i]) != 0)
			return (perror("Creating thread failed"), 1);
		i++;
	}
	i = 0;
	while (i < philo_data->nb_of_philo)
	{
		if (pthread_join(threads[i], NULL) != 0)
			return (perror("Joining thread failed"), 1);
		i++;
	}
	destroy_mutex(philo_data->forks, philo_data->nb_of_philo);
	free(threads);
	return (0);
}
