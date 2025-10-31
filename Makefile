# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kroyo-di <kroyo-di@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/10/26 18:38:48 by kroyo-di         ###   ########.fr        #
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
		  src/render/render.c \
		  src/render/intro.c \
		  src/render/display_scores.c \
		  src/render/high_scores.c \
		  src/render/minimap_render/minimap.c \
		  src/render/minimap_render/draw_minimap.c \
		  src/render/minimap_render/minimap_sprites.c \
		  src/render/minimap_render/fog.c \
		  src/render/minimap_render/player.c \
		  src/moves/spider.c \
		  src/moves/spider_attack.c \
		  src/moves/lizards.c \
		  src/moves/key_hooks.c \
		  src/moves/mark_score.c \
		  src/close_program.c \
		src/main.c 
OBJ     = $(SRCS:.c=.o)

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_REPO = https://github.com/alejhern/libft.git

MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_DIR = minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

NAME    = cub3d
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinc -I$(LIBFT_DIR) -g


# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(LIBFT) $(MLX_LIB) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(MLX) $(LIBFT)

%.o: %.c ./inc/cub3d.h Makefile
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@if [ ! -d "$(LIBFT_DIR)" ]; then \
		git clone $(LIBFT_REPO) $(LIBFT_DIR); \
	fi
	@make -C $(LIBFT_DIR)

$(MLX_LIB):
	@if [ ! -d "$(MLX_DIR)" ]; then \
		echo "📥 Clonando MiniLibX..."; \
		git clone $(MLX_REPO) $(MLX_DIR); \
	fi
	$(MAKE) -C $(MLX_DIR)
	@echo "✅ MiniLibX compilada."

clean:
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -rf $(MLX_DIR)
	rm -f $(NAME)

re: fclean all
