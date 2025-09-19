/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 17:14:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/18 17:14:05 by alejhern         ###   ########.fr       */
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
	int	newX;
	int	newY;

	newX = (int)g->spider.posX + (int)g->spider.dirX * forward;
	newY = (int)g->spider.posY + (int)g->spider.dirY * forward;
	if (is_walkable(newX, newY))
	{
		g->spider.posX = newX + 0.5; // centro de celda
		g->spider.posY = newY + 0.5;
		print_map(g); // depuración
	}
}

void	rotate_left(t_spidy *spidy)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = spidy->dirX;
	oldPlaneX = spidy->planeX;
	// Rotar 90° = aplicar matriz con cos(π/2), sin(π/2)
	spidy->dirX = -spidy->dirY;
	spidy->dirY = oldDirX;
	spidy->planeX = -spidy->planeY;
	spidy->planeY = oldPlaneX;
}

void	rotate_right(t_spidy *spidy)
{
	double	oldDirX;
	double	oldPlaneX;

	oldDirX = spidy->dirX;
	oldPlaneX = spidy->planeX;
	// Rotar -90° = aplicar matriz con cos(-π/2), sin(-π/2)
	spidy->dirX = spidy->dirY;
	spidy->dirY = -oldDirX;
	spidy->planeX = spidy->planeY;
	spidy->planeY = -oldPlaneX;
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
