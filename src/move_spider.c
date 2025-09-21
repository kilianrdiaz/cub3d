/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_spider.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:14:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/20 12:27:47 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static int	is_walkable(int x, int y)
{
	if (x < 0 || y < 0)
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

static void	move_player_cell(t_game *g, int forward)
{
	int		new_x;
	int		new_y;
	t_tex	hand;

	new_x = (int)g->spider.x + (int)g->spider.dir_x * forward;
	new_y = (int)g->spider.y + (int)g->spider.dir_y * forward;
	if (is_walkable(new_x, new_y))
	{
		g->spider.x = new_x + 0.5; // centro de celda
		g->spider.y = new_y + 0.5;
		print_map(g); // depuración
		ft_rotate_array((void ***)g->spider.hand);
		hand = g->spider.hand[0];
		g->spider.hand[0] = g->spider.hand[1];
		g->spider.hand[1] = hand;
	}
}

void	rotate_left(t_spidy *spidy)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = spidy->dir_x;
	oldplane_x = spidy->plane_x;
	// Rotar 90° = aplicar matriz con cos(π/2), sin(π/2)
	spidy->dir_x = -spidy->dir_y;
	spidy->dir_y = olddir_x;
	spidy->plane_x = -spidy->plane_y;
	spidy->plane_y = oldplane_x;
}

void	rotate_right(t_spidy *spidy)
{
	double	olddir_x;
	double	oldplane_x;

	olddir_x = spidy->dir_x;
	oldplane_x = spidy->plane_x;
	// Rotar -90° = aplicar matriz con cos(-π/2), sin(-π/2)
	spidy->dir_x = spidy->dir_y;
	spidy->dir_y = -olddir_x;
	spidy->plane_x = spidy->plane_y;
	spidy->plane_y = -oldplane_x;
}

void	update_player_position(t_game *g)
{
	if (g->keys.a)
		rotate_left(&g->spider);
	if (g->keys.d)
		rotate_right(&g->spider);
	if (g->keys.s)
		move_player_cell(g, -1);
	if (g->keys.w)
		move_player_cell(g, 1);
}
