/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark_score.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 12:51:58 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/17 12:52:04 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define MOVE_MARK 10

static void check_limits(t_game *g, t_sprite *sprites, int num_sprites)
{
    int i;
    int max_x = 0;
    int max_y = 0;

    if (!sprites)
        return;
    for (i = 0; i < num_sprites; i++)
    {
        if (sprites[i].x > max_x)
            max_x = sprites[i].x;
    }
    max_y = g->font.char_h * g->font.scale / num_sprites; // Ajuste para altura de texto
    printf("Max limits: x=%d, y=%d\n", max_x, max_y);
    if (g->spider.x < sprites[0].x)
        g->spider.x = sprites[0].x;
    if (g->spider.x > max_x)
        g->spider.x = max_x;
    if (g->spider.y < 0)
        g->spider.y = 0;
    if (g->spider.y > max_y)
        g->spider.y = max_y;
}

void update_web_target_position(t_game *g, t_sprite *sprites)
{
    if (g->keys.a)
		g->spider.x -= MOVE_MARK;
	if (g->keys.d)
		g->spider.x += MOVE_MARK;
	if (g->keys.w)
		g->spider.y += MOVE_SPEED;
	if (g->keys.s)
		g->spider.y -= MOVE_SPEED;
	if (g->keys.space)
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
    check_limits(g, sprites, 28); // 26 letras + 2 botones
    printf("Spider position: x=%.2f, y=%.2f\n", g->spider.x, g->spider.y);
}
