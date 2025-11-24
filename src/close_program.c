/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_program.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 19:29:32 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/24 19:17:41 by kroyo-di         ###   ########.fr       */
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

void	free_level(t_game *g)
{
	ft_free_array((void ***)&g->bombs);
	ft_free_array((void ***)&g->lizards);
	ft_free_array((void ***)&g->map);
	ft_free_array((void ***)&g->minimap.revealed);
}

int	close_window(t_game *g)
{
	close_program(g);
	return (0);
}

void	close_program(t_game *g)
{
	clean_texture(g, g->bomb_tex, sizeof(t_state));
	clean_texture(g, g->lizard_tex, sizeof(t_state));
	clean_texture(g, g->spider.hand, sizeof(t_state));
	clean_texture(g, g->live.spidermask_tex, 2);
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
	free_level(g);
	exit(0);
}
