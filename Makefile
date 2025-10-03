# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alejhern <alejhern@student.42barcelona.co  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/15 14:52:33 by alejhern          #+#    #+#              #
#    Updated: 2025/09/15 14:52:40 by alejhern         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                 VARIABLES                                    #
# **************************************************************************** #

SRCS    = src/render/player.c \
		src/render/utils.c \
		src/render/map.c \
		src/render/sprite.c \
		src/render/render.c \
		src/move_spider.c \
		src/spider_attack.c \
		src/key_hooks.c \
		src/main.c 

OBJ     = $(SRCS:.c=.o)

LIBFT_DIR = libft/
LIBFT = $(LIBFT_DIR)libft.a
LIBFT_REPO = https://github.com/alejhern/libft.git

MLX_DIR = minilibx-linux
MLX_REPO = https://github.com/42Paris/minilibx-linux.git
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX     = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm

NAME    = cub3d
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -Iinc -I$(LIBFT_DIR) -g
MLX     = -lmlx -lXext -lX11 -lm

# **************************************************************************** #
#                                 RULES                                        #
# **************************************************************************** #

all: $(LIBFT) $(MLX_LIB) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(MLX) $(LIBFT)

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
	rm -f $(OBJ)

fclean: clean
	@make -C $(LIBFT_DIR) fclean
	rm -rf $(MLX_DIR)
	rm -f $(NAME)

re: fclean all
