/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:52:58 by rluiz             #+#    #+#             */
/*   Updated: 2023/11/14 19:42:12 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_table	*parse(t_arena *arena, int argc, char **argv)
{
	t_table	*table;

	table = (t_table *)arena_alloc(arena, sizeof(t_table));
	table->arena = arena;
	table->start_time = 0;
	table->start_time = get_time_ms(table);
	table->print = (pthread_mutex_t *)arena_alloc(table->arena, 20);
	pthread_mutex_init(table->print, NULL);
	if (argc != 6 && argc != 5)
		error_exit("Error: Wrong number of arguments\n", table->print, arena);
	table->number_of_philosopher = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else if (argc == 5)
		table->number_of_times_each_philosopher_must_eat = -1;
	if (!check_args(argc, argv))
		error_exit("Error: Wrong arguments\n", table->print, arena);
	table->philos_dead = 0;
	return (table);
}

int	check_death(t_philo *philo2)
{
	if (philo2->full)
		return (0);
	pthread_mutex_lock(philo2->table->table_lock);
	if (philo2->table->philos_dead)
	{
		if (philo2->is_eating == 1 && philo2->id % 2 == 0)
			pthread_mutex_unlock(&(philo2->right_fork->fork));
		else if (philo2->is_eating == 1 && philo2->id % 2 == 1)
			pthread_mutex_unlock(&(philo2->left_fork->fork));
		else if (philo2->is_eating == 2)
		{
			pthread_mutex_unlock(&(philo2->left_fork->fork));
			pthread_mutex_unlock(&(philo2->right_fork->fork));
		}
		pthread_mutex_unlock(philo2->table->table_lock);
		return (0);
	}
	if ((philo2->last_meal_time != 0 && get_time_ms(philo2->table)
			- philo2->last_meal_time > philo2->table->time_to_die)
		|| philo2->table->number_of_philosopher == 1)
	{
		return (check_helper(philo2));
	}
	pthread_mutex_unlock(philo2->table->table_lock);
	return (1);
}

void	lock_fork(t_philo *philo2)
{
	if (philo2->id % 2 == 0)
	{
		pthread_mutex_lock(&(philo2->right_fork->fork));
		ft_printf(philo2->table, "%d ms %d has taken a fork\n",
			get_time_ms(philo2->table), philo2->id);
		pthread_mutex_lock(&(philo2->left_fork->fork));
		ft_printf(philo2->table, "%d ms %d has taken a fork\n",
			get_time_ms(philo2->table), philo2->id);
	}
	else
		lock_helper(philo2);
	philo2->is_eating = 1;
	ft_printf(philo2->table, "%d ms %d is eating\n", get_time_ms(philo2->table),
		philo2->id);
	if (get_time_ms(philo2->table) - philo2->last_meal_time > philo2->table->time_to_die)
	{
		ft_printf(philo2->table, "%d ms %d died\n", get_time_ms(philo2->table),
		philo2->id);
		philo2->table->philos_dead = 1;
		pthread_mutex_unlock(&(philo2->right_fork->fork));
		pthread_mutex_unlock(&(philo2->left_fork->fork));
		pthread_exit(NULL);
	}
	philo2->last_meal_time = get_time_ms(philo2->table);
	usleep(philo2->table->time_to_eat * 1000);
}

void	philo_eat(t_philo *philo2)
{
	int	i;

	i = philo2->table->number_of_times_each_philosopher_must_eat;
	lock_fork(philo2);
	pthread_mutex_unlock(&(philo2->right_fork->fork));
	pthread_mutex_unlock(&(philo2->left_fork->fork));
	if (philo2->table->philos_dead)
		pthread_exit(NULL);
	philo2->is_eating = 0;
	philo2->meals_counter++;
	if (i == philo2->meals_counter)
		philo2->full = 1;
}

int	main(int argc, char **argv)
{
	t_table	*table;
	t_arena	*arena;
	int		i;

	i = -1;
	arena = arena_init(1000000);
	table = parse(arena, argc, argv);
	init_philo(table);
	while (++i < table->number_of_philosopher)
	{
		pthread_create(&(table->philos[i].thread_id), NULL, philo_life,
			&(table->philos[i]));
		usleep(100);
	}
		
	i = 0;
	while(check_death(table->philos + i))
	{
		i++;
		if (i == table->number_of_philosopher) i = 0;
	}
	i = -1;
	while (++i < table->number_of_philosopher)
		pthread_join(table->philos[i].thread_id, NULL);
	i = -1;
	ft_printf(table, "%d ms everyone is full\n", get_time_ms(table));
	arena_destroy(arena);
	return (0);
}
