/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 16:03:03 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 16:03:54 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int	show_sprite(t_game *g, t_sprite *sp)
{
	double	dx;
	double	dy;
	double	dist;

	if (!sp || sp->state == DEFUSED)
		return (0);
	dx = g->spider.pos.x - sp->pos.x;
	dy = g->spider.pos.y - sp->pos.y;
	dist = sqrt(dx * dx + dy * dy);
	return (dist <= REVEAL_STEP_RADIUS);
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
