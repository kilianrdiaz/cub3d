/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fog.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/26 18:49:26 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/26 18:49:39 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	free_revealed(t_minimap *m)
{
	int	i;

	if (!m->revealed)
		return ;
	i = -1;
	while (++i < m->height)
		free(m->revealed[i]);
	free(m->revealed);
	m->revealed = NULL;
	m->width = 0;
	m->height = 0;
}

void	init_revealed_if_needed(t_minimap *m, int w, int h)
{
	int	i;

	if (m->revealed && w == m->width && h == m->height)
		return ;
	free_revealed(m);
	m->revealed = malloc(sizeof(char *) * h);
	if (!m->revealed)
		return ;
	i = -1;
	while (++i < h)
	{
		m->revealed[i] = malloc(w);
		if (!m->revealed[i])
			break ;
		ft_memset(m->revealed[i], 0, w);
	}
	m->width = w;
	m->height = h;
}

void	reveal_radius(t_minimap *m, int cx, int cy, int r)
{
	int	y;
	int	x;

	if (!m->revealed)
		return ;
	y = fmax(0, cy - r) - 1;
	while (++y <= fmin(m->height - 1, cy + r))
	{
		x = fmax(0, cx - r) - 1;
		while (++x <= fmin(m->width - 1, cx + r))
			if (sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy)) <= r + 0.4)
				m->revealed[y][x] = 1;
	}
}
