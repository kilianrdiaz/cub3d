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

#define NO_CLOSE "Error: Map is not closed/surrounded by walls"
#define NO_MAP "Error: Could not load map"
#define NO_WAY_TO_BOMB "Error: No way to reach all bombs in the map"

static int	is_map_str(char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (!(s[i] == '0' || s[i] == '1' || s[i] == 'N' || s[i] == 'S'
				|| s[i] == 'E' || s[i] == 'W' || s[i] == ' ' || s[i] == 'P'
				|| s[i] == 'B' || s[i] == 'L'))
			return (0);
		i++;
	}
	return (1);
}

static void	checker_map(char ***map)
{
	if (!map || !*map)
		return ;
	if (!check_map_accessibility(*map))
	{
		ft_putendl_fd(NO_WAY_TO_BOMB, STDERR_FILENO);
		ft_free_array((void ***)map);
	}
	else if (!is_map_closed(*map))
	{
		ft_putendl_fd(NO_CLOSE, STDERR_FILENO);
		ft_free_array((void ***)map);
	}
}

char	**get_map(char **content)
{
	char	**map;
	int		i;
	char	*line;

	map = NULL;
	i = 0;
	while (content[i] && !is_map_str(content[i]))
		i++;
	while (content[i] && is_map_str(content[i]))
	{
		if (!validate_line(content[i]))
			break ;
		line = ft_safe_strdup(content[i]);
		ft_append_array((void ***)&map, line);
		i++;
	}
	if (content[i] && !is_map_str(content[i]))
		return (ft_free_array((void ***)&map), NULL);
	checker_map(&map);
	return (map);
}
