/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:07 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/08 20:04:46 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	put_rect(t_game *g, t_sprite r, unsigned int color)
{
	int	i;
	int	j;

	if (r.width <= 0 || r.height <= 0)
		return ;
	i = -1;
	while (++i < r.height)
	{
		j = -1;
		while (++j < r.width)
			put_pixel(g, r.pos.x + j, r.pos.y + i, color);
	}
}

void	draw_border(t_game *g, t_sprite r, unsigned int color)
{
	int	i;

	if (r.width <= 1 || r.height <= 1)
		return ;
	i = -1;
	while (++i < r.width)
	{
		put_pixel(g, r.pos.x + i, r.pos.y, color);
		put_pixel(g, r.pos.x + i, r.pos.y + r.height - 1, color);
	}
	i = -1;
	while (++i < r.height)
	{
		put_pixel(g, r.pos.x, r.pos.y + i, color);
		put_pixel(g, r.pos.x + r.width - 1, r.pos.y + i, color);
	}
}

void draw_tile(t_game *g, t_pos tile)
{
    t_sprite    r;
    t_minimap   *m;
    int         screen_x;
    int         screen_y;

    m = &g->minimap;
    r.width = m->tile_size;
    r.height = m->tile_size;

    // posición en pantalla restando la cámara
    screen_x = m->offset.x + tile.x * m->tile_size - m->cam_x;
    screen_y = m->offset.y + tile.y * m->tile_size - m->cam_y;

    // recorte: fuera del área visible del minimapa (cuadrado 100x100)
    if (screen_x + r.width < m->offset.x || screen_x > m->offset.x + m->visible_size)
        return;
    if (screen_y + r.height < m->offset.y || screen_y > m->offset.y + m->visible_size)
        return;

    r.pos.x = screen_x;
    r.pos.y = screen_y;

    if (!m->revealed || !m->revealed[tile.y][tile.x])
        put_rect(g, r, COL_UNK_FOG);
    else if (g->map[tile.y][tile.x] == '1')
    {
        put_rect(g, r, COL_WALL);
        draw_border(g, r, COL_WALL_BORDER);
    }
    else
        put_rect(g, r, COL_FLOOR);
}

