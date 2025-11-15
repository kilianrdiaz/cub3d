/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:03:03 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 18:00:47 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int	show_sprite(t_game *g, t_sprite *sp)
{
	if (!sp)
		return (0);
	if (sp->dist <= 5)
		sp->found = 1;
	if (sp->type == BOMB && sp->state != DEFUSED && sp->found)
		return (1);
	if (sp->type == LIZARD)
	{
		if (sp->dist <= 5)
		{
			if (g->lives.mask_sprite.delay < g->timer)
			{
				g->lives.mask_sprite.delay = g->timer + 7;
				g->spider.spider_sense = 1;
			}
			if (g->lives.mask_sprite.delay == g->timer)
			{
				g->spider.spider_sense = !g->spider.spider_sense;
				g->lives.mask_sprite.delay = g->timer + 7;
			}
			return (1);
		}
		g->spider.spider_sense = 0;
	}
	return (0);
}

static int	in_bounds(int sx, int sy, t_minimap *m)
{
	if (sx < m->offset.x || sx > m->offset.x + MINIMAP_SIZE_LIMIT)
		return (0);
	if (sy < m->offset.y || sy > m->offset.y + MINIMAP_SIZE_LIMIT)
		return (0);
	return (1);
}

static void	draw_single_sprite(t_game *g, t_sprite *sp, unsigned int color)
{
	t_minimap	*m;
	t_sprite	r;
	int			sx;
	int			sy;
	int			ts;

	if (!show_sprite(g, sp))
		return ;
	m = &g->minimap;
	ts = m->tile_size;
	sx = m->offset.x + (int)(sp->pos.x * ts + ts / 2) - m->cam_x;
	sy = m->offset.y + (int)(sp->pos.y * ts + ts / 2) - m->cam_y;
	if (!in_bounds(sx, sy, m))
		return ;
	r.width = ts * 0.5;
	if (r.width < 2)
		r.width = 2;
	r.height = r.width;
	r.pos.x = sx - r.width / 2;
	r.pos.y = sy - r.height / 2;
	put_rect(g, r, color);
}

void	draw_sprites_minimap(t_game *g)
{
	int				i;
	unsigned int	color;

	i = -1;
	color = COL_BOMB;
	while (g->bombs && g->bombs[++i])
		draw_single_sprite(g, g->bombs[i], color);
	i = -1;
	color = COL_LIZARD;
	while (g->lizards && g->lizards[++i])
		draw_single_sprite(g, g->lizards[i], color);
}
