/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 23:08:27 by rluiz             #+#    #+#             */
/*   Updated: 2023/11/13 18:55:53 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	life_helper(t_philo *philo2)
{
	if (philo2->table->number_of_philosopher == 1)
	{
		pthread_mutex_lock(&(philo2->right_fork->fork));
		ft_printf(philo2->table, "%d ms %d has taken a fork\n",
			get_time_ms(philo2->table), philo2->id);
		usleep(philo2->table->time_to_die * 1000);
		check_death(philo2);
	}
	check_death(philo2);
	philo_eat(philo2);
	if (philo2->full)
		return (1);
	ft_printf(philo2->table, "%d ms %d is sleeping\n",
		get_time_ms(philo2->table), philo2->id);
	check_death(philo2);
	usleep(philo2->table->time_to_sleep * 1000);
	ft_printf(philo2->table, "%d ms %d is thinking\n",
		get_time_ms(philo2->table), philo2->id);
	check_death(philo2);
	return (0);
}

void	*philo_life(void *philo)
{
	t_philo	*philo2;

	philo2 = (t_philo *)philo;
	while (!philo2->table->philos_dead && !philo2->full && check_death(philo2))
	{
		if (philo2->table->number_of_philosopher == 1)
		{
			pthread_mutex_lock(&(philo2->right_fork->fork));
			ft_printf(philo2->table, "%d ms %d has taken a fork\n",
				get_time_ms(philo2->table), philo2->id);
			usleep(philo2->table->time_to_die * 1000);
		}
		philo_eat(philo2);
		if (philo2->full)
			break ;
		ft_printf(philo2->table, "%d ms %d is sleeping\n",
			get_time_ms(philo2->table), philo2->id);
		usleep(philo2->table->time_to_sleep * 1000);
		ft_printf(philo2->table, "%d ms %d is thinking\n",
			get_time_ms(philo2->table), philo2->id);
	}
	pthread_exit(NULL);
}

void	lock_helper(t_philo *philo2)
{
	pthread_mutex_lock(&(philo2->left_fork->fork));
	ft_printf(philo2->table, "%d ms %d has taken a fork\n",
		get_time_ms(philo2->table), philo2->id);
	check_death(philo2);
	pthread_mutex_lock(&(philo2->right_fork->fork));
	ft_printf(philo2->table, "%d ms %d has taken a fork\n",
		get_time_ms(philo2->table), philo2->id);
}

int	check_helper(t_philo *philo2)
{
	ft_printf(philo2->table, "%d ms %d died\n", get_time_ms(philo2->table),
		philo2->id);
	pthread_mutex_lock(philo2->table->dead_mutex);
	philo2->table->philos_dead = 1;
	pthread_mutex_unlock(philo2->table->dead_mutex);
	if (philo2->is_eating != 0)
	{
		if (philo2->table->number_of_philosopher != 1)
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
		}
		else
			pthread_mutex_unlock(&(philo2->right_fork->fork));
	}
	pthread_mutex_unlock(philo2->table->table_lock);
	return (0);
}

int	check_args(int argc, char **argv)
{
	if (ft_atoi(argv[1]) < 1 || ft_atoi(argv[1]) > 200)
		return (0);
	if (ft_atoi(argv[2]) < 60 || ft_atoi(argv[3]) < 60 || ft_atoi(argv[4]) < 60)
		return (0);
	if (argc == 6 && ft_atoi(argv[5]) < 1)
		return (0);
	return (1);
}