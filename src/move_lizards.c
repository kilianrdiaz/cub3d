/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_lizards.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 22:34:04 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/04 22:34:09 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	move_lizard_to(t_game *g, t_sprite *l, double new_x, double new_y)
{
	int	old_gx;
	int	old_gy;
	int	gx;
	int	gy;

	old_gx = (int)l->x;
	old_gy = (int)l->y;
	gx = (int)new_x;
	gy = (int)new_y;
	if (gx < 0 || gx >= MAP_W || gy < 0 || gy >= MAP_H)
		return (0);
	if (g->map[gy][gx] == '1' || g->map[gy][gx] == 'L' || g->map[gy][gx] == 'P')
		return (0);
	if (gx != old_gx || gy != old_gy)
	{
		if (g->map[old_gy][old_gx] == 'L')
			g->map[old_gy][old_gx] = '0';
		if (g->map[gy][gx] == '0')
			g->map[gy][gx] = 'L';
	}
	l->x = new_x;
	l->y = new_y;
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
	move_lizard_to(g, l, l->x + dx, l->y + dy);
}

static double	get_move_speed(double diff)
{
	if (diff > 0)
		return (MOVE_SPEED_LIZARD);
	else
		return (-MOVE_SPEED_LIZARD);
}

static void	chase_lizard(t_game *g, t_sprite *l, t_spidy *p)
{
	double	diff_x;
	double	diff_y;
	double	dx;
	double	dy;

	diff_x = p->x - l->x;
	diff_y = p->y - l->y;
	if (fabs(diff_x) > fabs(diff_y))
		dx = get_move_speed(diff_x);
	else
		dy = get_move_speed(diff_y);
	if (move_lizard_to(g, l, l->x + dx, l->y + dy))
		return ;
	dx = 0;
	dy = 0;
	if (fabs(diff_x) > fabs(diff_y))
		dy = get_move_speed(diff_y);
	else
		dx = get_move_speed(diff_x);
	move_lizard_to(g, l, l->x + dx, l->y + dy);
}

void	move_lizards(t_game *g)
{
	t_sprite	*l;
	double		dist;
	int			i;

	i = -1;
	while (g->lizards[++i])
	{
		l = g->lizards[i];
		if (!l || g->timer < l->delay)
			continue ;
		else if (l->state == ATTACKED && g->timer < l->delay)
			continue ;
		else if (l->state == ATTACKED && g->timer >= l->delay)
			l->state = ACTIVE;
		else
			l->delay = g->timer + 1;
		dist = fabs(l->x - g->spider.x) + fabs(l->y - g->spider.y);
		if (dist <= DETECT_RADIUS)
			chase_lizard(g, l, &g->spider);
		else
			patrol_lizard(g, l);
	}
}
