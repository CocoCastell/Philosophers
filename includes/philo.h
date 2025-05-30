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
# include <stdbool.h>
# include <time.h>
# include <semaphore.h>
# include <errno.h>
# include <sys/wait.h>

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
	int				id_philo;
	int				meals_eaten;
	int				time_last_meal;
	t_mutex		meals_mutex;
	t_mutex		meal_time;
	t_mutex		*left_fork;
	t_mutex		*right_fork;
	t_data		*infos;
	pthread_t	thread;
}	t_philo;

typedef struct s_data
{
	time_t	time_start;
	int			nb_of_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			nb_of_meals;
	int			nb_of_full_philo;
	bool		is_end_sim;
	t_mutex	data_mutex;
	t_mutex	is_end_mutex;
	t_mutex	*forks;
	t_philo	*philos;
}	t_data;

// 	FUNCTIONS

// Init
t_data	*init_data(char *argv[], int nb_of_arg);
t_philo	*init_philo(int nb_of_philo, t_mutex *forks, t_data *infos);
t_mutex	*init_fork(int nb_of_forks);
int		init_meal_time(t_philo *philo);

// Diner manager
int		eating_routine(t_philo *philo);
int		diner_table(t_data *data);
void	sleeping_routine(t_philo *philo);
void	thinking_routine(t_philo *philo);
void	*philo_routine(void *args);

// Diner utils
int		destroy_forks(t_mutex *forks, int nb_of_forks);
int		take_fork(t_mutex *first_fork, t_mutex *second_fork, t_philo *philo);
void	synchronise_threads(t_data *data);
void	de_synchronise_threads(t_philo *philo);
void	one_philo_routine(t_data *data);

// State tracking
bool	is_full(t_philo *philo, int *error);
void	*state_tracker(void *args);
int		check_if_dead(t_philo *philo, int *error);

// Parsing
int		parse_args(char *argv[]);
long	ft_atol(const char *str, int *error);

// Utils
time_t	get_time_in_ms(void);
bool		get_bool(t_mutex *mutex, bool target, int *error);
int			set_bool(bool value, bool *target, t_mutex *mutex);
int			get_int(t_mutex *mutex, int target, int *error);
int			set_int(int value, int *target, t_mutex *mutex);
void		clear_all(t_data *data);
void		ft_usleep(size_t milliseconds);

// Error management
int	thread_error_handler(int error, t_opcode opcode);
int	mutex_error_handler(int error, t_opcode opcode);
int	safe_mutex_handler(t_opcode opcode, t_mutex *mutex);
int	safe_thread_handler(t_opcode opcode, pthread_t *thread, t_philo *data, void *(*routine)(void *));

#endif
