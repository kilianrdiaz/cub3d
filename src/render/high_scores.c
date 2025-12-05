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

#define TARGET_FILE "./textures/web_target.xpm"
#define SCORE_PANEL "./textures/score_panel.xpm"
#define SCORE_FILE "scores.txt"

static t_ray	ray_web_target(t_game *g, t_tex web_target, float scale)
{
	t_ray	ray;

	ft_bzero(&ray, sizeof(t_ray));
	ray.d_end.x = g->spider.hand[ACTIVE].width * SCALE_SPRITE;
	ray.d_end.y = g->spider.hand[ACTIVE].height * SCALE_SPRITE;
	ray.d_start.x = g->spider.pos.x - ray.d_end.x / 2;
	ray.d_start.y = HEIGHT - ray.d_end.y;
	ray.d_end.x = web_target.width * scale;
	ray.d_end.y = web_target.height * scale;
	ray.line_height = -ray.d_end.y * (1.0 + g->spider.pos.y * 0.5);
	ray.d_start.x = ray.d_start.x + (ray.d_end.x / 1.5) - (ray.d_end.x / 2);
	ray.d_start.y = ray.d_start.y + ray.line_height;
	return (ray);
}

static t_ray	draw_web_target(t_game *g, t_tex *web_target)
{
	t_ray			ray;
	double			scale;
	t_pos			pos;
	unsigned int	color;

	scale = (g->font.char_h * g->font.scale) / (double)web_target->height;
	ray = ray_web_target(g, *web_target, scale);
	pos.y = -1;
	while (++pos.y < ray.d_end.y)
	{
		pos.x = -1;
		while (++pos.x < ray.d_end.x)
		{
			ray.src.x = pos.x / scale;
			ray.src.y = pos.y / scale;
			color = get_pixel_color(*web_target, ray.src.x, ray.src.y);
			if ((color & 0x00FFFFFF) != 0)
				put_pixel(g, ray.d_start.x + pos.x, ray.d_start.y + pos.y,
					color);
		}
	}
	return (ray);
}

static char	*register_score(t_game *g, t_tex score_panel)
{
	t_tex		target;
	t_sprite	*alphabet;
	char		*result;
	t_ray		ray;

	clean_screen(g);
	ft_bzero(&target, sizeof(t_tex));
	load_texture(g, &target, TARGET_FILE);
	if (!target.img)
		return (g->render_state = SCORE_SAVED, NULL);
	draw_fullscreen_image(g, score_panel);
	g->font.scale = 1.5;
	render_text(g, "ENTER NAME", (t_coords){WIDTH / 10, 50});
	alphabet = print_alphabet(g);
	draw_hand(g, g->spider.pos.x);
	ray = draw_web_target(g, &target);
	result = set_name(g, alphabet, ray);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (target.img)
		mlx_destroy_image(g->mlx, target.img);
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

void	show_high_scores(t_game *g)
{
	char	**scores;
	int		position;
	char	*new_score;

	if (!g->timer && ++g->timer)
		load_texture(g, &g->wallpaper, SCORE_PANEL);
	scores = get_scores();
	position = get_position(g, scores);
	if (position != -1 && g->render_state != SCORE_SAVED && g->score)
		g->render_state = WAITING_FOR_NAME;
	if (g->render_state == HIGH_SCORE || g->render_state == SCORE_SAVED)
		display_score_panel(g, g->wallpaper, scores);
	else
	{
		new_score = register_score(g, g->wallpaper);
		if (new_score)
		{
			update_scores(scores, position);
			scores[position] = new_score;
			save_scores(scores);
			g->render_state = SCORE_SAVED;
		}
	}
	ft_free_array((void ***)&scores);
}
