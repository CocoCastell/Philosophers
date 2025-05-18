/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/01/26 16:57:10 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

// STRUCTURES 

typedef	pthread_mutex_t t_mutex;

typedef struct s_philo
{
	int		id_philo;
	int		meals_eaten;
	int		time_last_meal;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
}	t_philo;

typedef struct s_data
{
	int	nb_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_of_meals;
	t_mutex	**forks;
	t_philo	*philos;
}	t_data;

// FUNCTIONS

void	thread_manager(t_data *philo_data);
long	ft_atol(const char *str, int *error);
int	parse_args(char *argv[]);

#endif
