/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_scores.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/19 17:18:02 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/19 17:18:09 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define SCORE_SPACING 100

static int	close_high_scores(int keycode, t_game *g)
{
	(void)keycode;
	if (g->render_state == HIGH_SCORE)
	{
		g->render_state = END;
		ft_bzero(&g->keys, sizeof(t_keys));
		clean_screen(g);
		mlx_hook(g->win, 2, 1L << 0, key_press, g);
		mlx_hook(g->win, 3, 1L << 1, key_release, g);
	}
	return (0);
}

static void	print_separator_line(t_game *g, t_coords coords, char **line)
{
	int		i;
	char	*separator;
	int		separator_length;

	separator_length = (WIDTH - SCORE_SPACING) - (ft_strlen(line[0])
			* g->font.char_w * g->font.scale) - (ft_strlen(line[1])
			* g->font.char_w * g->font.scale);
	separator_length /= (g->font.char_w * g->font.scale);
	separator = ft_safe_calloc(sizeof(char), separator_length + 1);
	if (!separator)
		return ;
	i = 0;
	while (i < separator_length - 1)
		separator[i++] = '-';
	separator[i] = '\0';
	coords.x += ft_strlen((*line)) * g->font.char_w * g->font.scale;
	render_text(g, separator, coords);
	free(separator);
}

static void	put_line_scores(t_game *g, char *score, t_coords coords)
{
	char	**line;
	int		name_x;
	int		score_col_x;

	line = ft_split(score, ' ');
	if (!line)
		return ;
	else if (!line[0] || !line[1])
	{
		ft_free_array((void ***)&line);
		return ;
	}
	g->font.scale = 2.5;
	score_col_x = WIDTH - SCORE_SPACING;
	name_x = SCORE_SPACING;
	coords.x = name_x;
	render_text(g, line[0], coords);
	print_separator_line(g, coords, line);
	coords.x = score_col_x - ft_strlen(line[1]) * g->font.char_w
		* g->font.scale;
	render_text(g, line[1], coords);
	ft_free_array((void ***)&line);
}

void	display_score_panel(t_game *g, t_tex score_panel, char **scores)
{
	int			index;
	t_coords	pos;

	clean_screen(g);
	g->font.scale = 3.0;
	draw_fullscreen_image(g, score_panel);
	pos.x = WIDTH / 2 - (ft_strlen("HIGH SCORES") * g->font.char_w
			* g->font.scale) / 2;
	pos.y = 50;
	render_text(g, "HIGH SCORES", (t_coords){(double)pos.x, (double)pos.y});
	if (!scores)
		return ;
	index = -1;
	pos.y += g->font.char_h * g->font.scale + 90;
	while (scores[++index])
	{
		put_line_scores(g, scores[index], pos);
		pos.y += g->font.char_h * g->font.scale + 20;
		g->font.scale = 3.0;
	}
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	if (score_panel.img)
		mlx_destroy_image(g->mlx, score_panel.img);
	mlx_key_hook(g->win, close_high_scores, g);
}
