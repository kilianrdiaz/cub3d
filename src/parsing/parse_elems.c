/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elems.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 21:22:21 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/17 21:34:11 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

int	parse_rgb(char *r, char *g, char *b);
void	parse_path(char *data);

void	save_element(t_elem *elems, char **splited)
{
	t_elem	*new_elem;

	new_elem = malloc(sizeof(t_elem));
	if (!new_elem)
		error_handler(3);
	if (!elems)
		elems = new_elem;
	else
	{
		new_elem->id = ft_strdup(splited[0]);
		// Guardar path o color en new_elem TODO
		//ft_append_array((void ***)&elems, new_elem);
	}
}

int	check_path(char *data)
{
	// OPEN() PARA PROBAR SI EXISTE
	/*if (!open())
	{
		return (-1)
	}*/
	// Si no existe, es rgb
	//close()
	//load_texture(data);
	data = NULL; // evitar warning
	(void)data;
	return (1);
	
}

int	check_id(t_elem *elems, char *id)
{
	int	i;

	i = 0;
	if (!id)
		return (-1);
	if (!elems)
		return (1);
	while (elems[i].id)
	{
		if (!ft_strncmp(elems[i].id, id, ft_strlen(id)))
			return (-1);
		i++;
	}
	return (0);
}

void	parse_element(t_game *game, char *line)
{
	int		i;
	char	**splited;

	i = 0;

	splited = ft_split(line, ' ');
	if (!splited[0] || !splited[1])
		error_handler(3);
	if (check_id(game->elems, splited[0]) == -1)
		error_handler(3);
	if (!splited[2])
		check_path(splited[1]);
	else if (splited[2] && splited[3] && !splited[4])
	{
		if (parse_rgb(splited[1], splited[2], splited[3]) == 0)
			error_handler(3);
	}
	else
		error_handler(3);
	// Guardar en struct
	save_element(game->elems, splited);
	while (splited[i])
		free(splited[i++]);
	free(splited);	
}

int	parse_rgb(char *r, char *g, char *b)
{
	int		i;
	int		j;
	char	**rgb;
	
	rgb = NULL;
	i = 0;
	j = 0;
	
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(r, ",")));
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(g, ",")));
	ft_append_array((void ***)&rgb, ft_strdup(ft_strtrim(b, ",")));

	while (rgb[i])
	{
		j = 0;
		while (rgb[i][j])
		{
			if (!ft_isnum(&rgb[i][j]) || j > 2)
				return (0);
			j++;
		}
		i++;
	}
	if (i != 4)
		error_handler(3);
	i = 0;
	while (i < 3)
	{
		if (ft_atoi(rgb[i++]) > 255)
			return (0);
	}
	return (1);
}


/*
*/