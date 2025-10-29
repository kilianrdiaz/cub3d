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

static void	draw_single_sprite(t_game *g, t_minimap *m,
		t_sprite *sp, t_sprite_info *inf)
{
	t_position	p;
	t_rect		r;

	if (!sp || sp->state == DEFUSED)
		return ;
	p.mx = (int)sp->x;
	p.my = (int)sp->y;
	if (!m->revealed || p.mx < 0 || p.my < 0
		|| p.mx >= m->width || p.my >= m->height
		|| !m->revealed[p.my][p.mx])
		return ;
	p.sx = inf->ox + (int)(sp->x * inf->t + inf->t / 2);
	p.sy = inf->oy + (int)(sp->y * inf->t + inf->t / 2);
	p.s = (int)(inf->t * 0.5);
	if (p.s < 2)
		p.s = 2;
	r.x = p.sx - p.s / 2;
	r.y = p.sy - p.s / 2;
	r.w = p.s;
	r.h = p.s;
	r.c = inf->color;
	put_rect(g, r);
}

void	draw_sprites_minimap(t_game *g, t_minimap *m,
		t_sprite **arr, t_sprite_info *inf)
{
	int	i;

	i = -1;
	while (arr && arr[++i])
		draw_single_sprite(g, m, arr[i], inf);
}
