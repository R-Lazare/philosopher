/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluiz <rluiz@student.42lehavre.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 16:52:56 by rluiz             #+#    #+#             */
/*   Updated: 2023/11/13 18:11:47 by rluiz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <limits.h>
# include <pthread.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_arena
{
	void				*buf;
	size_t				buf_size;
	size_t				prev_offset;
	size_t				curr_offset;
}						t_arena;

typedef struct s_fork
{
	pthread_mutex_t		fork;
	int					fork_id;
}						t_fork;

typedef struct s_philo
{
	pthread_mutex_t		*philock;
	int					id;
	int					meals_counter;
	bool				full;
	int					is_eating;
	int					last_meal_time;
	t_fork				*left_fork;
	t_fork				*right_fork;
	pthread_t			thread_id;
	t_table				*table;
}						t_philo;

typedef struct s_table
{
	int					number_of_philosopher;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_times_each_philosopher_must_eat;
	int					start_simulation;
	int					start_time;
	int					philos_dead;
	pthread_mutex_t		*dead_mutex;
	pthread_mutex_t		*print;
	t_fork				*forks;
	t_philo				*philos;
	t_arena				*arena;
	pthread_mutex_t		*table_lock;
}						t_table;

void					*arena_init(size_t buffer_size);
void					arena_reset(t_arena *a);
void					*arena_alloc(t_arena *a, size_t size);
void					arena_destroy(t_arena *a);
void					error_exit(char *error, pthread_mutex_t *print,
							t_arena *arena);
int						get_time(void);
int						get_time_ms(t_table *table);
void					ft_putstr_fd(char *s, int fd);
void					ft_putchar_fd(char c, int fd);
char					*ft_itoa(t_arena *arena, int n);
int						ft_isdigit(const int c);
int						ft_atoi(const char *str);
void					init_philo(t_table *table);
void					init_fork(t_table *table);
int						ft_printf(t_table *table, const char *src, ...);
int						ft_putstr(char *src);
int						ft_putchar(char c);
int						check_death(t_philo *philo2);
void					*philo_life(void *philo);
void					philo_eat(t_philo *philo2);
void					lock_helper(t_philo *philo2);
int						check_helper(t_philo *philo2);
int						check_args(int argc, char **argv);

#endif