/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:30:01 by kroyo-di          #+#    #+#             */
/*   Updated: 2025/09/12 18:09:20 by kroyo-di         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void    error_handler(int code)
{
    if (code == 0)
        printf("Usage: ./cub3d <map_file>\n");
    else if (code == 1)
        printf("Wrong file type. Expected file: *.cub\n");
    else if (code == 2)
        printf("Could not open specified file\n");
    else
        printf("Unknown error code\n");
    exit(0);
}