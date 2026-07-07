NAME = ft_ping

CC = gcc
CFLAGS = -Wall -Wextra -Werror -Iincludes

SRC_DIR = src
OBJ_DIR = obj

SRC = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/signal.c \
	$(SRC_DIR)/stats.c \
	$(SRC_DIR)/send.c \
	$(SRC_DIR)/resolve.c \
	$(SRC_DIR)/receive.c \
	$(SRC_DIR)/packet.c \
	$(SRC_DIR)/loop.c \
	$(SRC_DIR)/init.c \
	$(SRC_DIR)/usage.c\
	$(SRC_DIR)/verbose.c

OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

RM = rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re