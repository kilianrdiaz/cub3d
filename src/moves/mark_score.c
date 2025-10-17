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

static void	check_limits(t_game *g, t_sprite *sprites, int num_sprites)
{
	int		i;
	t_pos	max;
	int		min;

	if (!sprites)
		return ;
	g->font.scale = 3.5;
	max.x = sprites[0].x;
	min = sprites[0].x + (g->font.char_w * g->font.scale) - 10;
	i = -1;
	while (++i < num_sprites)
		if (sprites[i].x > max.x)
			max.x = sprites[i].x;
	max.y = g->font.char_h * g->font.scale / num_sprites;
	max.x += g->font.char_w * g->font.scale;
	if (g->spider.x < min)
		g->spider.x = min;
	if (g->spider.x > max.x)
		g->spider.x = max.x;
	if (g->spider.y < 0)
		g->spider.y = 0;
	if (g->spider.y > max.y)
		g->spider.y = max.y;
}

static void	mark_letter(t_game *g, t_sprite *sprites, t_ray ray)
{
	int		i;
	float	w;
	float	h;

	float cx, cy;
	// Coordenadas reales del centro del target (en pantalla)
	cx = ray.draw_start_x + ray.draw_end_x / 2.0f;
	cy = ray.draw_start_y + ray.draw_end_y / 2.0f;
	for (i = 0; i < 28; i++) // 26 letras + 2 botones
	{
		w = g->font.char_w * g->font.scale;
		h = g->font.char_h * g->font.scale;
		if (cx >= sprites[i].x && cx <= sprites[i].x + w && cy >= sprites[i].y
			&& cy <= sprites[i].y + h)
		{
			printf("✅ Marked letter/button at index: %d\n", i);
			// Aquí puedes registrar la letra o acción
			break ;
		}
	}
}

void	update_web_target_position(t_game *g, t_sprite *sprites, t_ray ray)
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
	{
		g->spider.state = ATTACKING;
		mark_letter(g, sprites, ray);
	}
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
	check_limits(g, sprites, 28);
}
