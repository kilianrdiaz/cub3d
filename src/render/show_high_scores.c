/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_high_scores.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 18:19:03 by alejhern          #+#    #+#             */
/*   Updated: 2025/10/18 18:19:08 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"
static char	**get_scores(void)
{
	int		fd;
	char	*line;
	char	**scores;

	fd = open("scores.txt", O_RDONLY);
	if (fd == -1)
	{
		fd = open("scores.txt", O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
			return (NULL);
		close(fd);
		fd = open("scores.txt", O_RDONLY);
		if (fd == -1)
			return (NULL);
	}

	scores = NULL;
	line = get_next_line(fd);
	while (line)
	{
		ft_clean_line(&line);
		ft_append_array((void ***)&scores, ft_strdup(line));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);

	if (!scores)
		scores = ft_safe_calloc(1, sizeof(char *));
	return (scores);
}

static int	get_position(t_game *g, char **scores)
{
	char	**line;
	int		position;

	if (!scores)
		return (0);
	position = -1;
	while (scores[++position])
	{
		line = ft_split(scores[position], ' ');
		if (!line || !line[1])
			break ;
		if (g->score >= ft_atoi(line[1]))
		{
			ft_free_array((void ***)&line);
			return (position);
		}
		ft_free_array((void ***)&line);
	}
	if (position < 5)
		return (position);
	return (-1);
}

static void	save_scores(char **scores)
{
	int	fd;
	int	i;

	fd = open("scores.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return ;
	i = -1;
	while (scores && scores[++i])
		ft_putendl_fd(scores[i], fd);
	close(fd);
}

static void update_scores(char **scores, int position)
{
    int last;
    int j;

    if (!scores || position < 0 || scores[position] == NULL)
        return;

    // Contar elementos hasta NULL
    last = 0;
    while (scores[last])
        last++;

    // Comprobar que hay espacio para desplazar
    if (scores[last] != NULL)
        return; // no hay hueco, no hacemos nada

    // Desplazar los elementos hacia abajo desde el final hasta 'position'
    j = last - 1;
    while (j >= position)
    {
        scores[j + 1] = scores[j];
        j--;
    }

    // Dejar la posición libre
    scores[position] = NULL;
}

static void	display_score_panel(t_game *g, t_tex score_panel, char **scores)
{
	int	index;
	t_pos	pos;

	g->font.scale = 1.0;
	draw_fullscreen_image(g, &score_panel);
	render_text(g, "HIGH SCORES", (t_pos){
		(WIDTH - score_panel.width) / 2,
		(HEIGHT - score_panel.height) / 2 - 100
	});
	if (!scores)
		return ;
	index = -1;
	while (scores[++index])
	{
		pos.x = (WIDTH - score_panel.width) / 2;
		pos.y = (HEIGHT - score_panel.height) / 2 + 50 + index * 50;
		render_text(g, scores[index], pos);
	}
}

int	show_high_scores(t_game *g)
{
	char	**scores;
	int		position;
	t_tex	score_panel;
	char	*new_score;

	load_texture(g, &score_panel, "./textures/score_panel.xpm");
	scores = get_scores();
	position = get_position(g, scores);
	if (position != -1 && g->render_state != SCORE_SAVED)
		g->render_state = WAITING_FOR_NAME;
	if (g->render_state == WAITING_FOR_NAME)
	{
		new_score = register_score(g, &score_panel);
		if (new_score)
		{
			update_scores(scores, position);
			scores[position] = new_score;
			save_scores(scores);
			g->render_state = SCORE_SAVED;
		}
		ft_free_array((void ***)&scores);
		return (0);
	}
	display_score_panel(g, score_panel, scores);
	ft_free_array((void ***)&scores);
	mlx_put_image_to_window(g->mlx, g->win, g->img, 0, 0);
	mlx_destroy_image(g->mlx, score_panel.img);
	return (0);
}
