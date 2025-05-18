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

void	*philo_thread(void *args)
{
	return ((void)args, NULL);
}

void	thread_manager(t_data *philo_data)
{
	pthread_t	*threads;
	int		i;

	i = 0;
	threads = malloc(sizeof(philo_data->nb_of_philo));
	while (i < philo_data->nb_of_philo)
	{
		if (pthread_create(&threads[i], NULL, philo_thread, philo_data) != 0)
			printf("Error: creating thread failed.\n");
		i++;
	}
	i = 0;
	while (i < philo_data->nb_of_philo)
	{
		if (pthread_join(threads[i], NULL) != 0)
			printf("Error: joining thread failed.\n");
		i++;
	}
}
