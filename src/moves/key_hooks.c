/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:18:12 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/29 15:20:44 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

int	key_press(int key, t_game *g)
{
	if (key == 119)
		g->keys.w = 1;
	if (key == 97)
		g->keys.a = 1;
	if (key == 115)
		g->keys.s = 1;
	if (key == 100)
		g->keys.d = 1;
	if (key == 65361)
		g->keys.left = 1;
	if (key == 65363)
		g->keys.right = 1;
	if (key == 32)
		g->keys.space = 1;
	if (key == 112)
		g->keys.p = 1;
	if (key == 65307)
		close_program(g);
	return (0);
}

int	key_release(int key, t_game *g)
{
	if (key == 119)
		g->keys.w = 0;
	if (key == 97)
		g->keys.a = 0;
	if (key == 115)
		g->keys.s = 0;
	if (key == 100)
		g->keys.d = 0;
	if (key == 65361)
		g->keys.left = 0;
	if (key == 65363)
		g->keys.right = 0;
	if (key == 32)
		g->keys.space = 0;
	if (key == 112)
		g->keys.p = 0;
	return (0);
}

int	mouse_rotation(int x, int y, t_game *g)
{
	int			dx;
	double		angle;

	(void)y;
	if (!g || !g->mlx || !g->win)
		return (0);
	if (g->mouse_enabled == 0)
	{
		mlx_mouse_move(g->mlx, g->win, WIDTH / 2, HEIGHT / 2);
		g->mouse_enabled = 1;
		return (0);
	}
	dx = x - WIDTH / 2;
	if (dx == 0)
		return (0);
	angle = dx * MOUSE_SENSITIVITY;
	update_player_position(g, angle);
	mlx_mouse_move(g->mlx, g->win, WIDTH / 2, HEIGHT / 2);
	return (0);
}
