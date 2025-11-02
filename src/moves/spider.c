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
	if (y < 0 || x < 0 || !map[y] || x >= (int)ft_strlen(map[y]))
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

static void	move_player(t_game *g, double dir)
{
	t_coords	new_pos;
	double		distance;

	new_pos.x = g->spider.pos.x + g->spider.dir.x * dir;
	new_pos.y = g->spider.pos.y + g->spider.dir.y * dir;
	distance = sqrt(pow(new_pos.x - g->spider.pos.x, 2) + pow(new_pos.y
				- g->spider.pos.y, 2));
	// Movimiento diagonal: revisar esquinas
	if ((int)new_pos.x != (int)g->spider.pos.x
		&& (int)new_pos.y != (int)g->spider.pos.y)
		if (!is_walkable((int)new_pos.x, (int)g->spider.pos.y, g->map)
			&& !is_walkable((int)g->spider.pos.x, (int)new_pos.y, g->map))
			return ;
	if (!is_walkable((int)new_pos.x, (int)new_pos.y, g->map))
		return ;
	g->spider.pos = new_pos;
	g->spider.move_accum += distance;
	if (g->spider.move_accum < 0.40)
		return ;
	if (g->spider.state == ACTIVE)
		g->spider.state = MOVING;
	else if (g->spider.state == MOVING)
		g->spider.state = ACTIVE;
	g->spider.move_accum = 0.0;
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
		move_player(g, MOVE_SPEED);
	if (g->keys.s)
		move_player(g, -MOVE_SPEED);
	if (g->keys.space)
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
}
