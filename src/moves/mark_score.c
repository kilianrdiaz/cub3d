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
	max.x = sprites[0].pos.x;
	min = sprites[0].pos.x + (g->font.char_w * g->font.scale) - 10;
	i = -1;
	while (++i < num_sprites)
		if (sprites[i].pos.x > max.x)
			max.x = sprites[i].pos.x;
	max.y = g->font.char_h * g->font.scale / num_sprites;
	max.x += g->font.char_w * g->font.scale;
	if (g->spider.pos.x < min)
		g->spider.pos.x = min;
	if (g->spider.pos.x > max.x)
		g->spider.pos.x = max.x;
	if (g->spider.pos.y < 0)
		g->spider.pos.y = 0;
	if (g->spider.pos.y > max.y)
		g->spider.pos.y = max.y;
}

int	mark_letter(t_game *g, t_sprite *sprites, t_ray ray)
{
	int			i;
	t_coords	x;
	t_sprite	target;

	// Coordenadas reales del centro del target (en pantalla)
	x.x = ray.d_start.x + ray.d_end.x / 2.0f;
	x.y = ray.d_start.y + ray.d_end.y / 2.0f;
	i = -1;
	while (++i < 28)
	{
		target.trans.x = g->font.char_w * g->font.scale;
		target.trans.y = g->font.char_h * g->font.scale;
		if (x.x >= sprites[i].pos.x && x.x <= sprites[i].pos.x + target.trans.x
			&& x.y >= sprites[i].pos.y && x.y <= sprites[i].pos.y
			+ target.trans.y)
		{
			g->keys.space = 0;
			return (i);
		}
	}
	return (-1);
}

static void	update_web_target_position(t_game *g, t_sprite *sprites)
{
	if (g->keys.a)
		g->spider.pos.x -= MOVE_MARK;
	if (g->keys.d)
		g->spider.pos.x += MOVE_MARK;
	if (g->keys.w)
		g->spider.pos.y += MOVE_SPEED;
	if (g->keys.s)
		g->spider.pos.y -= MOVE_SPEED;
	if (g->keys.space)
		g->spider.state = ATTACKING;
	else if (g->spider.state == ATTACKING)
		g->spider.state = ACTIVE;
	check_limits(g, sprites, 28);
}

static char	*save_name(t_game *g, char *name)
{
	char	*full_line;
	char	*score_str;

	full_line = ft_strtrim(name, "-");
	if (!full_line)
	{
		ft_putendl_fd("Error: Memory allocation failed", 2);
		return (NULL);
	}
	full_line = ft_strappend(full_line, " ");
	score_str = ft_itoa(g->score);
	if (!score_str)
		ft_putendl_fd("Error: Memory allocation failed", 2);
	full_line = ft_strappend(full_line, score_str);
	free(score_str);
	if (!full_line)
	{
		ft_putendl_fd("Error: Memory allocation failed", 2);
		return (NULL);
	}
	return (full_line);
}

char	*set_name(t_game *g, t_sprite *alphabet, t_ray ray)
{
	int			ch;
	static char	name[7] = "------";
	static int	index = 0;
	t_pos		pos;

	update_web_target_position(g, alphabet);
	g->font.scale = 2.5;
	pos.x = (WIDTH - g->font.char_w * g->font.scale) / 7;
	pos.y = (HEIGHT - g->font.char_h * g->font.scale) - 100;
	render_text(g, name, pos);
	if (g->spider.state != ATTACKING)
		return (NULL);
	ch = mark_letter(g, alphabet, ray);
	if (ch == -1)
		return (NULL);
	if (ch < 26 && index < 6) // A-Z
		name[index++] = 'A' + ch;
	else if (ch == 26) // DEL
	{
		if (index > 0)
			name[--index] = '-';
	}
	else if (ch == 27)
		return (save_name(g, name));
	return (NULL);
}
