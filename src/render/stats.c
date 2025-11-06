/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/02 20:59:06 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/02 20:59:09 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static void	draw_text(t_game *g, t_tex tex, t_sprite sp)
{
	t_pos	p;
	t_pos	src_pos;
	char	*src;

	p.y = -1;
	while (++p.y < tex.height * sp.scale)
	{
		p.x = -1;
		while (++p.x < tex.width * sp.scale)
		{
			// Convertir coordenadas escaladas -> coordenadas reales de textura
			src_pos.x = (int)(p.x / sp.scale);
			src_pos.y = (int)(p.y / sp.scale);
			// Proteger límites
			if (src_pos.x < 0 || src_pos.x >= tex.width || src_pos.y < 0
				|| src_pos.y >= tex.height)
				continue ;
			src = tex.addr + (src_pos.y * tex.line_len + src_pos.x * (tex.bpp
						/ 8));
			tex.color = *(unsigned int *)src;
			if ((tex.color & 0x00FFFFFF) != 0) // transparencia
				put_pixel(g, (int)sp.pos.x + p.x, (int)sp.pos.y + p.y,
					tex.color);
		}
	}
}

static void	put_score_text(t_game *g, t_pos pos)
{
	int		advance;
	int		i;
	char	*str;
	char	ch;

	str = ft_itoa(g->score);
	if (!str)
		return ;
	pos.x -= (g->font.char_w * ft_strlen(str) / 2);
	advance = (g->font.char_w);
	i = -1;
	while (str[++i])
	{
		ch = str[i];
		render_text(g, &ch, (t_coords){pos.x + i * advance, pos.y});
	}
	free(str);
}

void	draw_panel_separator(t_game *g)
{
	t_pos	p;

	p.x = GAME_WIDTH - 1;
	while (++p.x < GAME_WIDTH + 8)
	{
		p.y = -1;
		while (++p.y < HEIGHT)
			put_pixel(g, p.x, p.y, 0xFF555555);
	}
}

void	render_stats(t_game *g)
{
	t_sprite	sp;
	char		*str;

	draw_panel_separator(g);
	sp.pos.x = GAME_WIDTH + 150;
	sp.pos.y = HEIGHT - 300;
	sp.scale = 0.5;
	draw_text(g, g->bomb_tex[ACTIVE], sp);
	sp.pos.x += g->bomb_tex[ACTIVE].width * sp.scale + 50;
	g->font.scale = 1.8;
	sp.pos.y -= g->font.char_h * g->font.scale / 3;
	str = ft_itoa(g->bomb_count);
	render_text(g, str, sp.pos);
	free(str);
	sp.pos.x = GAME_WIDTH + 150;
	sp.pos.y += g->bomb_tex[ACTIVE].height * sp.scale + 50;
	put_timer(g, sp.pos);
	sp.pos.y += g->font.char_h * g->font.scale + 10;
	sp.pos.x = GAME_WIDTH + 100 + g->bomb_tex[ACTIVE].width * sp.scale + 50;
	put_score_text(g, (t_pos){sp.pos.x, sp.pos.y});
}
