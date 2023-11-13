/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:53:00 by rluiz             #+#    #+#             */
/*   Updated: 2023/11/13 18:19:59 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(char *error, pthread_mutex_t *print, t_arena *arena)
{
	pthread_mutex_lock(print);
	ft_putstr_fd(error, 1);
	if (arena)
		arena_destroy(arena);
	exit(EXIT_FAILURE);
}

int	get_time_ms(t_table *table)
{
	struct timeval	time;
	int				timems;

	gettimeofday(&time, NULL);
	if (table->start_time == 0)
		table->start_time = (int)(1000 * time.tv_sec) + (int)(time.tv_usec
				/ 1000);
	timems = (int)(1000 * time.tv_sec) + (int)(time.tv_usec / 1000)
		- table->start_time;
	return (timems);
}

void	init_fork(t_table *table)
{
	t_fork	*fork;
	int		i;

	i = 0;
	fork = (t_fork *)arena_alloc(table->arena, sizeof(t_fork)
			* table->number_of_philosopher);
	if (!fork)
		error_exit("Forks could't be malloc'd\n", table->print, table->arena);
	while (i < table->number_of_philosopher)
	{
		fork[i].fork_id = i + 1;
		pthread_mutex_init(&(fork[i].fork), NULL);
		table->philos[i].right_fork = fork + i;
		if (table->number_of_philosopher == 1)
			table->philos[i].left_fork = 0;
		else if (i != table->number_of_philosopher - 1)
			table->philos[i + 1].left_fork = fork + i;
		else
			table->philos[0].left_fork = fork + i;
		i++;
	}
	table->print = (pthread_mutex_t *)arena_alloc(table->arena,
			sizeof(pthread_mutex_t));
	return ;
}

void	init_philo(t_table *table)
{
	int	i;

	table->philos = (t_philo *)arena_alloc(table->arena, sizeof(t_philo)
			* table->number_of_philosopher);
	i = 0;
	table->dead_mutex = (pthread_mutex_t *)arena_alloc(table->arena,
			sizeof(pthread_mutex_t));
	pthread_mutex_init(table->dead_mutex, NULL);
	table->table_lock = (pthread_mutex_t *)arena_alloc(table->arena,
			sizeof(pthread_mutex_t));
	pthread_mutex_init(table->table_lock, NULL);
	while (i < table->number_of_philosopher)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_counter = 0;
		table->philos[i].full = 0;
		table->philos[i].is_eating = 0;
		table->philos[i].last_meal_time = 0;
		table->philos[i].table = table;
		table->philos[i].philock = (pthread_mutex_t *)arena_alloc(table->arena,
				sizeof(pthread_mutex_t));
		pthread_mutex_init(table->philos[i].philock, NULL);
		i++;
	}
	return (init_fork(table));
}