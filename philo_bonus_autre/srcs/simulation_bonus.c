/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dapetros <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 18:37:57 by dapetros          #+#    #+#             */
/*   Updated: 2024/03/16 18:37:59 by dapetros         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils_bonus.h"

void	*die_checker(void *ptr)
{
	t_philo	*philo;

	philo = (t_philo *)ptr;
	while (true)
	{
		sem_wait(philo->sems->die_sem);
		if (get_current_time() - philo->times.last_meal > philo->times.die)
		{
			print_action(philo, RED"died"RESET);
			sem_wait(philo->sems->write_sem);
			exit(EXIT_SUCCESS);
		}
		sem_post(philo->sems->die_sem);
	}
	return (NULL);
}

void	start_simulation(t_engine *engine, int index)
{
	t_id	die_checker_id;
	t_philo	*philo;

	philo = engine->philos[index];
	pthread_create(&die_checker_id, NULL, die_checker, philo);
	pthread_detach(die_checker_id);
	philo->times.born_time = get_current_time();
	philo->times.last_meal = get_current_time();
	while (true)
	{
	sem_wait(philo->sems->fork_sem);
	print_action(philo, "has taken a fork");
	sem_wait(philo->sems->fork_sem);
	print_action(philo, "has taken a fork");
	print_action(philo, "is eating");
	sem_wait(philo->sems->die_sem);
	philo->times.last_meal = get_current_time();
	sem_post(philo->sems->die_sem);
	ft_usleep(philo->times.eat);
	sem_post(philo->sems->fork_sem);
	sem_post(philo->sems->fork_sem);
	print_action(philo, "is sleeping");
	ft_usleep(philo->times.sleep);
	print_action(philo, "is thinking");
	}
}

void	launcher(t_engine *engine, int count)
{
	pid_t	id;
	int		i;

	i = -1;
	while (++i < count)
	{
		id = fork();
		engine->proc_ids[i] = id;
		if (id == -1)
			destroy_all(engine, "[Fork ERROR]\n", true, 1);
		if (id == 0)
			start_simulation(engine, i);
	}
	waitpid(-1, NULL, 0);
}
