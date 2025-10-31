/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/20 13:18:12 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/20 13:34:36 by alejhern         ###   ########.fr       */
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
	return (0);
}
