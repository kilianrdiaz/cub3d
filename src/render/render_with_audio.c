/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_with_audio.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 13:56:22 by alejhern          #+#    #+#             */
/*   Updated: 2025/11/10 15:41:27 by alejhern         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/audio.h"
#include "../../inc/cub3d.h"

static void	clean_audio(t_audio *audio)
{
	if (!audio)
		return ;
	ma_sound_stop(&audio->game);
	ma_sound_uninit(&audio->game);
	ma_sound_uninit(&audio->bomb);
	ma_sound_uninit(&audio->lizard);
	ma_sound_uninit(&audio->spiderweb);
	ma_sound_uninit(&audio->game_over);
	ma_sound_uninit(&audio->win);
	ma_engine_uninit(&audio->engine);
	free(audio);
}

static void	load_audio(ma_engine *engine, ma_sound *sound,
		const char *file_path, ma_uint32 flags)
{
	if (ma_sound_init_from_file(engine, file_path, flags, NULL, NULL,
			sound) != MA_SUCCESS)
		ft_printf_fd(STDERR_FILENO, "Error loading audio file: %s\n",
			file_path);
}

static void	audio_init(void **audio_ptr)
{
	t_audio	*audio;

	if (*audio_ptr) // Ya inicializado
		return ;
	audio = ft_calloc(1, sizeof(t_audio));
	if (!audio)
		return ;
	if (ma_engine_init(NULL, &audio->engine) != MA_SUCCESS)
	{
		ft_putendl_fd("Error inicializando el motor de audio", 2);
		return (clean_audio(audio));
	}
	load_audio(&audio->engine, &audio->game,
		"audios/escape-from-the-sewers.wav", MA_SOUND_FLAG_LOOPING);
	load_audio(&audio->engine, &audio->bomb, "audios/bomb.wav", 0);
	load_audio(&audio->engine, &audio->lizard, "audios/lizard.wav", 0);
	load_audio(&audio->engine, &audio->spiderweb, "audios/web.wav", 0);
	load_audio(&audio->engine, &audio->game_over, "audios/gameover.wav", 0);
	load_audio(&audio->engine, &audio->win, "audios/win.wav", 0);
	ma_sound_start(&audio->game);
	*audio_ptr = audio; // Guardamos el puntero en g->audio
}

static void	game_effects(t_game *g, t_audio *audio)
{
	t_sprite	**sprites;
	int			i;

	if (!audio || g->render_state != PLAYING)
		return ;
	if (g->spider.state == ATTACKING)
		ma_sound_start(&audio->spiderweb);
	sprites = get_sprites(*g);
	i = -1;
	while (sprites && sprites[++i])
	{
		if ((sprites[i]->type == LIZARD && sprites[i]->state == ATTACKING)
			|| (sprites[i]->type == BOMB && sprites[i]->state == ATTACKED))
		{
			ma_sound_stop(&audio->bomb);
			ma_sound_stop(&audio->lizard);
		}
		if (sprites[i]->type == LIZARD && sprites[i]->state == ATTACKING)
			ma_sound_start(&audio->lizard);
		if (sprites[i]->type == BOMB && sprites[i]->state == ATTACKED)
			ma_sound_start(&audio->bomb);
	}
	free(sprites);
}

int	render(t_game *g)
{
	t_audio	*audio;

	audio_init(&g->audio);
	audio = (t_audio *)g->audio;
	if (g->render_state == GAME_OVER && g->timer < TEXT_DURATION / 2)
		ma_sound_start(&audio->game_over);
	if (g->render_state == WIN && g->timer < TEXT_DURATION / 2)
		ma_sound_start(&audio->win);
	game_effects(g, audio);
	if (g->render_state == END)
		clean_audio(audio);
	g->game_loop(g);
	return (0);
}
