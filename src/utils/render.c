/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:50 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/19 20:47:55 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	clean_screen(t_game *g)
{
	t_pos	it;

	it.y = -1;
	while (++it.y < HEIGHT)
	{
		it.x = -1;
		while (++it.x < WIDTH)
			put_pixel(g, it.x, it.y, 0x000000);
	}
}

unsigned int	get_pixel_color(t_tex tex, int x, int y)
{
	unsigned int	color;
	char			*dst;

	if (tex.color != COLOR_NONE)
		return (tex.color);
	if (x < 0 || x >= tex.width || y < 0 || y >= tex.height)
		return (0);
	dst = tex.addr + (y * tex.line_len + x * (tex.bpp / 8));
	color = *(unsigned int *)dst;
	return (color);
}

void	put_pixel(t_game *g, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = g->addr + (y * g->line_len + x * (g->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

t_sprite	**get_sprites(t_game *g)
{
	t_sprite	**sprites;
	int			count;
	int			x;

	count = g->bomb_count;
	if (g->lizards)
		count += ft_memlen((void **)g->lizards);
	if (count == 0)
		return (NULL);
	sprites = malloc(sizeof(t_sprite *) * (count + 1));
	if (!sprites)
		ft_error_exit("Error: Memory allocation failed for sprites\n");
	sprites[count] = NULL;
	x = -1;
	count = 0;
	while (g->bombs && g->bombs[++x])
	{
		if (g->bombs[x]->state == DEFUSED)
			continue ;
		sprites[count++] = g->bombs[x];
	}
	x = -1;
	while (g->lizards && g->lizards[++x])
		sprites[count++] = g->lizards[x];
	return (sprites);
}

void	recalc_sprite_scale(t_game *g, t_sprite *sp, double dist)
{
	int		base_scale;
	double	target_scale;
	int		max_dist;
	int		int_dist;

	max_dist = 8;
	if (sp->type == LIZARD)
		max_dist = 10;
	dist = sqrt(pow(g->spider.pos.x - sp->pos.x, 2) + pow(g->spider.pos.y
				- sp->pos.y, 2));
	if (dist > max_dist)
	{
		sp->scale += (0.0 - sp->scale) * LERP_SPEED;
		if (sp->scale < 0.01)
			sp->scale = 0.0;
		return ;
	}
	base_scale = 1;
	if (sp->type == LIZARD)
		base_scale = 2;
	int_dist = (int)(dist + 0.5);
	target_scale = base_scale + (max_dist - int_dist) * 0.2;
	if (target_scale < base_scale)
		target_scale = base_scale;
	sp->scale += (target_scale - sp->scale) * LERP_SPEED;
}
