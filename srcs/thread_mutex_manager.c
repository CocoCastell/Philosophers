/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_mutex_manager.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+       */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2025/06/03 16:55:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	thread_err_handler(int error, enum e_opcode opcode)
{
	if (error == 0)
		return (0);
	if (error == EAGAIN)
		print_action(NULL, "Error: Insufficient resources to create another \
				thread.\n", false);
	if (error == EINVAL && opcode == PTHREAD_CREATE)
		print_action(NULL, "Error: The value specified by in attr is \
				invalid.\n", false);
	if (error == EINVAL && (opcode == PTHREAD_JOIN || opcode == PTHREAD_DETACH))
		print_action(NULL, "Error: The value specified by the thread is not \
				joinable.\n", false);
	if (error == EPERM)
		print_action(NULL, "Error: The caller does not have appropriate \
				permission.\n", false);
	if (error == ESRCH)
		print_action(NULL, "Error: No thread could be found corresponding to \
				that specified \
				by the given thread ID.\n", false);
	if (error == EDEADLK)
		print_action(NULL, "Error: A deadlock was detected.\n", false);
	return (1);
}

int	mutex_err_handler(int error, t_opcode opcode)
{
	if (error == 0)
		return (0);
	if (error == EINVAL && (opcode == MUTEX_LOCK || opcode == MUTEX_UNLOCK))
		print_action(NULL, "Error: The value specified by mutex is \
				invalid.\n", false);
	else if (error == EINVAL && opcode == MUTEX_INIT)
		print_action(NULL, "Error: The value specified by attr is \
				invalid.\n", false);
	else if (error == EDEADLK)
		print_action(NULL, "Error: A deadlock would occur if the thread blocked \
				waiting for mutex.\n", false);
	else if (error == EPERM)
		print_action(NULL, "Error: The current thread does not hold a lock on \
				mutex.\n", false);
	else if (error == EBUSY)
		print_action(NULL, "Error: The mutex is currently locked.\n", false);
	else if (error == ENOMEM)
		print_action(NULL, "Error: The process can not allocate enough memory to create \
				another mutex.\n", false);
	return (1);
}

int	safe_mutex_handler(t_opcode opcode, t_mutex *mutex)
{
	if (opcode == MUTEX_INIT && mutex_err_handler(pthread_mutex_init(mutex, \
					NULL), opcode) == 1)
		return (1);
	else if (opcode == MUTEX_LOCK && mutex_err_handler(pthread_mutex_lock(\
					mutex), opcode) == 1)
		return (1);
	else if (opcode == MUTEX_UNLOCK && mutex_err_handler(pthread_mutex_unlock(\
					mutex), opcode) == 1)
		return (1);
	else if (opcode == MUTEX_DESTROY && mutex_err_handler(pthread_mutex_destroy(\
					mutex), opcode) == 1)
		return (1);
	return (0);
}

int	safe_th_handle(t_opcode c, pthread_t *t, t_philo *p, void *(*f)(void *))
{
	void	*output;
	int		error;

	if (c == PTHREAD_CREATE && thread_err_handler(pthread_create(t, NULL, \
					f, p), c) == 1)
		return (1);
	else if (c == PTHREAD_JOIN)
	{
		if (thread_err_handler(pthread_join(*t, &output), c) == 1)
			return (1);
		error = (int)(intptr_t)output;
		return (error);
	}
	else if (c == PTHREAD_DETACH && thread_err_handler(pthread_detach(*t), c) \
			== 1)
		return (1);
	return (0);
}
