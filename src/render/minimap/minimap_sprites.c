/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:03:03 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 20:20:13 by kroyo-di         ###   ########.fr       */
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
			if (g->live.mask_sprite.delay < g->timer)
			{
				g->live.mask_sprite.delay = g->timer + 7;
				g->spider.spider_sense = 1;
			}
			if (g->live.mask_sprite.delay == g->timer)
			{
				g->spider.spider_sense = !g->spider.spider_sense;
				g->live.mask_sprite.delay = g->timer + 7;
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
	t_pos		src;
	int			ts;

	if (!show_sprite(g, sp))
		return ;
	m = &g->minimap;
	ts = MINIMAP_TILE;
	src.x = m->offset.x + (int)(sp->pos.x * ts + ts / 2) - m->cam_x;
	src.y = m->offset.y + (int)(sp->pos.y * ts + ts / 2) - m->cam_y;
	if (!in_bounds(src.x, src.y, m))
		return ;
	r.width = ts * 0.5;
	if (r.width < 2)
		r.width = 2;
	r.height = r.width;
	r.pos.x = src.x - r.width / 2;
	r.pos.y = src.y - r.height / 2;
	put_rect(g, r, color);
}

void	draw_sprites_minimap(t_game *g)
{
	int	i;

	i = -1;
	while (g->bombs && g->bombs[++i])
		draw_single_sprite(g, g->bombs[i], COL_BOMB);
	i = -1;
	while (g->lizards && g->lizards[++i])
		draw_single_sprite(g, g->lizards[i], COL_LIZARD);
}
