/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void    eating_routine(t_philo *philo)
{
    sem_wait(philo->infos->forks);
    print_action(philo, FORK);
    sem_wait(philo->infos->forks);
    print_action(philo, FORK);
    // sem_wait(philo->infos->philo_dead);
    print_action(philo, EAT);
    // philo->time_last_meal = get_time_in_ms() - philo->infos->time_start;
    // sem_post(philo->infos->philo_dead);
    set_long(get_time_in_ms(), &philo->time_last_meal, &philo->meal_time);
    // printf("last meal: %ld\n", philo->time_last_meal);
    ft_usleep(philo->infos->time_to_eat);
    sem_post(philo->infos->forks);
    sem_post(philo->infos->forks);
    philo->meals_eaten++;
}

void    sleeping_routine(t_philo *philo)
{
    print_action(philo, SLEEP);
	  ft_usleep(philo->infos->time_to_sleep);
}

void    thinking_routine(t_philo *philo)
{
    print_action(philo, THINK);
}

void	de_synchronise_threads(t_philo *philo)
{
	if (philo->infos->nb_of_philo % 2 == 0)
	{
		if (philo->id_philo % 2 == 0)
			ft_usleep(30);
	}
	else
	{
		if (philo->id_philo % 2)
			thinking_routine(philo);
	}
}

void    philo_routine(t_philo *philo)
{
    pthread_t death_thread;

    // init_meal_time(philo);
    philo->time_last_meal = get_time_in_ms(); //- philo->infos->time_start;
    // synchronise_processes(philo->infos);
    // de_synchronise_threads(philo);
    safe_thread_handler(PTHREAD_CREATE, &death_thread, philo, is_dead);
    safe_thread_handler(PTHREAD_DETACH, &death_thread, NULL, NULL);
    while (true)
    {
        eating_routine(philo);
        if (philo->infos->nb_of_meals != -1 && philo->meals_eaten >= philo->infos->nb_of_meals)
            clear_process(philo->infos);
        sleeping_routine(philo);
        thinking_routine(philo);
    }
}

int diner_table(t_data *data)
{
  int i;
  pthread_t kill_thread;
  i = 0;
 
  data->time_start = get_time_in_ms();
	// data->time_start = get_time_in_ms() + (data->nb_of_philo * 8 / 5);
  safe_thread_handler(PTHREAD_CREATE, &kill_thread, data->philos, kill_philos);
  while (i < data->nb_of_philo)
  {
    data->pid[i] = fork();
    if (data->pid[i] == 0)
      philo_routine(&data->philos[i]);
    else if (data->pid[i] < 0)
      printf("Fork failed");
    i++;
  }
  wait_philos(data);
  return (0);
}