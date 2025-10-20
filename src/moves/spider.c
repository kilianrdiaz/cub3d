/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:14:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 15:28:48 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_walkable(int x, int y, char **map)
{
	if (x < 0 || y < 0)
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

static void	move_player(t_game *g, double dir)
{
	double	new_x;
	double	new_y;
	double	distance;

	new_x = g->spider.pos.x + g->spider.dir.x * dir * MOVE_SPEED;
	new_y = g->spider.pos.y + g->spider.dir.y * dir * MOVE_SPEED;
	distance = sqrt(pow(new_x - g->spider.pos.x, 2) + pow(new_y
				- g->spider.pos.y, 2));
	if (is_walkable((int)new_x, (int)new_y, g->map))
	{
		g->map[(int)g->spider.pos.y][(int)g->spider.pos.x] = '0';
		g->spider.pos.x = new_x;
		g->spider.pos.y = new_y;
		g->map[(int)g->spider.pos.y][(int)g->spider.pos.x] = 'P';
	}
	g->spider.move_accum += distance;
	if (g->spider.move_accum >= 0.40)
	{
		if (g->spider.state == ACTIVE)
			g->spider.state = MOVING;
		else if (g->spider.state == MOVING)
			g->spider.state = ACTIVE;
		g->spider.move_accum = 0.0;
	}
	print_map(g);
}

static void	rotate_spidy(t_spidy *spidy, double angle)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = spidy->dir.x;
	oldplane_x = spidy->plane.x;
	spidy->dir.x = spidy->dir.x * cos(angle) - spidy->dir.y * sin(angle);
	spidy->dir.y = olddir_x * sin(angle) + spidy->dir.y * cos(angle);
	spidy->plane.x = spidy->plane.x * cos(angle) - spidy->plane.y * sin(angle);
	spidy->plane.y = oldplane_x * sin(angle) + spidy->plane.y * cos(angle);
}

void	update_player_position(t_game *g)
{
	if (g->keys.a)
		rotate_spidy(&g->spider, -ROT_SPEED);
	if (g->keys.d)
		rotate_spidy(&g->spider, ROT_SPEED);
	if (g->keys.w)
		move_player(g, 1.0);
	if (g->keys.s)
		move_player(g, -1.0);
	if (g->keys.space)
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
}
