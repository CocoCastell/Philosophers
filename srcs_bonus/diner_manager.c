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

void    *is_dead(void *args)
{
    time_t  time;
    time_t   last_meal;
    t_philo *philo;

    philo = (t_philo *)args;
    while (true)
    {
        time = get_time_in_ms();
        last_meal = get_long(&philo->meal_time, &philo->time_last_meal);
        if (time - last_meal > philo->infos->time_to_die)
        {
            sem_wait(philo->infos->philo_dead);
            print_action(philo, RED"died"DEF);
	          sem_post(philo->infos->end_diner);
            clear_all(philo->infos);
            exit(0);
        }
        ft_usleep(2);
    }
    return (NULL);
}

void    eating_routine(t_philo *philo)
{
    sem_wait(philo->infos->limit);   
    sem_wait(philo->infos->forks);
    print_action(philo, "has taken a fork");
    sem_wait(philo->infos->forks);
    print_action(philo, "has taken a fork");
    sem_post(philo->infos->limit);   
    sem_wait(philo->infos->philo_dead);
    print_action(philo, CYAN"is eating"DEF);
    sem_post(philo->infos->philo_dead);
    if (set_long(get_time_in_ms(), &philo->time_last_meal, &philo->meal_time) != 0)
        exit_error(philo, NULL);
    ft_usleep(philo->infos->time_to_eat);
    sem_post(philo->infos->forks);
    sem_post(philo->infos->forks);
    philo->meals_eaten++;
}

void    sleeping_thinking_routine(t_philo *philo)
{
    print_action(philo, PURPLE"is sleeping"DEF);
	  ft_usleep(philo->infos->time_to_sleep);
    print_action(philo, YELLOW"is thinking"DEF);
}

void    philo_routine(t_philo *philo)
{
    pthread_t death_thread;

    philo->time_start = get_time_in_ms();
    philo->time_last_meal = get_time_in_ms();
    if (safe_thread_handler(PTHREAD_CREATE, &death_thread, philo, is_dead) == 1)
        exit_error(philo, "Error creating death thread");
    if (safe_thread_handler(PTHREAD_DETACH, &death_thread, NULL, NULL) == 1)
        exit_error(philo, "Error detaching death thread");
    while (true)
    {
        eating_routine(philo);
        if (philo->infos->nb_of_meals != -1 && philo->meals_eaten >= philo->infos->nb_of_meals)
        {
          clear_all(philo->infos);
          exit(0);
        }
        sleeping_thinking_routine(philo);
    }
}

int diner_table(t_data *data)
{
  int       i;
  pthread_t kill_thread;
  
  i = -1;
  if (safe_thread_handler(PTHREAD_CREATE, &kill_thread, data->philos, kill_philos) == 1)
    return (1);
  if (safe_thread_handler(PTHREAD_DETACH, &kill_thread, NULL, NULL) == 1)
    return (1);
  while (++i < data->nb_of_philo)
  {
    data->pid[i] = fork();
    if (data->pid[i] == 0)
      philo_routine(&data->philos[i]);
    else if (data->pid[i] < 0)
    {
      printf("Fork failed");
      return (kill_all(data, i), 1);
    }
  }
  return (wait_philos(data), 0);
}