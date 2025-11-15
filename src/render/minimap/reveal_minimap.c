/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reveal_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 17:04:05 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/11/15 17:42:48 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/cub3d.h"

void	free_revealed(t_minimap *m)
{
	int	i;

	if (!m->revealed)
		return ;
	i = 0;
	while (i < m->height)
	{
		free(m->revealed[i]);
		i++;
	}
	free(m->revealed);
	m->revealed = NULL;
	m->width = 0;
	m->height = 0;
}

static int	alloc_revealed_rows(t_minimap *m, int w, int h)
{
	int	i;

	i = 0;
	while (i < h)
	{
		m->revealed[i] = ft_calloc(w, sizeof(char));
		if (!m->revealed[i])
			return (i);
		i++;
	}
	m->revealed[h] = NULL;
	return (-1);
}

void	init_revealed_if_needed(t_minimap *m, int w, int h)
{
	int	fail_i;
	int	j;

	if (m->revealed && m->width == w && m->height == h)
		return ;
	if (m->revealed)
		free_revealed(m);
	m->revealed = ft_calloc(h + 1, sizeof(char *));
	if (!m->revealed)
		return ;
	fail_i = alloc_revealed_rows(m, w, h);
	if (fail_i != -1)
	{
		j = 0;
		while (j < fail_i)
		{
			free(m->revealed[j]);
			j++;
		}
		free(m->revealed);
		m->revealed = NULL;
		return ;
	}
	m->width = w;
	m->height = h;
}

int	get_map_max_width(char **map)
{
	int	max;
	int	len;
	int	i;

	if (!map)
		return (0);
	max = 0;
	i = -1;
	while (map[++i])
	{
		len = ft_strlen(map[i]);
		if (len > max)
			max = len;
	}
	return (max);
}
