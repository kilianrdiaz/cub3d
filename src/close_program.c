/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_program.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:29:32 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/16 19:29:37 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cub3d.h"

static void	clean_texture(t_game *g, t_tex *tex, int count)
{
	int	i;

	if (!tex)
		return ;
	i = -1;
	while (++i < count)
	{
		if (tex[i].img)
			mlx_destroy_image(g->mlx, tex[i].img);
	}
	free(tex);
}

static void	clean_font(t_game *g, t_font *font)
{
	int	i;

	if (!font)
		return ;
	i = -1;
	while (++i < 126)
	{
		if (font->chars[i].pixels)
			free(font->chars[i].pixels);
	}
	if (font->img)
		mlx_destroy_image(g->mlx, font->img);
}

void	close_program(t_game *g)
{
	clean_texture(g, g->bomb_tex, sizeof(t_state));
	clean_texture(g, g->lizard_tex, sizeof(t_state));
	clean_texture(g, g->spider.hand, sizeof(t_state));
	clean_font(g, &g->font);
	clean_texture(g, g->map_text, 6);
	clean_screen(g);
	if (g->img)
		mlx_destroy_image(g->mlx, g->img);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	ft_free_array((void ***)&g->bombs);
	ft_free_array((void ***)&g->lizards);
	ft_free_array((void ***)&g->map);
	exit(0);
}
