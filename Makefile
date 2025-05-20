#.SILENT:
.PHONY: all re clean fclean

NAME		=	philo
FLAGS		=	-Wall -Werror -Wextra -pthread -I$(INC)
CC		=	cc

INC		=	includes/
SRCS_DIR	=	srcs/
OBJS_DIR	=	objs/
SRCS		=	main.c \
			parsing.c \
			thread_manager.c \
			utils.c

OBJS		=	$(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

all: Makefile $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o:$(SRCS_DIR)%.c
	mkdir -p $(OBJS_DIR)
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean:	clean
	rm $(NAME)

re:	fclean all
