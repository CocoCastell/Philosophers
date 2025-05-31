/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	clear_all(t_data *data)
{
	int	i;

	i = -1;
	safe_mutex_handler(MUTEX_DESTROY, &data->print_mutex);
	safe_mutex_handler(MUTEX_DESTROY, &data->data_mutex);
	safe_mutex_handler(MUTEX_DESTROY, &data->is_end_mutex);
	while (++i < data->nb_of_philo)
	{
		safe_mutex_handler(MUTEX_DESTROY, &data->philos[i].meals_mutex);
		safe_mutex_handler(MUTEX_DESTROY, &data->philos[i].meal_time);
		safe_mutex_handler(MUTEX_DESTROY, &data->forks[i]);
	}
	if (data->forks != NULL)
		free(data->forks);
	if (data->philos != NULL)
		free(data->philos);
	if (data != NULL)
		free(data);
}

bool get_bool(t_mutex *mutex, bool *target, int *error)
{
	int		tmp;
	bool	value;

	tmp = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (true);
	};
	value = *target;
	tmp = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (tmp!= 0)
	{
		*error = 1;
		return (true);
	};
	return (value);
}

int	set_bool(bool value, bool *target, t_mutex *mutex)
{
	int	error;

	error = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (error != 0)
		return (1);
	*target = value;
	error = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (error != 0)
		return (1);
	return (0);
}

int get_int(t_mutex *mutex, int *target, int *error)
{
	int tmp;
	int value;

	tmp = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (0);
	};
	value = *target;
	tmp = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (tmp != 0)
	{
		*error = 1;
		return (0);
	};
	return (value);
}

int	set_int(int value, int *target, t_mutex *mutex)
{
	int	error;

	error = safe_mutex_handler(MUTEX_LOCK, mutex);
	if (error != 0)
		return (1);
	*target = value;
	error = safe_mutex_handler(MUTEX_UNLOCK, mutex);
	if (error != 0)
		return (1);
	return (0);
}