/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:20:58 by alejhern          #+#    #+#             */
/*   Updated: 2025/12/01 21:36:15 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define LINE_AFTER_MAP "Error: Found lines after map definition"

int	is_map_str(char *line)
{
	int	i;

	if (!line)
		return (0);
	i = 0;
	while (ft_isspace(line[i]))
		i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (!(line[i] == '0' || line[i] == '1' || line[i] == 'N'
				|| line[i] == 'S' || line[i] == 'E' || line[i] == 'W'
				|| line[i] == ' ' || line[i] == 'P' || line[i] == 'B'
				|| line[i] == 'L'))
			return (0);
		i++;
	}
	return (1);
}

static void	checker_map(char ***map)
{
	char	*msg;

	if (!map || !*map)
		return ;
	msg = check_map_accessibility(*map);
	if (!msg)
		return ;
	ft_putendl_fd(msg, STDERR_FILENO);
	ft_free_array((void ***)map);
}

char	**get_map(char **content)
{
	char	**map;
	int		i;
	char	*line;

	if (!content)
		return (NULL);
	map = NULL;
	i = 0;
	while (content[i] && !is_map_str(content[i]))
		i++;
	while (content[i] && is_map_str(content[i]))
	{
		line = ft_safe_strdup(content[i]);
		ft_append_array((void ***)&map, line);
		i++;
	}
	if (content[i])
	{
		ft_putendl_fd(LINE_AFTER_MAP, STDERR_FILENO);
		return (ft_free_array((void ***)&map), NULL);
	}
	checker_map(&map);
	return (map);
}
