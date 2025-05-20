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


# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define WHITE "\033[1;37m"
# define BLACK "\033[1;30m"
# define BL "\033[94m"
# define DEF "\033[0m"

// STRUCTURES 

typedef	pthread_mutex_t t_mutex;
typedef struct s_data t_data;

typedef struct s_philo
{
	int		id_philo;
	int		meals_eaten;
	int		time_last_meal;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
	t_data		*infos;
}	t_philo;

typedef struct s_data
{
	time_t	time_start;
	int	nb_of_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_of_meals;
	t_mutex	*forks;
	t_philo	*philos;
}	t_data;

// FUNCTIONS
void print_info(t_data *data);
void print_all_philo(t_philo *philos);
void print_single_philo(t_philo *philos);

long	ft_atol(const char *str, int *error);
int	thread_manager(t_data *philo_data);
int	parse_args(char *argv[]);

// Utils
time_t	get_time_in_ms(void);
void	destroy_mutex(t_mutex *forks, int nb_of_forks);

#endif
