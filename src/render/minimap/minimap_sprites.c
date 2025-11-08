/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_sprites.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:25 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/08 19:20:06 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static int show_sprite(t_game *g, t_sprite *sp)
{
    double dist;

    if (!sp || sp->state == DEFUSED)
        return 0;

    // calcular distancia del sprite al jugador
    dist = sqrt(pow(g->spider.pos.x - sp->pos.x, 2) +
                pow(g->spider.pos.y - sp->pos.y, 2));

    // solo mostrar si está dentro del radio de revelación del minimapa
    if (dist <= REVEAL_STEP_RADIUS)
        return 1;

    return 0;
}



static void draw_single_sprite(t_game *g, t_sprite *sp, unsigned int color)
{
    t_sprite    r;
    t_minimap   *m;
    int         size_tile;
    int         screen_x;
    int         screen_y;

	if (!show_sprite(g, sp))
		return ;

    m = &g->minimap;
    size_tile = m->tile_size;

    // posición centrada del sprite
    screen_x = m->offset.x + (int)(sp->pos.x * size_tile + size_tile / 2) - m->cam_x;
    screen_y = m->offset.y + (int)(sp->pos.y * size_tile + size_tile / 2) - m->cam_y;

    // recorte
    if (screen_x < m->offset.x || screen_x > m->offset.x + MINIMAP_SIZE_LIMIT)
        return;
    if (screen_y < m->offset.y || screen_y > m->offset.y + MINIMAP_SIZE_LIMIT)
        return;

    r.width = (int)(size_tile * 0.5);
    if (r.width < 2)
        r.width = 2;
    r.height = r.width;

    r.pos.x = screen_x - r.width / 2;
    r.pos.y = screen_y - r.height / 2;

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
