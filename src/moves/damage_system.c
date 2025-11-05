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
	// Evita recibir daño más de una vez por segundo
	if (g->timer - g->spider.last_hit_time < 30)
		return;

	g->spider.last_hit_time = g->timer;
	g->spider.hp -= amount;
	if (g->spider.hp < 0)
		g->spider.hp = 0;

	// 🔊 (opcional) aquí puedes añadir sonidos o parpadeos más adelante
}
