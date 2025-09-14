NAME = cub3d

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRCS = 	src/main.c \
		src/parser.c \
		src/error.c \
		src/libft_tmp.c

OBJDIR = objs
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# Regla para compilar los .c en objs/
$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Crear carpeta objs si no existe
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re