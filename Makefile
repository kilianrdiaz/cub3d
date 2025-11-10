# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/11/04 20:37:36 by kroyo-di         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

SRCS    = src/utils/parsing.c  \
		  src/utils/render.c    \
		  src/utils/ray_map.c \
		  src/utils/score.c   \
		  src/parsing/map_textures.c   \
		  src/parsing/map.c   \
		  src/parsing/sprites.c   \
		  src/parsing/file.c   \
		  src/render/player.c \
		  src/render/map.c \
		  src/render/sprite.c \
		  src/render/font.c \
		  src/render/intro.c \
		  src/render/load_level.c \
		  src/render/timer.c \
		  src/render/stats.c \
		  src/render/display_scores.c \
		  src/render/high_scores.c \
		  src/render/minimap/minimap.c \
		  src/render/minimap/draw_minimap.c \
		  src/render/minimap/minimap_sprites.c \
		  src/render/minimap/player.c \
		  src/moves/spider.c \
		  src/moves/spider_attack.c \
		  src/moves/lizards.c \
		  src/moves/key_hooks.c \
		  src/moves/mark_score.c \
		  src/close_program.c \
		  src/main.c

RENDER_DEFAULT       = src/render/render.c
RENDER_WITH_AUDIO    = src/render/render_with_audio.c

OBJ_DIR = obj

# ================================
# 🧱 LIBFT
# ================================
LIBFT_DIR = libs/libft/
LIBFT_LIB = $(LIBFT_DIR)libft.a
LIBFT_REPO = https://github.com/alejhern/libft.git
LIBFT   = -L$(LIBFT_DIR) -lft

# ================================
# 🖼️ MINILIBX
# ================================
MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_DIR = libs/minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

# ================================
# 🎵 MINIAUDIO (header-only)
# ================================
MINIAUDIO_DIR = libs/miniaudio
MINIAUDIO_REPO = https://github.com/mackron/miniaudio.git
MINIAUDIO_LIB = $(MINIAUDIO_DIR)/miniaudio.h
MINIAUDIO_INC = -I$(MINIAUDIO_DIR)

# ================================
# ⚙️ COMPILACIÓN
# ================================
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g -Iinc
NAME    = cub3d
VICTUS  = -DFRAMES_PER_SECOND=30

# ================================
#   OBJETOS DINÁMICOS
# ================================
OBJ_RENDER = $(OBJ_DIR)/render/render.o        # por defecto
OBJ        = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRCS)) $(OBJ_RENDER)

# **************************************************************************** #
#                                 REGLAS                                        #
# **************************************************************************** #

all: $(LIBFT_LIB) $(MLX_LIB) $(NAME)

# -----------------------------
# Compilación con audio
# -----------------------------
miniaudio: $(MINIAUDIO_LIB)
	@echo "🔊 Compilando con audio..."
	$(MAKE) OBJ_RENDER=$(OBJ_DIR)/render/render_with_audio.o CFLAGS+="$(CFLAGS)" all

# -----------------------------
# Victus
# -----------------------------
victus: CFLAGS += $(VICTUS)
victus: miniaudio

# -----------------------------
# Compilación final
# -----------------------------
$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(MLX) $(LIBFT)

# -----------------------------
# Compilación de cada objeto
# -----------------------------
$(OBJ_DIR)/%.o: src/%.c ./inc/cub3d.h Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# render.o normal
$(OBJ_DIR)/render/render.o: $(RENDER_DEFAULT) ./inc/cub3d.h Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# render_with_audio.o
$(OBJ_DIR)/render/render_with_audio.o: $(RENDER_WITH_AUDIO) ./inc/cub3d.h ./inc/audio.h Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(MINIAUDIO_INC) -c $< -o $@

# ===========================
#   🧰 LIBRERÍAS EXTERNAS
# ===========================

$(LIBFT_LIB):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
	    echo "📥 Clonando Libft..."; \
	    git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
	    echo "📥 Clonando MiniLibX..."; \
	    git clone $(MLX_REPO) $(MLX_DIR); \
	fi
	@make -C $(MLX_DIR)

$(MINIAUDIO_LIB):
	@if [ ! -f "$(MINIAUDIO_LIB)" ]; then \
	    echo "📥 Clonando Miniaudio..."; \
	    git clone $(MINIAUDIO_REPO) $(MINIAUDIO_DIR); \
	fi

# ===========================
#   🧹 LIMPIEZA
# ===========================

clean:
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean
	rm -rf $(OBJ_DIR)
	@echo "🧹 Archivos objeto eliminados."

fclean: clean
	rm -rf libs
	rm -f $(NAME)
	@echo "🧼 Limpieza completa."

re: fclean all

.PHONY: all clean fclean re victus miniaudio
