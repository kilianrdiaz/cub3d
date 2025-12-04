/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 21:09:41 by alejhern          #+#    #+#             */
/*   Updated: 2025/12/01 21:32:20 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define NO_PLAYER "Error: No player starting position found in the map"
#define MULTI_PLAYER                             \
	"Error: Multiple player starting positions found in the map"
#define NO_WAY_TO_BOMB "Error: No way to reach all bombs in the map"
#define NO_CLOSE "Error: Map is not closed/surrounded by walls"

static t_pos	get_player_pos(char **map)
{
	t_pos	player;
	t_pos	pos;
	int		found;

	ft_memset(&player, -1, sizeof(t_pos));
	pos.y = -1;
	found = 0;
	while (map[++pos.y])
	{
		pos.x = -1;
		while (map[pos.y][++pos.x])
		{
			if (map[pos.y][pos.x] == 'N' || map[pos.y][pos.x] == 'S'
				|| map[pos.y][pos.x] == 'E' || map[pos.y][pos.x] == 'W'
				|| map[pos.y][pos.x] == 'P')
			{
				player = pos;
				found++;
				if (found > 1)
					return ((t_pos){.x = -2, .y = -2});
			}
		}
	}
	return (player);
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

static int	is_map_closed(char **map)
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
			if (map[p.y][p.x] != '1' && map[p.y][p.x] != ' '
				&& !visited[p.y][p.x])
			{
				if (!flood_fill(map, p, rows, visited))
					return (ft_free_array((void ***)&visited), 0);
			}
		}
	}
	return (ft_free_array((void ***)&visited), 1);
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

char	*check_map_accessibility(char **map)
{
	int		**visited;
	t_pos	player;
	t_pos	p;

	if (!is_map_closed(map))
		return (NO_CLOSE);
	player = get_player_pos(map);
	if (player.x == -1 && player.y == -1)
		return (NO_PLAYER);
	else if (player.x == -2 && player.y == -2)
		return (MULTI_PLAYER);
	visited = get_visited_map(map);
	accessible_flood(map, player, ft_memlen((const void **)map), visited);
	p.y = -1;
	while (map[++p.y])
	{
		p.x = -1;
		while (map[p.y][++p.x])
		{
			if (map[p.y][p.x] == 'B' && !visited[p.y][p.x])
				return (ft_free_array((void ***)&visited), NO_WAY_TO_BOMB);
		}
	}
	ft_free_array((void ***)&visited);
	return (NULL);
}
