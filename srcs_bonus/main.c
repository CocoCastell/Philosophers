/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cochatel <cochatel@student.42barcelona.com>+#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 14:47:37 by cochatel          #+#    #+#             */
/*   Updated: 2025/03/08 16:24:47 by cochatel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void  kill_all(t_data *data);

int     main(int argc, char *argv[])
{
        t_data  *data;

        if (argc < 5 || argc > 6)
                return (printf("Error: wrong number of arguments.\n"), 1);
        if (parse_args(argv) == 1)
                return (2);
        data = init_data(argv, --argc);
        if (data == NULL)
                return (3);
	// if (data->nb_of_philo == 1)
	// 	one_philo_routine(data);
        if (diner_table(data) != 0)
                return (clear_all(data), 4);
        // kill_all(data);
        return (EXIT_SUCCESS);
}