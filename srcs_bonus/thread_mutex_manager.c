/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_mutex_manager.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:11:14 by cochatel          #+#    #+#             */
/*   Updated: 2024/10/05 17:29:00 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int thread_err_handler(int error, enum e_opcode opcode)
{
  if (error == 0)
    return (0);
  if (error == EAGAIN)
		printf("Error: Insufficient resources to create another thread.\n");
	if (error == EINVAL && opcode == PTHREAD_CREATE)
		printf("Error: The value specified by in attr is invalid.\n");
	if (error == EINVAL && (opcode == PTHREAD_JOIN || opcode == PTHREAD_DETACH))
		printf("Error: The value specified by the thread is not joinable.\n");
	if (error == EPERM)
		printf("Error: The caller does not have appropriate permission.\n");
	if (error == ESRCH)
		printf("Error: No thread could be found corresponding to that specified by the given thread ID.\n");
	if (error == EDEADLK)
		printf("Error: A deadlock was detected.\n");
  return (1);
}

int  mutex_err_handler(int error, t_opcode opcode)
{
  if (error == 0)
    return (0);
  if (error == EINVAL && (opcode == MUTEX_LOCK || opcode == MUTEX_UNLOCK))
    printf("Error: The value specified by mutex is invalid.\n");
  else if (error == EINVAL && opcode == MUTEX_INIT)
    printf("Error: The value specified by attr is invalid.\n");
  else if (error == EDEADLK)
    printf("Error: A deadlock would occur if the thread blocked waiting for mutex.\n");
  else if (error == EPERM)
    printf("Error: The current thread does not hold a lock on mutex.\n");
  else if (error == EBUSY)
    printf("Error: The mutex is currently locked.\n");
  else if (error == ENOMEM)
    printf("Error: The process can not allocate enough memory to create another mutex.\n");
  return (1);
}

int	safe_mutex_handler(t_opcode opcode, t_mutex *mutex)
{
  if (opcode == MUTEX_INIT && mutex_err_handler(pthread_mutex_init(mutex, NULL), opcode) == 1)
    return (1);
  else if (opcode == MUTEX_LOCK && mutex_err_handler(pthread_mutex_lock(mutex), opcode) == 1)
    return (1);
  else if (opcode == MUTEX_UNLOCK && mutex_err_handler(pthread_mutex_unlock(mutex), opcode) == 1)
    return (1);
  else if (opcode == MUTEX_DESTROY && mutex_err_handler(pthread_mutex_destroy(mutex), opcode) == 1)
    return (1);
  return (0);
}

int	safe_thread_handler(t_opcode opcode, pthread_t *thread, t_philo *philo, void *(*routine)(void *))
{
  void *output;
  int   error;

  if (opcode == PTHREAD_CREATE && thread_err_handler(pthread_create(thread, NULL, routine, philo), opcode) == 1)
    return (1);
  else if (opcode == PTHREAD_DETACH && thread_err_handler(pthread_detach(*thread), opcode) == 1)
    return (1);
  return (0);
}