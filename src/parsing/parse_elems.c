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

void	parse_rgb(char *data);
void	parse_path(char *data);

void	parse_element(char *line, char *id)
{
	int		i;
	char	*e_data;

	i = 0;
	e_data = NULL;
	while (ft_isspace(*line))
		line++;
	if (i == 0)
		error_handler(3);
	line = &line[i];
	i = 0;
	while (!ft_isspace(line[i]))
		i++;
	ft_strlcpy(e_data, line, i + 1);
	i = 0;
	if (id[0] == 'F' || id[0] == 'C')
		parse_rgb(e_data);
	else
		parse_path(e_data); // TODO
}

void	parse_rgb(char *data)
{
	int		i;
	int		j;
	char	**rgb;
	
	rgb = NULL;
	i = 0;
	j = 0;
	
	// Comprobar si es rgb o textura (se aceptan las 2)
	if (!ft_strncmp(data, "./", 2))  // Las texturas deben empezar con ./ ???
		parse_path(data); // TODO
	else
	{
		rgb = ft_split(data, ',');
		while (rgb[i])
		{
			j = 0;
			while (rgb[i][j])
			{
				if (!ft_isnum(&rgb[i][j]) || j > 2)
					error_handler(3);
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
				error_handler(3);
		}
	}
}

void	parse_path(char *data)
{
    // TODO
    
	data = NULL;
}