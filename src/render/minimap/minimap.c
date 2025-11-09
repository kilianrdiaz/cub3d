/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/29 21:38:36 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/09 15:13:44 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

static void	init_revealed_if_needed(t_minimap *m, int w, int h)
{
	int	i;

	if (m->revealed && w == m->width && h == m->height)
		return ;
	m->revealed = ft_calloc(sizeof(char *), h + 1);
	if (!m->revealed)
		return ;
	i = -1;
	while (++i < h)
	{
		m->revealed[i] = ft_calloc(sizeof(char), w);
		if (!m->revealed[i])
			break ;
	}
	m->width = w;
	m->height = h;
}

static void	reveal_radius(t_minimap *m, int cx, int cy, int r)
{
	t_pos	pos;

	if (!m->revealed)
		return ;
	pos.y = fmax(0, cy - r) - 1;
	while (++pos.y <= fmin(m->height - 1, cy + r))
	{
		pos.x = fmax(0, cx - r) - 1;
		while (++pos.x <= fmin(m->width - 1, cx + r))
			if (sqrt((pos.x - cx) * (pos.x - cx) + (pos.y - cy) * (pos.y
						- cy)) <= r + 0.4)
				m->revealed[pos.y][pos.x] = 1;
	}
}

static void	draw_map_tiles(t_game *g)
{
	t_pos	pos;

	pos.y = -1;
	while (g->map[++pos.y])
	{
		pos.x = -1;
		while (g->map[pos.y][++pos.x])
			draw_tile(g, pos);
	}
}

void draw_minimap(t_game *g)
{
    int         h;
    int         w;
    t_minimap   *m;
	int         bar_width;

    m = &g->minimap;
    h = ft_memlen(g->map);
    w = ft_strlen(g->map[0]);

    if (!m->revealed || m->width != w || m->height != h)
        init_revealed_if_needed(m, w, h);

    reveal_radius(m, (int)g->spider.pos.x,
        (int)g->spider.pos.y, REVEAL_STEP_RADIUS);

    m->tile_size = MINIMAP_TILE;
    m->real_w = w * m->tile_size;
    m->real_h = h * m->tile_size;

    // offset dentro de la franja negra
       bar_width = WIDTH - GAME_WIDTH;

    m->visible_size = 300; // tu tamaño actual

    m->offset.x = GAME_WIDTH + (bar_width / 2) - (m->visible_size / 2);
    m->offset.y = (HEIGHT / 2) - (m->visible_size - 100);
    // límite máximo visible (cuadrado 300x300)
    m->visible_size = 300;

    /* --- Cámara del minimapa (scroll) --- */
    m->cam_x = (int)(g->spider.pos.x * m->tile_size) - m->visible_size / 2;
    m->cam_y = (int)(g->spider.pos.y * m->tile_size) - m->visible_size / 2;

    // evitar que la cámara salga del mapa
    if (m->cam_x < 0)
        m->cam_x = 0;
    if (m->cam_y < 0)
        m->cam_y = 0;
    if (m->cam_x > m->real_w - m->visible_size)
        m->cam_x = m->real_w - m->visible_size;
    if (m->cam_y > m->real_h - m->visible_size)
        m->cam_y = m->real_h - m->visible_size;

    draw_map_tiles(g);
    draw_sprites_minimap(g);
    draw_player_arrow(g, *m);
}



