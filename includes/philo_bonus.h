/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
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
# include <stdbool.h>
# include <time.h>
# include <semaphore.h>
# include <errno.h>
# include <sys/wait.h>
#	include <semaphore.h>
# include <fcntl.h>
# include <sys/stat.h>

# define FORK "has taken a fork"
# define EAT "is eating"
# define SLEEP "is sleeping"
# define THINK "is thinking"
# define DIED "died"

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define YELLOW "\033[1;33m"
# define BL "\033[94m"
# define DEF "\033[0m"

// ENUMS

typedef enum e_opcode 
{
	PTHREAD_CREATE,
	PTHREAD_JOIN,
	PTHREAD_DETACH,
	MUTEX_LOCK,
	MUTEX_UNLOCK,
	MUTEX_INIT,
	MUTEX_DESTROY,
}	t_opcode;

// STRUCTURES 

typedef	pthread_mutex_t t_mutex;
typedef struct s_data t_data;

typedef struct s_philo
{
	time_t		time_start;
	int				id_philo;
	int				meals_eaten;
	time_t		time_last_meal;
	t_mutex		meals_mutex;
	t_mutex		meal_time;
	t_data		*infos;
	pthread_t	thread;
}	t_philo;

typedef struct s_data
{
	int			nb_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			nb_of_meals;
	int			*pid;
	sem_t		*limit;
	sem_t		*end_diner;
	sem_t		*philo_dead;
	sem_t		*forks;
	sem_t		*write;
	t_philo	*philos;
}	t_data;

// 	FUNCTIONS

// Init
t_data	*init_data(char *argv[], int nb_of_arg);
t_philo	*init_philo(int nb_of_philo, t_data *infos);
void		init_meal_time(t_philo *philo);
int			init_forks(t_mutex *forks, int nb_of_forks);

// Diner manager
void	eating_routine(t_philo *philo);
void	sleeping_routine(t_philo *philo);
void	thinking_routine(t_philo *philo);
void	philo_routine(t_philo *philo);
int		diner_table(t_data *data);

// Diner utils
void  *is_dead(void *args);
void	*kill_philos(void *args);
void	one_philo_routine(t_data *data); // TODO
void	ft_usleep(size_t milliseconds);
void	print_action(t_philo *philo, char *action);
void	wait_philos(t_data *data);

// Parsing
int		parse_args(char *argv[]);
long	ft_atol(const char *str, int *error);

// Utils
time_t	get_time_in_ms(void);
time_t	get_long(t_mutex *mutex, time_t *target);
void		set_long(long value, long *target, t_mutex *mutex);
void		clear_all(t_data *data);
void		clear_process(t_data *data);
void		synchronise_processes(t_data *data);

// Error management
int	thread_error_handler(int error, t_opcode opcode);
int	mutex_error_handler(int error, t_opcode opcode);
int	safe_mutex_handler(t_opcode opcode, t_mutex *mutex);
int	safe_thread_handler(t_opcode opcode, pthread_t *thread, t_philo *data, void *(*routine)(void *));

#endif
