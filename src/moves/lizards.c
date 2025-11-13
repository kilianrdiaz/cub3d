/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lizards.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 22:34:04 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/04 21:05:45 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	move_lizard_to(t_game *g, t_sprite *l, t_coords move)
{
	int 	x;

	if (move.x < 0 || move.y < 0)
		return (0);
	if (g->map[(int)move.y][(int)move.x] == '1')
		return (0);
	x = -1;
	while (g->bombs && g->bombs[++x])
	{
		if (g->bombs[x]->state == DEFUSED)
			continue ;
		if (g->bombs[x]->pos.x == move.x
			&& g->bombs[x]->pos.y == move.y)
			g->bombs[x]->state = NO_RENDER;
		else
			g->bombs[x]->state = ACTIVE;
	}
	l->pos = move;
	l->delay = g->timer + 10;
	return (1);
}

static void	patrol_lizard(t_game *g, t_sprite *l)
{
	int		dir;
	double	dx;
	double	dy;

	dx = 0;
	dy = 0;
	dir = rand() % 4;
	if (dir == 0)
		dx = -MOVE_SPEED_LIZARD;
	else if (dir == 1)
		dx = MOVE_SPEED_LIZARD;
	else if (dir == 2)
		dy = -MOVE_SPEED_LIZARD;
	else
		dy = MOVE_SPEED_LIZARD;
	move_lizard_to(g, l, (t_coords){l->pos.x + dx, l->pos.y + dy});
}

static double	get_move_speed(double diff)
{
	if (diff > 0)
		return (MOVE_SPEED_LIZARD);
	else
		return (-MOVE_SPEED_LIZARD);
}

static void	chase_lizard(t_game *g, t_sprite *l, t_coords spider)
{
	t_coords	diff;
	t_coords	direction;

	diff.x = spider.x - l->pos.x;
	diff.y = spider.y - l->pos.y;
	direction.x = 0;
	direction.y = 0;
	if (fabs(diff.x) > fabs(diff.y))
		direction.x = get_move_speed(diff.x);
	else
		direction.y = get_move_speed(diff.y);
	if (move_lizard_to(g, l, (t_coords){l->pos.x + direction.x, l->pos.y
			+ direction.y}))
		return ;
	direction.x = 0;
	direction.y = 0;
	if (fabs(diff.x) > fabs(diff.y))
		direction.y = get_move_speed(diff.y);
	else
		direction.x = get_move_speed(diff.x);
	move_lizard_to(g, l, (t_coords){l->pos.x + direction.x, l->pos.y
		+ direction.y});
}

void	move_lizards(t_game *g)
{
	t_sprite	*l;
	int			i;

	i = -1;
	while (g->lizards && g->lizards[++i])
	{
		l = g->lizards[i];
		if (!l || g->timer < l->delay)
			continue ;
		if (l->state == ACTIVE)
			l->state = MOVING;
		else if (l->state == MOVING || (l->state == ATTACKED
				&& g->timer >= l->delay))
			l->state = ACTIVE;
		if (l->dist <= 1.1)
		{
			if (g->timer >= l->delay)
			{
				l->state = ATTACKING;// 1 segundo entre ataques
				player_take_damage(g, 5);
			}
		}
		if (l->dist <= DETECT_RADIUS)
			chase_lizard(g, l, g->spider.pos);
		else
			patrol_lizard(g, l);
	}
}
