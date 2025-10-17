/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   high_scores.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 16:02:07 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 16:02:12 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	add_buttons(t_game *g, t_sprite *alphabet, int index, t_pos pos)
{
	pos.y += g->font.scale * g->font.char_h - 40;
	g->font.scale = 0.75;
	alphabet[index].x = pos.x;
	alphabet[index].y = pos.y;
	render_text(g, "DEL", pos);
	index++;
	pos.x += g->font.char_w * 4 * g->font.scale + 10;
	alphabet[index].x = pos.x;
	alphabet[index].y = pos.y;
	render_text(g, "END", pos);
	g->font.scale = 3.5;
}

static t_sprite	*print_alphabet(t_game *game, t_tex score_panel)
{
	char		x;
	int			index;
	t_sprite	*alphabet;
	t_pos		pos;

	alphabet = ft_safe_calloc(sizeof(t_sprite), 28);
	pos.x = (WIDTH - score_panel.width) / 2 - 60;
	pos.y = (HEIGHT - score_panel.height) / 2 + 120;
	index = -1;
	x = 'A' - 1;
	while (++x <= 'Z' && ++index < 26)
	{
		alphabet[index].x = pos.x;
		alphabet[index].y = pos.y;
		render_text(game, &x, pos);
        pos.x += game->font.char_w * game->font.scale + 10;
		if ((x - 'A' + 1) % 9 == 0)
		{
			pos.x = (WIDTH - score_panel.width) / 2 - 60;
			pos.y += game->font.char_h * game->font.scale + 10;
		}
	}
	add_buttons(game, alphabet, index, pos);
	return (alphabet);
}

static t_ray	ray_web_target(t_game *g, t_tex web_target, float scale)
{
    t_ray	ray;

    ft_bzero(&ray, sizeof(t_ray));
    ray.draw_end_x = g->spider.hand[ACTIVE].width * SCALE_SPRITE;
    ray.draw_end_y = g->spider.hand[ACTIVE].height * SCALE_SPRITE;
    ray.draw_start_x = g->spider.x - ray.draw_end_x / 2;
    ray.draw_start_y = HEIGHT - ray.draw_end_y;
    // 2. Calculamos la escala según el tamaño de las letras
    ray.draw_end_x = web_target.width * scale;
    ray.draw_end_y = web_target.height * scale;
    // 3. Ajuste vertical (por encima de la mano)
    ray.line_height = -ray.draw_end_y * (1.0 + g->spider.y * 0.5);
    // 4. Posición: centrado respecto a la mano
    ray.draw_start_x = ray.draw_start_x + (ray.draw_end_x / 1.5) 
                        - (ray.draw_end_x / 2);
    ray.draw_start_y = ray.draw_start_y + ray.line_height;
    return (ray);
}

void	draw_web_target(t_game *g, t_tex *web_target)
{
	t_ray	ray;
	float	scale;
	t_pos	pos;

    scale = (g->font.char_h * g->font.scale) / (float)web_target->height;
	ray = ray_web_target(g, *web_target, scale);
	// 5. Dibujado con reescalado por muestreo sencillo
	pos.y = -1;
	while (++pos.y < ray.draw_end_y)
	{
		pos.x = -1;
		while (++pos.x < ray.draw_end_x)
		{
			ray.tx = pos.x / scale;
			ray.ty = pos.y / scale;
			ray.color = *((unsigned int *)(web_target->addr + (ray.ty
							* web_target->line_len + ray.tx * (web_target->bpp
								/ 8))));
			if ((ray.color & 0x00FFFFFF) != 0)
				put_pixel(g, ray.draw_start_x + pos.x, ray.draw_start_y + pos.y,
					ray.color);
		}
	}
}

int	register_score(t_game *g)
{
	t_tex score_panel;
	t_tex web_target;
	t_sprite *alphabet;
	t_pos pos;

	g->render_state = HIGH_SCORE;
	load_texture(g, &score_panel, "./textures/score_panel.xpm");
	load_texture(g, &web_target, "./textures/web_target.xpm");
	draw_fullscreen_image(g, &score_panel);
	pos.x = (WIDTH - score_panel.width) / 2;
	pos.y = (HEIGHT - score_panel.height) / 2;
	g->font.scale = 1.5;
	render_text(g, "ENTER NAME", pos);
	g->font.scale = 3.5;
	alphabet = print_alphabet(g, score_panel);
	draw_hand(g, g->spider.x);
	draw_web_target(g, &web_target);
	free(alphabet);
	g->font.scale = 2.5;
	render_text(g, "-----", (t_pos){pos.x, pos.y + 700});
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	mlx_destroy_image(g->mlx, score_panel.img);
	mlx_destroy_image(g->mlx, web_target.img);
	return (0);
}
