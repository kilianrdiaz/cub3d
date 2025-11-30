/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:03:03 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/30 15:37:46 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int	show_sprite(t_sprite *sp, int *enemy_nearby)
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
			*enemy_nearby = 1;
			return (1);
		}
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

static void	draw_single_sprite(t_game *g, t_sprite *sp, unsigned int color,
		int *enemy_nearby)
{
	t_minimap	*m;
	t_sprite	r;
	t_pos		src;
	int			ts;

	if (!show_sprite(sp, enemy_nearby))
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

static void	update_spider_sense(t_game *g, int enemy_nearby)
{
	if (enemy_nearby)
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
	}
	else
	{
		g->spider.spider_sense = 0;
	}
}

void	draw_sprites_minimap(t_game *g)
{
	int	i;
	int	enemy_nearby;

	enemy_nearby = 0;
	i = -1;
	while (g->bombs && g->bombs[++i])
		draw_single_sprite(g, g->bombs[i], COL_BOMB, &enemy_nearby);
	i = -1;
	while (g->lizards && g->lizards[++i])
		draw_single_sprite(g, g->lizards[i], COL_LIZARD, &enemy_nearby);
	update_spider_sense(g, enemy_nearby);
}
