/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 16:47:00 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/29 20:39:26 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

#define O_FAILED "Error: Could not open file %s\n"
#define R_FAILED "Error: Could not read %s content\n"
#define NO_BOMBS "Error: No bombs found in the map %s\n"
#define NO_TEXTURES "Error: Not all textures/colors were defined in %s\n"
#define INVALID_LINE "Error: Invalid line in texture definitions in %s\n"
#define MAX_TIMEOUT 3500
#define MIN_TIMEOUT 500

static unsigned int	get_timeout_value(t_game g)
{
	double			dist;
	int				i;
	unsigned int	timeout;

	dist = 0.0;
	if (!g.bomb_count)
		return (0);
	i = -1;
	while (g.bombs[++i])
		dist += sqrt(pow(g.spider.pos.x - g.bombs[i]->pos.x, 2)
				+ pow(g.spider.pos.y - g.bombs[i]->pos.y, 2));
	dist = dist / g.bomb_count;
	dist /= sqrt(pow(get_map_max_width(g.map), 2) + pow(ft_memlen(g.map), 2));
	timeout = MIN_TIMEOUT + (unsigned int)(dist * (MAX_TIMEOUT - MIN_TIMEOUT));
	return (timeout);
}

static char	**read_file(int fd)
{
	char	**content;
	char	*line;

	content = NULL;
	line = get_next_line(fd);
	if (!line)
		return (NULL);
	while (line)
	{
		ft_clean_line(&line);
		ft_append_array((void ***)&content, line);
		line = get_next_line(fd);
	}
	return (content);
}

int	check_files_extension(int argc, char **argv)
{
	int	i;
	int	len;

	if (argc < 2)
		ft_error_exit("Error: No input files provided\n");
	i = 0;
	while (argv[++i])
	{
		len = ft_strlen(argv[i]);
		if (len < 4 || ft_strncmp(argv[i] + len - 4, ".cub", 4) != 0)
		{
			ft_printf_fd(2, "Error: Invalid file extension for %s, ", argv[i]);
			ft_putendl_fd("Expected .cub", 2);
			return (1);
		}
	}
	return (0);
}

static char	**get_content(t_game *g)
{
	int		fd;
	char	**content;
	int		all;

	fd = open(*g->levels, O_RDONLY);
	if (fd == -1)
		return (set_error_parsing(g, O_FAILED, *g->levels), NULL);
	content = read_file(fd);
	if (!content)
		set_error_parsing(g, R_FAILED, *g->levels);
	close(fd);
	if (g->level == 0)
	{
		all = all_textures_defined(content);
		if (all != 1)
			free(content);
		if (all == -1)
			return (set_error_parsing(g, INVALID_LINE, *g->levels), NULL);
		if (all == 0)
			return (set_error_parsing(g, NO_TEXTURES, *g->levels), NULL);
	}
	return (content);
}

void	get_info_file(t_game *g, int only_map)
{
	char	**content;

	if (!ft_memlen(g->levels))
		return (g->render_state = WIN, (void)0);
	if (g->level > 1)
		free_level(g);
	content = get_content(g);
	if (!content)
		return ;
	if (!g->map)
		g->map = get_map(content);
	if (!g->map)
		return (ft_free_array((void ***)&content), set_error_parsing(g, NULL,
				NULL));
	if (only_map)
		return (ft_free_array((void ***)&content), (void)0);
	load_map_textures(g, content);
	ft_free_array((void ***)&content);
	create_spiderman(g);
	create_sprites(g);
	if (!g->bomb_count)
		set_error_parsing(g, NO_BOMBS, *g->levels);
	if (g->level > 1 && g->render_state == LOAD_LEVEL)
		g->render_state = NEW_LEVEL;
	g->timeout = get_timeout_value(*g);
}
