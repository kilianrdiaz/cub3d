/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:25 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/29 21:46:11 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int	show_sprite(t_game *g, t_sprite *sp)
{
	double	dist;

	if (!sp || sp->state == DEFUSED)
		return (0);
	if (sp->type != LIZARD)
		return (1);
	dist = sqrt(pow(g->spider.pos.x - sp->pos.x, 2) + pow(g->spider.pos.y
				- sp->pos.y, 2));
	if (dist <= 5)
		return (1);
	return (0);
}

static void	draw_single_sprite(t_game *g, t_minimap *m, t_sprite *sp,
		unsigned int color)
{
	t_rect	r;
	t_pos	offset;
	int		size_title;

	if (!show_sprite(g, sp))
		return ;
	if (!m->revealed || sp->pos.x < 0 || sp->pos.y < 0 || sp->pos.x >= m->width
		|| sp->pos.y >= m->height
		|| !m->revealed[(int)sp->pos.y][(int)sp->pos.x])
		return ;
	offset.x = m->offset.x;
	offset.y = m->offset.y;
	size_title = m->tile_size;
	r.x = offset.x + (int)(sp->pos.x * size_title + size_title / 2);
	r.y = offset.y + (int)(sp->pos.y * size_title + size_title / 2);
	r.w = (int)(size_title * 0.5);
	if (r.w < 2)
		r.w = 2;
	r.x -= r.w / 2;
	r.y -= r.w / 2;
	r.h = r.w;
	r.c = color;
	put_rect(g, r);
}

void	draw_sprites_minimap(t_game *g, t_minimap *m)
{
	int				i;
	unsigned int	color;

	i = -1;
	color = COL_BOMB;
	while (g->bombs && g->bombs[++i])
		draw_single_sprite(g, m, g->bombs[i], color);
	i = -1;
	color = COL_LIZARD;
	while (g->lizards && g->lizards[++i])
		draw_single_sprite(g, m, g->lizards[i], color);
}
