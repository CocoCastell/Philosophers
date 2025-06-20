/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/03 14:36:39 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	one_philo_routine(t_data *data)
{
	t_philo	*philo;

	philo = &data->philos[0];
	safe_mutex_handler(MUTEX_LOCK, philo->left_fork);
	printf(GREEN"[0]"BL" 1 "DEF"has taken a fork\n");
	ft_usleep(data->time_to_die, data);
	printf(GREEN"[%d]"BL" 1 "RED"died\n"DEF, data->time_to_die);
	safe_mutex_handler(MUTEX_UNLOCK, philo->left_fork);
}

int	main(int argc, char *argv[])
{
	t_data	*data;

	if (argc < 5 || argc > 6)
		return (printf("Error: wrong number of arguments.\n"), 1);
	if (parse_args(argv) == 1)
		return (2);
	data = init_data(argv, --argc);
	if (data == NULL)
		return (3);
	if (data->nb_of_meals == 0)
	{
		clear_all(data);
		return (printf("Error: Number of meals must be more than 0.\n"), 4);
	}
	if (data->nb_of_philo == 1)
		one_philo_routine(data);
	else if (diner_table(data, -1) != 0)
		return (clear_all(data), 5);
	clear_all(data);
	return (EXIT_SUCCESS);
}
