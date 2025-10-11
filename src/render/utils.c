/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:50 by alejhern          #+#    #+#             */
/*   Updated: 2025/09/16 13:56:52 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	clamp_int(int v, int a, int b)
{
	if (v < a)
		return (a);
	if (v > b)
		return (b);
	return (v);
}

void	clean_screen(t_game *g)
{
	t_pos	it;

	it.y = -1;
	while (++it.y < HEIGHT)
	{
		it.x = -1;
		while (++it.x < WIDTH)
			put_pixel(g, it.x, it.y, 0x000000);
	}
}

void	put_pixel(t_game *g, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	dst = g->addr + (y * g->line_len + x * (g->bpp / 8));
	*(unsigned int *)dst = (unsigned int)color;
}

t_sprite	**get_sprites(t_game *g)
{
	t_sprite	**sprites;
	int			count;
	int			x;

	count = g->bomb_count;
	if (g->lizards)
		count += ft_memlen((void **)g->lizards);
	if (count == 0)
		return (NULL);
	sprites = malloc(sizeof(t_sprite *) * (count + 1));
	if (!sprites)
		ft_error_exit("Error: Memory allocation failed for sprites\n");
	sprites[count] = NULL;
	x = -1;
	count = 0;
	while (g->bombs && g->bombs[++x])
	{
		if (g->bombs[x]->state == DEFUSED)
			continue ;
		sprites[count++] = g->bombs[x];
	}
	x = -1;
	while (g->lizards && g->lizards[++x])
		sprites[count++] = g->lizards[x];
	return (sprites);
}
