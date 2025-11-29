/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spider.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:14:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/29 15:12:25 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

#define SAFETY_OFFSET 0.2

static int	is_walkable(int x, int y, char **map)
{
	if (y < 0 || x < 0 || !map[y] || x >= (int)ft_strlen(map[y]))
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

static double	move_player(t_game *g, double dir)
{
	t_coords	new_pos;
	t_coords	check_pos;
	double		distance;

	new_pos.x = g->spider.pos.x + g->spider.dir.x * dir;
	new_pos.y = g->spider.pos.y + g->spider.dir.y * dir;
	distance = sqrt(pow(new_pos.x - g->spider.pos.x, 2) + pow(new_pos.y
				- g->spider.pos.y, 2));
	if (dir > 0)
		dir += SAFETY_OFFSET;
	else if (dir < 0)
		dir -= SAFETY_OFFSET;
	check_pos.x = g->spider.pos.x + g->spider.dir.x * dir;
	check_pos.y = g->spider.pos.y + g->spider.dir.y * dir;
	if ((int)check_pos.x != (int)g->spider.pos.x
		&& (int)check_pos.y != (int)g->spider.pos.y)
	{
		if (!is_walkable((int)check_pos.x, (int)g->spider.pos.y, g->map)
			&& !is_walkable((int)g->spider.pos.x, (int)check_pos.y, g->map))
			return (0);
	}
	if (!is_walkable((int)check_pos.x, (int)check_pos.y, g->map))
		return (0);
	g->spider.pos = new_pos;
	return (distance);
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

int	mouse_rotation(int x, int y, t_game *g)
{
	static int	first = 1;
	int			dx;
	double		angle;

	(void)y;
	if (!g || !g->mlx || !g->win)
		return (0);
	if (first)
	{
		first = 0;
		mlx_mouse_move(g->mlx, g->win, WIDTH / 2, HEIGHT / 2);
		return (0);
	}
	dx = x - WIDTH / 2;
	if (dx == 0)
		return (0);
	angle = dx * MOUSE_SENSITIVITY;
	rotate_spidy(&g->spider, angle);
	mlx_mouse_move(g->mlx, g->win, WIDTH / 2, HEIGHT / 2);
	return (0);
}

void	update_player_position(t_game *g)
{
	if (g->keys.a)
		rotate_spidy(&g->spider, -ROT_SPEED);
	if (g->keys.d)
		rotate_spidy(&g->spider, ROT_SPEED);
	if (g->keys.w)
		g->spider.move_accum += move_player(g, MOVE_SPEED);
	if (g->keys.s)
		g->spider.move_accum += move_player(g, -MOVE_SPEED);
	if (g->keys.space)
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
	if (g->spider.move_accum < 0.40)
		return ;
	if (g->spider.state == ACTIVE)
		g->spider.state = MOVING;
	else if (g->spider.state == MOVING)
		g->spider.state = ACTIVE;
	g->spider.move_accum = 0.0;
}
