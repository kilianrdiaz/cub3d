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

static int show_sprite(t_game *g, t_sprite *sp)
{
	double	dist;

	if (!sp || sp->state == DEFUSED)
		return (0);
	if (sp->type != LIZARD)
		return (1);
	dist = sqrt(pow(g->spider.pos.x - sp->pos.x, 2)
			+ pow(g->spider.pos.y - sp->pos.y, 2));
	if (dist <= 5)
		return (1);
	return (0);
}

static void	draw_single_sprite(t_game *g, t_minimap *m,
		t_sprite *sp, t_sprite_info *inf)
{
	t_rect		r;

	if (!show_sprite(g, sp))
		return ;
	if (!m->revealed || sp->pos.x < 0 || sp->pos.y < 0
		|| sp->pos.x >= m->width || sp->pos.y >= m->height
		|| !m->revealed[(int)sp->pos.y][(int)sp->pos.x])
		return ;
	r.x = inf->ox + (int)(sp->pos.x * inf->t + inf->t / 2);
	r.y = inf->oy + (int)(sp->pos.y * inf->t + inf->t / 2);
	r.w = (int)(inf->t * 0.5);
	if (r.w < 2)
		r.w = 2;
	r.x -= r.w / 2;
	r.y -= r.w / 2;
	r.h = r.w;
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
