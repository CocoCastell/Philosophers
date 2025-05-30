#.SILENT:
.PHONY:			all re clean fclean

NAME				=		philo
BONUS				=		philo_bonus
INC					=		includes/
FLAGS				=		-Wall -Werror -Wextra -pthread -I$(INC)
CC					=		cc

# Mandatory
SRCS_DIR		=		srcs/
OBJS_DIR		=		objs/
SRCS				=		main.c \
								parsing.c \
								diner_manager.c \
								diner_utils.c \
								thread_mutex_manager.c \
								state_tracking.c \
								init.c \
								utils.c
OBJS				=		$(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

# Bonus
SRCS_DIR_B	=		srcs_bonus/
OBJS_DIR_B	=		objs_bonus/
SRCS_B			=		main.c \
								parsing.c \
								diner_manager.c \
								thread_mutex_manager.c \
								diner_utils.c \
								init.c \
								utils.c
OBJS_B			=		$(addprefix $(OBJS_DIR_B), $(SRCS_B:.c=.o))

all: 						Makefile $(NAME)

$(NAME):				$(OBJS)
								$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o:	$(SRCS_DIR)%.c
								mkdir -p $(OBJS_DIR)
								$(CC) $(FLAGS) -c $< -o $@

bonus:					$(OBJS_B)
								$(CC) $(FLAGS) $(OBJS_B) -o $(BONUS)

$(OBJS_DIR_B)%.o:	$(SRCS_DIR_B)%.c	
								mkdir -p $(OBJS_DIR_B)
								$(CC) $(FLAGS) -c $< -o $@

clean:
								rm -rf $(OBJS_DIR) $(OBJS_DIR_B)

fclean:					clean
								rm $(NAME) $(BONUS)

re:							fclean all