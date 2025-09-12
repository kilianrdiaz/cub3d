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

#include "cub3d.h"

void    error_handler(int code)
{
    if (code == 0)
        printf("* Usage: ./cub3d <map file>.cub *");
    if (code == 1)
        printf("* Could not open specified file *");
    else
        printf("* Unknown error code *");
    exit(0);
}