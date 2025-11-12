/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   audio.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 16:21:02 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/10 16:21:05 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIO_H
# define MINIAUDIO_IMPLEMENTATION
# define AUDIO_H
# include "../libs/miniaudio/miniaudio.h"

typedef struct s_audio
{
	ma_engine	engine;
	ma_sound	bg_music;
	ma_sound	lizard;
	ma_sound	spiderweb;
	ma_sound	game_over;
}				t_audio;

#endif