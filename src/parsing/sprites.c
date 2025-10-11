/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:32:18 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/10 19:32:21 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static int	set_direction(t_game *g, char c)
{
	if (c == 'N' || c == 'P')
		return (g->spider.dir_x = 0, g->spider.dir_y = -1,
			g->spider.plane_x = 0.66, g->spider.plane_y = 0, 0);
	if (c == 'S')
		return (g->spider.dir_x = 0, g->spider.dir_y = 1, g->spider.plane_x
			= -0.66, g->spider.plane_y = 0, 0);
	if (c == 'E')
		return (g->spider.dir_x = 1, g->spider.dir_y = 0, g->spider.plane_x = 0,
			g->spider.plane_y = 0.66, 0);
	if (c == 'W')
		return (g->spider.dir_x = -1, g->spider.dir_y = 0,
			g->spider.plane_x = 0, g->spider.plane_y = -0.66, 0);
	return (-1);
}

void	create_spiderman(t_game *g)
{
	t_pos	p;
	char	c;

	p.y = -1;
	while (g->map[++p.y])
	{
		p.x = -1;
		while (g->map[p.y][++p.x])
		{
			c = g->map[p.y][p.x];
			if (c == 'N' || c == 'S' || c == 'E' || c == 'W' || c == 'P')
			{
				ft_bzero(&g->spider, sizeof(t_spidy));
				g->spider.x = p.x + 0.5;
				g->spider.y = p.y + 0.5;
				set_direction(g, c);
				return ;
			}
		}
	}
	ft_error_exit("Error: No player start position found in map\n");
}

static void	create_sprite(t_game *g, t_pos p, char type)
{
	t_sprite	*sprite;

	sprite = ft_safe_calloc(1, sizeof(t_sprite));
	sprite->x = p.x;
	sprite->y = p.y;
	if (type == 'L')
	{
		sprite->over_scale = 1.5;
		ft_append_array((void ***)&g->lizards, sprite);
	}
	else
	{
		ft_append_array((void ***)&g->bombs, sprite);
		g->bomb_count++;
	}
}

void	create_sprites(t_game *g)
{
	t_pos	p;

	p.y = -1;
	while (g->map[++p.y])
	{
		p.x = -1;
		while (g->map[p.y][++p.x])
		{
			if (g->map[p.y][p.x] == 'B' || g->map[p.y][p.x] == 'L')
				create_sprite(g, p, g->map[p.y][p.x]);
		}
	}
}
