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

static t_ray	draw_web_target(t_game *g, t_tex *web_target)
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
			ray.src.x = pos.x / scale;
			ray.src.y = pos.y / scale;
			ray.color = *((unsigned int *)(web_target->addr + (ray.src.y
							* web_target->line_len + ray.src.x * (web_target->bpp
								/ 8))));
			if ((ray.color & 0x00FFFFFF) != 0)
				put_pixel(g, ray.draw_start_x + pos.x, ray.draw_start_y + pos.y,
					ray.color);
		}
	}
	return (ray);
}

static char	*register_score(t_game *g, t_tex *score_panel)
{
	t_tex		target;
	t_sprite	*alphabet;
	char		*result;
	t_ray		ray;

	load_texture(g, &target, "./textures/web_target.xpm");
	draw_fullscreen_image(g, score_panel);
	g->font.scale = 1.5;
	render_text(g, "ENTER NAME", (t_pos){(WIDTH - score_panel->width) / 2,
		(HEIGHT - score_panel->height) / 2});
	alphabet = print_alphabet(g, *score_panel);
	draw_hand(g, g->spider.x);
	ray = draw_web_target(g, &target);
	result = set_name(g, alphabet, ray);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	mlx_destroy_image(g->mlx, target.img);
	mlx_destroy_image(g->mlx, score_panel->img);
	free(alphabet);
	return (result);
}

static void	save_scores(char **scores)
{
	int	fd;
	int	i;

	if (!scores)
		return ;
	fd = open("scores.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
	i = -1;
	while (scores && scores[++i])
	{
		if (!ft_putendl_fd(scores[i], fd))
		{
			ft_putendl_fd("Error: Could not write to scores.txt", 2);
			break ;
		}
	}
	close(fd);
}

int	show_high_scores(t_game *g)
{
	char	**scores;
	int		position;
	t_tex	score_panel;
	char	*new_score;

	load_texture(g, &score_panel, "./textures/score_panel.xpm");
	scores = get_scores();
	position = get_position(g, scores);
	if (position != -1 && g->render_state != SCORE_SAVED)
		g->render_state = WAITING_FOR_NAME;
	if (g->render_state == WAITING_FOR_NAME)
	{
		new_score = register_score(g, &score_panel);
		if (new_score)
		{
			update_scores(scores, position);
			scores[position] = new_score;
			save_scores(scores);
			g->render_state = SCORE_SAVED;
		}
		ft_free_array((void ***)&scores);
		return (0);
	}
	display_score_panel(g, &score_panel, scores);
	return (ft_free_array((void ***)&scores), 0);
}
