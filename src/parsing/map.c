/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 19:20:58 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/10 19:21:01 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

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
				|| s[i] == 'E' || s[i] == 'W' || s[i] == ' '))
			return (0);
		i++;
	}
	return (1);
}

static int	flood_fill(char **map, t_pos pos, int rows, int **visited)
{
	int	row_length;

	if (pos.y < 0 || pos.y >= rows)
		return (0);
	row_length = ft_strlen(map[pos.y]);
	if (pos.x < 0 || pos.x >= row_length)
		return (0);
	if (map[pos.y][pos.x] == '1' || visited[pos.y][pos.x])
		return (1);
	visited[pos.y][pos.x] = 1;
	if ((pos.y == 0 || pos.y == rows - 1) || (pos.x == 0 || pos.x == row_length
			- 1))
		return (0);
	if (!flood_fill(map, (t_pos){pos.x, pos.y + 1}, rows, visited))
		return (0);
	if (!flood_fill(map, (t_pos){pos.x, pos.y - 1}, rows, visited))
		return (0);
	if (!flood_fill(map, (t_pos){pos.x + 1, pos.y}, rows, visited))
		return (0);
	if (!flood_fill(map, (t_pos){pos.x - 1, pos.y}, rows, visited))
		return (0);
	return (1);
}

static int	**get_visited_map(char **map)
{
	int	row;
	int	**visited;

	visited = NULL;
	row = -1;
	while (map[++row])
		ft_append_array((void ***)&visited,
			ft_safe_calloc((ft_strlen(map[row])), sizeof(int)));
	if (!visited)
		ft_error_exit("Error: Memory allocation failed for visited map\n");
	return (visited);
}

static int	is_map_closed(char **map)
{
	int		rows;
	int		**visited;
	t_pos	p;
	int		result;

	rows = ft_memlen((const void **)map);
	result = 1;
	visited = get_visited_map(map);
	p.y = -1;
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (map[p.y][p.x] != '1' && map[p.y][p.x] != ' ')
			{
				if (!flood_fill(map, p, rows, visited))
					result = 0;
				ft_free_array((void ***)&visited);
				return (result);
			}
		}
	}
	ft_free_array((void ***)&visited);
	return (result);
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
	while (content[i])
	{
		if (!validate_line(content[i]))
			break ;
		line = ft_strdup(content[i]);
		if (!line)
			ft_error_exit("Error: Memory allocation failed for map line\n");
		ft_append_array((void ***)&map, line);
		i++;
	}
	if (!map || !is_map_closed(map))
		ft_error_exit("Error: Map is not closed/surrounded by walls\n");
	return (map);
}
