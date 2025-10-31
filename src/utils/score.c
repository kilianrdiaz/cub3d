/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   score.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:19:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/19 20:49:23 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

char	**get_scores(void)
{
	int		fd;
	char	*line;
	char	**scores;
	int		index;

	fd = open("scores.txt", O_RDONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		ft_putendl_fd("Error: Could not open scores.txt", 2);
		return (NULL);
	}
	scores = ft_calloc(sizeof(char *), 6);
	if (!scores)
		return (close(fd), NULL);
	line = get_next_line(fd);
	index = 0;
	while (line && index < 5)
	{
		ft_clean_line(&line);
		scores[index++] = line;
		line = get_next_line(fd);
	}
	close(fd);
	return (scores);
}

int	get_position(t_game *g, char **scores)
{
	char	**line;
	int		position;

	if (!scores)
		return (0);
	position = -1;
	while (scores[++position])
	{
		line = ft_split(scores[position], ' ');
		if (!line)
			return (position);
		if (!line[1])
			return (ft_free_array((void ***)&line), position);
		if (g->score > ft_atoi(line[1]))
			return (ft_free_array((void ***)&line), position);
		ft_free_array((void ***)&line);
	}
	if (position < 5)
		return (position);
	return (-1);
}

static void	add_buttons(t_game *g, t_sprite *alphabet, int index, t_pos pos)
{
	int	line_buttons;

	line_buttons = pos.y;
	pos.y += g->font.scale * g->font.char_h - 40;
	alphabet[++index].pos = (t_coords){pos.x, line_buttons};
	g->font.scale = 0.75;
	render_text(g, "DEL", pos);
	pos.x += g->font.char_w * 4 * g->font.scale + 10;
	alphabet[++index].pos = (t_coords){pos.x, line_buttons};
	render_text(g, "END", pos);
	g->font.scale = 3.5;
}

t_sprite	*print_alphabet(t_game *game, t_tex score_panel)
{
	char		x;
	int			index;
	t_sprite	*alphabet;
	t_pos		pos;

	game->font.scale = 3.5;
	alphabet = ft_safe_calloc(sizeof(t_sprite), 28);
	pos.x = (WIDTH - score_panel.width) / 2 - 60;
	pos.y = (HEIGHT - score_panel.height) / 2 + 120;
	index = -1;
	x = 'A' - 1;
	while (++x <= 'Z' && ++index < 26)
	{
		alphabet[index].pos = (t_coords){pos.x, pos.y};
		render_text(game, &x, pos);
		pos.x += game->font.char_w * game->font.scale + 10;
		if ((x - 'A' + 1) % 9 == 0)
		{
			pos.x = (WIDTH - score_panel.width) / 2 - 60;
			pos.y += game->font.char_h * game->font.scale + 10;
		}
	}
	add_buttons(game, alphabet, index, pos);
	return (alphabet);
}

void	update_scores(char **scores, int position)
{
	int	last;
	int	j;

	if (!scores || position < 0 || scores[position] == NULL)
		return ;
	last = 0;
	while (scores[last])
		last++;
	if (scores[last] != NULL)
		return ;
	j = last - 1;
	while (j >= position)
	{
		scores[j + 1] = scores[j];
		j--;
	}
	scores[position] = NULL;
}
