/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_damage.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: <tu_usuario>                                +#+  +:+       +#+        */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cub3d.h"

void	player_take_damage(t_game *g, int amount)
{
	/* Seguridad: garantizar max_hp razonable */
	if (g->player_max_hp <= 0)
		g->player_max_hp = 100;

	/* Evita daño repetido demasiado rápido (30 ticks ~= 1s) */
	if (g->timer - g->player_last_hit_time < 30)
		return ;

	g->player_last_hit_time = g->timer;
	g->player_hp -= amount;
	if (g->player_hp < 0)
		g->player_hp = 0;

	/* Opcional: log para debug (útil para reproducir) */
	ft_printf("player_take_damage: -%d hp, now %d/%d (timer=%d)\n",
		amount, g->player_hp, g->player_max_hp, g->timer);
}
