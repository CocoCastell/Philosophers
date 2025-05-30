/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_time_in_ms();
	while ((get_time_in_ms() - start) < milliseconds)
		usleep(500);
}

void  wait_philos(t_data *data)
{
  int i;
  int status;

  i = 0;
  while (i < data->nb_of_philo)
  {
    waitpid(data->pid[i], &status, 0);
    i++;
  }
  
}

void  *kill_philos(void *args)
{
  int       i;
  t_philo   *philo;

  i = 0;
  philo = (t_philo *)args;
  sem_wait(philo->infos->end_diner);
  while (i < philo->infos->nb_of_philo)
  {
    if (philo->infos->pid[i] > 0)
    {
        kill(philo->infos->pid[i], SIGKILL);
        waitpid(philo->infos->pid[i], NULL, 0);
    }
    i++;
  }
  return (NULL);
}

void    print_action(t_philo *philo, char *action)
{
    time_t	timestamp;

    sem_wait(philo->infos->write);
	timestamp = get_time_in_ms() - philo->infos->time_start;
    printf("%ld %d %s \n", timestamp, philo->id_philo, action);
    sem_post(philo->infos->write);
}

void	synchronise_processes(t_data *data)
{
	while (data->time_start > get_time_in_ms())
		continue ;
}

void    *is_dead(void *args)
{
    time_t  time;
    t_philo *philo;
    time_t  last_meal;

    philo = (t_philo *)args;
    while (true)
    {
        // sem_wait(philo->infos->philo_dead);
        // time = get_time_in_ms() - philo->infos->time_start;
        // last_meal = philo->time_last_meal;
        time = get_time_in_ms(); //- philo->infos->time_start;
        last_meal = get_long(&philo->meal_time, &philo->time_last_meal);
        // last_meal = philo->time_last_meal;
        // printf("time : %ld, last meal: %ld, tot: %ld\n", time, last_meal, time - last_meal);
        if (time - last_meal > philo->infos->time_to_die)
        {
            // printf("check\n");
            // sem_wait(philo->infos->philo_dead);
	        sem_post(philo->infos->end_diner);
            print_action(philo, DIED);
            clear_process(philo->infos);
        }
        // sem_post(philo->infos->philo_dead);
        ft_usleep(1);
    }
    exit(0);
}