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
    sem_wait(philo->infos->limit);   
    sem_wait(philo->infos->forks);
    print_action(philo, FORK);
    sem_wait(philo->infos->forks);
    print_action(philo, FORK);
    sem_post(philo->infos->limit);   
    // sem_wait(philo->infos->philo_dead);
    print_action(philo, EAT);
    // sem_post(philo->infos->philo_dead);
    set_long(get_time_in_ms(), &philo->time_last_meal, &philo->meal_time);
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

void    philo_routine(t_philo *philo)
{
    pthread_t death_thread;

    philo->time_start = get_time_in_ms();
    philo->time_last_meal = get_time_in_ms(); //- philo->infos->time_start;
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
 
  // data->time_start = get_time_in_ms();
  safe_thread_handler(PTHREAD_CREATE, &kill_thread, data->philos, kill_philos);
  while (i < data->nb_of_philo)
  {
    data->pid[i] = fork();
    if (data->pid[i] == 0)
      philo_routine(&data->philos[i]);
    else if (data->pid[i] < 0)
      printf("Fork failed"); // mieux gerer
    i++;
  }
  // waitpid(-1, NULL, 0);
  wait_philos(data);
  return (0);
}




/* void    *is_dead(void *args)
{
  t_philo *philo;

  philo = (t_philo *)args;
  while (true)
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
    return (NULL);
} */

/* void	thinking_routine(t_philo *philo)
{
	time_t	time_to_think;
	time_t	timestamp;

	timestamp = get_time_in_ms() - philo->time_start;
	printf(YELLOW"%ld %d is thinking\n"DEF, timestamp ,philo->id_philo);
	if (philo->infos->nb_of_philo % 2 == 0)
		return ;
	time_to_think = philo->infos->time_to_eat * 2 - philo->infos->time_to_sleep;
	if (time_to_think < 0)
		time_to_think = 0;
	ft_usleep(time_to_think * 0.3);
} */

/* void	philo_routine(t_philo *philo)
{
  pthread_t death_thread;

    philo->time_start = get_time_in_ms();
    philo->time_last_meal = get_time_in_ms();
    if (philo->id_philo % 2 == 0)
    {
        ft_usleep(philo->infos->time_to_eat * 0.3);
        print_action(philo, "is thinking");
    }
    safe_thread_handler(PTHREAD_CREATE, &death_thread, philo, is_dead);
    safe_thread_handler(PTHREAD_DETACH, &death_thread, NULL, NULL);
    while (true)
    {
  // sem_wait(philo->infos->limit);   
	sem_wait(philo->infos->forks);
	print_action(philo, "has taken a fork");
	sem_wait(philo->infos->forks);
	print_action(philo, "has taken a fork");
  // sem_post(philo->infos->limit);
	print_action(philo, "is eating");
	sem_wait(philo->infos->philo_dead);
	philo->time_last_meal = get_time_in_ms();
	sem_post(philo->infos->philo_dead);
	ft_usleep(philo->infos->time_to_eat);
	sem_post(philo->infos->forks);
	sem_post(philo->infos->forks);
	print_action(philo, "is sleeping");
	ft_usleep(philo->infos->time_to_sleep);
	print_action(philo, "is thinking");
  // thinking_routine(philo);
  }
} */

/* int diner_table(t_data *data)
{
	pid_t	id;
	int		i;

	i = -1;
	while (++i < data->nb_of_philo)
	{
		id = fork();
		data->pid[i] = id;
		if (id == 0)
      philo_routine(&data->philos[i]);
	}
	waitpid(-1, NULL, 0);
  return (0);
} */

