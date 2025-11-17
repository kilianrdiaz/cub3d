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

#define NO_PLAYER "Error: No player start position found in map\n"

static void	set_direction(t_game *g, char c)
{
	if (c == 'N' || c == 'P')
	{
		g->spider.dir = (t_coords){0, -1};
		g->spider.plane = (t_coords){0.66, 0};
	}
	else if (c == 'S')
	{
		g->spider.dir = (t_coords){0, 1};
		g->spider.plane = (t_coords){-0.66, 0};
	}
	else if (c == 'E')
	{
		g->spider.dir = (t_coords){1, 0};
		g->spider.plane = (t_coords){0, 0.66};
	}
	else if (c == 'W')
	{
		g->spider.dir = (t_coords){-1, 0};
		g->spider.plane = (t_coords){0, -0.66};
	}
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
				g->spider.pos.x = p.x + 0.5;
				g->spider.pos.y = p.y + 0.5;
				set_direction(g, c);
				return ;
			}
		}
	}
	set_error_parsing(g, NO_PLAYER, NULL);
}

static void	create_sprite(t_game *g, t_pos p, char type)
{
	t_sprite	*sprite;

	sprite = ft_safe_calloc(1, sizeof(t_sprite));
	sprite->pos = (t_coords){p.x, p.y};
	if (type == 'L')
	{
		sprite->type = LIZARD;
		sprite->scale = SCALE_LIZARD;
		ft_append_array((void ***)&g->lizards, sprite);
	}
	else
	{
		sprite->scale = SCALE_BOMB;
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
