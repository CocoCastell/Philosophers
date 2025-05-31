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

void  kill_all(t_data *data)
{
  int       i;

  i = 0;
  while (i < data->nb_of_philo)
  {
    if (data->pid[i] > 0)
    {
        kill(data->pid[i], SIGKILL);
        waitpid(data->pid[i], NULL, 0);
    }
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
	timestamp = get_time_in_ms() - philo->time_start;
    printf(GREEN"[%ld] "BL"%d "DEF"%s \n", timestamp, philo->id_philo, action);
    sem_post(philo->infos->write);
}

void    *is_dead(void *args)
{
    time_t  time;
    time_t  last_meal;
    t_philo *philo;

    philo = (t_philo *)args;
   /* 	while (true)
	{
		sem_wait(philo->infos->philo_dead);
		if (get_time_in_ms() - philo->time_last_meal > philo->infos->time_to_die)
		{
			print_action(philo, RED"died"DEF);
			sem_wait(philo->infos->write);
			exit(EXIT_SUCCESS);
		}
		sem_post(philo->infos->philo_dead);
	}
    return (NULL); */
    while (true)
    {
        time = get_time_in_ms(); //- philo->infos->time_start;
        last_meal = get_long(&philo->meal_time, &philo->time_last_meal);
        if (time - last_meal > philo->infos->time_to_die)
        {
            // printf("last meal: %ld, time: %ld, tot: %ld\n", last_meal, time, time - last_meal);
            // printf(RED"%ld %d %s\n"DEF, time - philo->time_start, philo->id_philo, DIED);
            sem_wait(philo->infos->philo_dead);
            print_action(philo, DIED);
	          sem_post(philo->infos->end_diner);
            clear_process(philo->infos);
        }
        ft_usleep(1);
    }
    exit(0);
}