/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_checking.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:00:00 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/10/07 11:06:04 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
#include "libft.h"

void	check_map_validity(t_game *game);

int is_map_str(char *s)
{
	int i;

	if (!s)
		return (0);
	i = 0;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (!(s[i] == '0' || s[i] == '1' || s[i] == 'N' ||
			  s[i] == 'S' || s[i] == 'E' || s[i] == 'W' ||
			  s[i] == ' ') )
			return (0);
		i++;
	}
	return (1);
}

int	is_empty_line(char *s)
{
	int i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

void	get_map(t_game *game, char **content, int start_index)
{
	int i;
	int map_start;
	int map_end;

	map_start = start_index;
	i = start_index;
	while (content[i] && is_map_str(content[i]))
		i++;
	map_end = i;

	game->map.height = map_end - map_start;
	game->map.map = malloc((game->map.height + 1) * sizeof(char *));
	if (!game->map.map)
		error_handler(3);
	i = 0;
	while (i < game->map.height)
	{
		game->map.map[i] = ft_strdup(content[map_start + i]);
		if (!game->map.map[i])
			error_handler(3);
		i++;
	}
	game->map.map[i] = NULL;
	game->map.width = 0;
	i = 0;
	while (game->map.map[i])
	{
		int len = ft_strlen(game->map.map[i]);
		if (len > game->map.width)
			game->map.width = len;
		i++;
	}
    i = 0;
    while (game->map.map[i])
    {
        int len = ft_strlen(game->map.map[i]);
        if (len < game->map.width)
        {
            char *tmp = malloc(game->map.width + 1);
            ft_memcpy(tmp, game->map.map[i], len);
            ft_memset(tmp + len, ' ', game->map.width - len);
            tmp[game->map.width] = '\0';
            free(game->map.map[i]);
            game->map.map[i] = tmp;
        }
        printf("%s\n", game->map.map[i]);
        i++;
    }
	check_map_validity(game);
}

/*void	check_map_validity(t_game *game)
{
	int i;
	int j;
	int player_count;

	player_count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			char curr = (j < (int)ft_strlen(game->map.map[i])) ? 
						game->map.map[i][j] : ' ';
			char up = (i > 0 && j < (int)ft_strlen(game->map.map[i - 1])) ? 
					  game->map.map[i - 1][j] : '1';
			char down = (i < game->map.height - 1 && j < (int)ft_strlen(game->map.map[i + 1])) ? 
						game->map.map[i + 1][j] : '1';
			char left = (j > 0) ? game->map.map[i][j - 1] : '1';
			char right = (j < (int)ft_strlen(game->map.map[i]) - 1) ? 
						 game->map.map[i][j + 1] : '1';

			if (curr == 'N' || curr == 'S' || curr == 'E' || curr == 'W')
				player_count++;
			if (curr != '1' && curr != ' ')
			{
				if (up == ' ' || down == ' ' || left == ' ' || right == ' ')
					error_handler(3);
			}
			j++;
		}
		i++;
	}
	if (player_count != 1)
		error_handler(3);
}*/
#include "../../inc/cub3d.h"
#include "libft.h"

static int	is_border_valid(t_game *game, int i)
{
	int	j;

	j = 0;
	while (j < (int)ft_strlen(game->map.map[i]))
	{
		if (i == 0 || i == game->map.height - 1)
		{
			if (game->map.map[i][j] != '1')
				return (0);
		}
		else if (j == 0 || j == (int)ft_strlen(game->map.map[i]) - 1)
		{
			if (game->map.map[i][j] != '1')
				return (0);
		}
		j++;
	}
	return (1);
}

static int	check_length_conditions(t_game *game, int i, int j)
{
	int	curr_len;
	int	top_len;
	int	bot_len;

	curr_len = ft_strlen(game->map.map[i]);
	top_len = (i > 0) ? ft_strlen(game->map.map[i - 1]) : curr_len;
	bot_len = (i < game->map.height - 1)
		? ft_strlen(game->map.map[i + 1]) : curr_len;

	if (curr_len > top_len && j >= top_len && game->map.map[i][j] != '1')
		return (0);
	if (curr_len > bot_len && j >= bot_len && game->map.map[i][j] != '1')
		return (0);
	return (1);
}

void	check_map_validity(t_game *game)
{
	int	i;
	int	j;
	int	player_count;

	player_count = 0;
	i = 0;
	while (i < game->map.height)
	{
		if (!is_border_valid(game, i))
			error_handler(3);
		j = 0;
		while (j < (int)ft_strlen(game->map.map[i]))
		{
			if (!check_length_conditions(game, i, j))
				error_handler(3);
			if (ft_strchr("NSEW", game->map.map[i][j]))
				player_count++;
			j++;
		}
		i++;
	}
	if (player_count != 1)
		error_handler(3);
}
