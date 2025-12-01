/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 21:09:41 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/30 21:09:51 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

static t_pos	get_player_pos(char **map)
{
	t_pos	p;
	int		y;
	int		x;

	p.x = -1;
	p.y = -1;
	y = -1;
	while (map[++y])
	{
		x = -1;
		while (map[y][++x])
		{
			if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E'
				|| map[y][x] == 'W' || map[y][x] == 'P')
			{
				p.x = x;
				p.y = y;
				return (p);
			}
		}
	}
	return (p);
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

static void	accessible_flood(char **map, t_pos p, int rows, int **visited)
{
	int	row_length;

	if (p.y < 0 || p.y >= rows)
		return ;
	row_length = ft_strlen(map[p.y]);
	if (p.x < 0 || p.x >= row_length)
		return ;
	if (map[p.y][p.x] == '1' || visited[p.y][p.x])
		return ;
	visited[p.y][p.x] = 1;
	accessible_flood(map, (t_pos){p.x + 1, p.y}, rows, visited);
	accessible_flood(map, (t_pos){p.x - 1, p.y}, rows, visited);
	accessible_flood(map, (t_pos){p.x, p.y + 1}, rows, visited);
	accessible_flood(map, (t_pos){p.x, p.y - 1}, rows, visited);
}

int	is_map_closed(char **map)
{
	int		rows;
	int		**visited;
	t_pos	p;

	rows = ft_memlen((const void **)map);
	p.y = -1;
	visited = get_visited_map(map);
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (map[p.y][p.x] != '1' && map[p.y][p.x] != ' ')
			{
				if (!flood_fill(map, p, rows, visited))
				{
					ft_free_array((void ***)&visited);
					return (0);
				}
			}
		}
	}
	ft_free_array((void ***)&visited);
	return (1);
}

int	check_bombs_accessibility(char **map)
{
	int		rows;
	int		**visited;
	t_pos	player;
	t_pos	p;

	rows = ft_memlen((const void **)map);
	visited = get_visited_map(map);
	player = get_player_pos(map);
	accessible_flood(map, player, rows, visited);
	p.y = -1;
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (map[p.y][p.x] == 'B' && !visited[p.y][p.x])
			{
				ft_free_array((void ***)&visited);
				return (0);
			}
		}
	}
	ft_free_array((void ***)&visited);
	return (1);
}
