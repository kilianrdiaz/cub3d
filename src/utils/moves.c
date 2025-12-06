/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 14:33:41 by alejhern          #+#    #+#             */
/*   Updated: 2025/12/06 14:33:52 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	is_walkable(int x, int y, char **map)
{
	if (y < 0 || x < 0 || !map[y] || x >= (int)ft_strlen(map[y]))
		return (0);
	if (map[y][x] == '1')
		return (0);
	return (1);
}

int	slide_player(t_game *g, t_coords *new_pos, t_coords check_pos)
{
	t_pos	slide_pos;

	if (is_walkable((int)check_pos.x, (int)check_pos.y, g->map))
		return (1);
	slide_pos.x = is_walkable((int)check_pos.x, (int)g->spider.pos.y, g->map);
	slide_pos.y = is_walkable((int)g->spider.pos.x, (int)check_pos.y, g->map);
	if (slide_pos.x && !slide_pos.y)
		new_pos->y = g->spider.pos.y;
	else if (slide_pos.y && !slide_pos.x)
		new_pos->x = g->spider.pos.x;
	if (!is_walkable((int)new_pos->x, (int)new_pos->y, g->map))
		return (0);
	return (1);
}
